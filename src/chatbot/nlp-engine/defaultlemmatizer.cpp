#include "nlp-engine/defaultlemmatizer.h"
#include "common/settings.h"
#include "common/settingskeys.h"

#include <QFile>
#include <QStringList>
#include <QtDebug>
#include <list>
#include <string>

#include "freeling.h"

#define KEY_TOKENIZER_FILE  "tokenizer"
#define KEY_SPLITTER_FILE   "splitter"
#define KEY_PROBS_FILE      "probs"
#define KEY_QUANTS_FILE     "quants"
#define KEY_DICT_FILE       "dict"

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline QString getLang()
{
    Lvk::Cmn::Settings settings;
    QString lang = settings.value(SETTING_NLP_LANGUAGE).toString();

    if (lang.isEmpty()) {
        lang = settings.value(SETTING_APP_LANGUAGE).toString().split("_").at(0);
    }

    return lang;
}


inline void getFlConfigFiles(QHash<QString, QString> &configFiles)
{
    Lvk::Cmn::Settings settings;
    QString dataPath = settings.value(SETTING_DATA_PATH).toString();
    QString flDataPath = dataPath + "/freeling/" + getLang() + "/";

    configFiles[KEY_TOKENIZER_FILE] = flDataPath + "tokenizer.dat";
    configFiles[KEY_SPLITTER_FILE]  = flDataPath + "splitter.dat";
    configFiles[KEY_PROBS_FILE]     = flDataPath + "probabilitats.dat";
    configFiles[KEY_QUANTS_FILE]    = flDataPath + "quantities.dat";
    configFiles[KEY_DICT_FILE]      = flDataPath + "dicc.src";
}

//--------------------------------------------------------------------------------------------------

inline bool exists(QHash<QString, QString> &configFiles)
{
    QHash<QString, QString>::const_iterator it;

    for (it = configFiles.constBegin(); it != configFiles.constEnd(); ++it) {
        if (!QFile::exists(it.value())) {
            qCritical() << "File not found" << it.value();
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------------------------

template<class T>
inline void init(T** p, const QString &configFile)
{
    *p  = new T(configFile.toStdString());
}

//--------------------------------------------------------------------------------------------------

inline void init(maco** p, const QHash<QString, QString> &configFiles)
{
    maco_options opt(getLang().toStdString());

    // We use the bare minimum of modules(ner = NONE = no NER)
    opt.set_active_modules(false, false, false, false, false, false, false, false, NER_NONE, false);
    opt.set_data_files("", "", "", "", "", "", "", "");

    opt.ProbabilityAssignment = true;
    opt.DictionarySearch      = true;
    opt.QuantitiesDetection   = true;
    opt.ProbabilityFile       = configFiles[KEY_PROBS_FILE].toStdString();
    opt.DictionaryFile        = configFiles[KEY_DICT_FILE].toStdString();
    opt.QuantitiesFile        = configFiles[KEY_QUANTS_FILE].toStdString();

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

Lvk::Nlp::DefaultLemmatizer::DefaultLemmatizer()
    : m_flInit(false), m_tk(0), m_sp(0), m_morpho(0)
{
    QHash<QString, QString> configFiles;

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

Lvk::Nlp::DefaultLemmatizer::~DefaultLemmatizer()
{
    delete m_morpho;
    delete m_sp;
    delete m_tk;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::DefaultLemmatizer::lemmatize(const QString &input)
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

    qDebug() << "Lemmatized:" << input << "->" << output;

    return output;
}
