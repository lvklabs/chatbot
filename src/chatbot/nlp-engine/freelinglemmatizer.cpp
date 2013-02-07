#include "nlp-engine/freelinglemmatizer.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QFile>
#include <QStringList>
#include <QtDebug>
#include <list>
#include <string>

#include "freeling.h"

#define KEY_TOKENIZER_FILE  1
#define KEY_SPLITTER_FILE   2
#define KEY_PROBS_FILE      3
#define KEY_QUANTS_FILE     4
#define KEY_DICT_FILE       5
#define KEY_AFFIXES_FILE    6
#define KEY_LOCUTIONS_FILE  7
#define KEY_PUNCT_FILE      8

typedef QHash<int, std::string> ConfigFilesMap;


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline std::string getLang()
{
    Lvk::Cmn::Settings settings;
    QString lang = settings.value(SETTING_NLP_LANGUAGE).toString();

    if (lang.isEmpty()) {
        lang = settings.value(SETTING_APP_LANGUAGE).toString().split("_").at(0);
    }

    return lang.toStdString();
}

//--------------------------------------------------------------------------------------------------

inline void getFlConfigFiles(ConfigFilesMap &configFiles)
{
    Lvk::Cmn::Settings settings;
    std::string dataPath = settings.value(SETTING_DATA_PATH).toString().toStdString();
    std::string flDataPath = dataPath + "/freeling/" + getLang() + "/";

    configFiles[KEY_TOKENIZER_FILE] = flDataPath + "tokenizer.dat";
    configFiles[KEY_SPLITTER_FILE]  = flDataPath + "splitter.dat";
    configFiles[KEY_PROBS_FILE]     = flDataPath + "probabilitats.dat";
    configFiles[KEY_QUANTS_FILE]    = flDataPath + "quantities.dat";
    configFiles[KEY_DICT_FILE]      = flDataPath + "dicc.src";
    configFiles[KEY_AFFIXES_FILE]   = flDataPath + "afixos.dat";
    configFiles[KEY_LOCUTIONS_FILE] = flDataPath + "locucions.dat";
    configFiles[KEY_PUNCT_FILE]     = flDataPath + "tags.dat";
}

//--------------------------------------------------------------------------------------------------

inline bool exists(ConfigFilesMap &configFiles)
{
    ConfigFilesMap::const_iterator it;

    for (it = configFiles.constBegin(); it != configFiles.constEnd(); ++it) {
        const char* const filename = it.value().c_str();
        if (!QFile::exists(filename)) {
            qCritical() << "File not found" << filename;
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------------------------

template<class T>
inline void init(T** p, const std::string &configFile)
{
    *p  = new T(configFile);
}

//--------------------------------------------------------------------------------------------------

inline void init(maco** p, const ConfigFilesMap &configFiles)
{
    maco_options opt(getLang());

    // Set all modules disabled:
    opt.set_active_modules(false, false, false, false, false, false, false, false, NER_NONE, false);
    opt.set_data_files("", "", "", "", "", "", "", "");

    // Enable one by one:
    opt.ProbabilityAssignment = true;
    opt.DictionarySearch      = true;
    opt.QuantitiesDetection   = true;
    opt.NumbersDetection      = true;
    opt.AffixAnalysis         = true;
    //opt.DatesDetection        = true;
    opt.PunctuationDetection  = true;
    opt.MultiwordsDetection   = true;
    opt.ProbabilityFile       = configFiles[KEY_PROBS_FILE];
    opt.DictionaryFile        = configFiles[KEY_DICT_FILE];
    opt.QuantitiesFile        = configFiles[KEY_QUANTS_FILE];
    opt.AffixFile             = configFiles[KEY_AFFIXES_FILE];
    opt.LocutionsFile         = configFiles[KEY_LOCUTIONS_FILE];
    opt.PunctuationFile       = configFiles[KEY_PUNCT_FILE];

    *p = new maco(opt);
}

//--------------------------------------------------------------------------------------------------

// Required for split() to mark the end of the sentence, otherwise returns an empty
// list and waits for more input.
//
// We need to append " ." instead of just "." because inputs like "Hi A" splits into:
// "Hi", "A."
// Freeling gets confused and thinks that "A." is an abbrevation, should split into:
// "Hi", "A", "."
//
// For more details see https://github.com/lvklabs/chatbot/issues/33
inline QString addFullStop(const QString &input)
{
    QString s = input.trimmed();

    if (!s.endsWith(" .")) {
        s.append(" .");
    }

    return s;
}

//--------------------------------------------------------------------------------------------------

inline void convert(const std::list<word> &lw, QStringList &l)
{
    l.clear();

    for (std::list<word>::const_iterator wit = lw.begin(); wit != lw.end(); ++wit) {
        l.append(QString::fromStdString(wit->get_form()));
    }
}

//--------------------------------------------------------------------------------------------------

inline void convert(const std::list<sentence> &ls, Lvk::Nlp::WordList &l)
{
    l.clear();

    for (list<sentence>::const_iterator lit = ls.begin(); lit != ls.end(); ++lit) {
        for (sentence::const_iterator wit = lit->begin(); wit != lit->end(); ++wit) {
            Lvk::Nlp::Word w;
            w.origWord = QString::fromStdString(wit->get_form());
            w.normWord = QString::fromStdString(wit->get_form());
            w.lemma    = QString::fromStdString(wit->get_lemma());
            w.posTag   = QString::fromStdString(wit->get_parole());
            l.append(w);
        }
    }
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Lemmatizer
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::FreelingLemmatizer::FreelingLemmatizer()
    : m_flInit(false), m_tk(0), m_sp(0), m_morpho(0)
{
    ConfigFilesMap configFiles;

    getFlConfigFiles(configFiles);

    if (exists(configFiles)) {
        qDebug() << "Initializing Freeling...";
        init(&m_tk, configFiles[KEY_TOKENIZER_FILE]);
        init(&m_sp, configFiles[KEY_SPLITTER_FILE]);
        init(&m_morpho, configFiles);
    }

    if (m_tk && m_sp && m_morpho) {
        m_flInit = true;
    } else {
        qCritical() << "Freeling could not be initialized. Lemmatization is disabled.";
    }
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::FreelingLemmatizer::~FreelingLemmatizer()
{
    delete m_morpho;
    delete m_sp;
    delete m_tk;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::FreelingLemmatizer::tokenize(const QString &input, QStringList &l)
{
    if (m_flInit) {
        std::list<word> lw;
        m_tk->tokenize(addFullStop(input).toStdString(), lw);

        convert(lw, l);
    } else {
        qCritical() << "Freeling could not be initialized. Lemmatization is disabled.";
    }

    qDebug() << "   - Tokenized:" << input << "->" << l;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::FreelingLemmatizer::lemmatize(const QString &input, Nlp::WordList &l)
{
    if (m_flInit) {
        std::list<word> lw;
        m_tk->tokenize(addFullStop(input).toStdString(), lw);

        std::list<sentence> ls;
        m_sp->split(lw, false, ls);

        m_morpho->analyze(ls);

        convert(ls, l);
    } else {
        qCritical() << "Freeling could not be initialized. Lemmatization is disabled.";
    }

    qDebug() << "   - Lemmatized:" << input << "->" << l;
}
