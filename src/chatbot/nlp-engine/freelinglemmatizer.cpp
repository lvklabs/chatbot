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

inline void convert(const std::list<sentence> &ls, QString &str)
{
    str.clear();

    for (list<sentence>::const_iterator lit = ls.begin(); lit != ls.end(); ++lit) {
        for (sentence::const_iterator wit = lit->begin(); wit != lit->end(); ++wit) {
            if (str.size() > 0 && wit->get_form() != ".") {
                str += " ";
            }
            str += QString::fromStdString(wit->get_lemma());
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

QString Lvk::Nlp::FreelingLemmatizer::lemmatize(const QString &input)
{
    QString output = input.trimmed();

    if (m_flInit) {
        if (!output.endsWith(".")) {
            output.append("."); // Required for split(), otherwise returns an empty list
        }

        std::list<word> lw;
        m_tk->tokenize(output.toStdString(), lw);

        std::list<sentence> ls;
        m_sp->split(lw, false, ls);

        m_morpho->analyze(ls);

        convert(ls, output);
    }

    qDebug() << "   - Lemmatized:" << input << "->" << output;

    return output;
}
