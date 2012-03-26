#include "aimlengine.h"
#include "nlprule.h"
#include "nullsanitizer.h"

#include "ProgramQ/aimlparser.h"

#include <QStringList>
#include <QFile>
#include <cassert>


//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

// getCategoryId provide a simple way to map two numbers in one unique number
// getRuleId and getInputNumber retrieve the original values
// Asumming sizeof(long) == 8, that's true for most modern archs
// 2^12 = 4K input rules should be far enough and we have 2^52 different rules ids

const int  INPUT_NUMBER_BITS = 12; 
const long INPUT_NUMBER_MASK = 0xfffl;

//--------------------------------------------------------------------------------------------------

long getCategoryId(Lvk::Nlp::RuleId ruleId, int inputNumber)
{
    return ((long)ruleId << INPUT_NUMBER_BITS) | (long)inputNumber;
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleId getRuleId(long categoryId)
{
    return categoryId >> INPUT_NUMBER_BITS;
}

//--------------------------------------------------------------------------------------------------

int getInputNumber(long categoryId)
{
    return categoryId & INPUT_NUMBER_MASK;
}

} // namespace

//--------------------------------------------------------------------------------------------------
// AimlEngine
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::AimlEngine()
    : m_aimlParser(0), m_sanitizer(new Lvk::Nlp::NullSanitizer())
{
    init();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::AimlEngine(Sanitizer *sanitizer)
    : m_aimlParser(0), m_sanitizer(sanitizer)
{
    init();
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::AimlEngine::~AimlEngine()
{
    delete m_sanitizer;
    delete m_aimlParser;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::init()
{
    QFile *logfile = new QFile("aiml_parser.log");
    logfile->open(QFile::WriteOnly);
    m_aimlParser = new AIMLParser(logfile);
}

//--------------------------------------------------------------------------------------------------

const Lvk::Nlp::RuleList & Lvk::Nlp::AimlEngine::rules() const
{
    return m_rules;
}

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::RuleList & Lvk::Nlp::AimlEngine::rules()
{
    return m_rules;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::setRules(const Lvk::Nlp::RuleList &rules)
{
    m_rules = rules;

    QString aiml;
    buildAiml(aiml);
    m_aimlParser->loadAimlFromString(aiml);
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::AimlEngine::getResponse(const QString &input, MatchList &matches)
{
    matches.clear();

    MatchList allMatches;
    QList<QString> responses = getAllResponses(input, allMatches);

    if (!allMatches.empty()) {
        matches.append(allMatches.first());

        assert(!responses.empty());
        return responses.first();
    } else {
        return "";
    }
}

//--------------------------------------------------------------------------------------------------

QList<QString> Lvk::Nlp::AimlEngine::getAllResponses(const QString &input, MatchList &matches)
{
    QString sanitizedInput = m_sanitizer->sanitize(input);

    QList<long> categoriesId;
    QString response = m_aimlParser->getResponse(sanitizedInput, categoriesId);

    QList<QString> responses;

    if (response != "Internal Error!" && categoriesId.size() > 0) { // FIXME harcoded string
        responses.append(response);

        long catId = categoriesId.last();
        matches.append(QPair<RuleId, int>(getRuleId(catId), getInputNumber(catId)));
    }

    return responses;
}

//--------------------------------------------------------------------------------------------------

void Lvk::Nlp::AimlEngine::buildAiml(QString &aiml)
{
    aiml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
    aiml += "<aiml>";

    for (int i = 0; i < m_rules.size(); ++i) {
        QStringList input = m_sanitizer->sanitize(m_rules[i].input());
        const QStringList &output = m_rules[i].output();

        for (int j = 0; j < input.size(); ++j) {
            QString categoryId = QString::number(getCategoryId(m_rules[i].id(), j));

            aiml += "<category>";
            aiml += "<id>" + categoryId + "</id>";
            aiml += "<pattern>" + input[j] + "</pattern>";

            if (output.size() == 1) {
                aiml += "<template>" + output[0] + "</template>";
            } else if (output.size() > 1) {
                aiml += "<template><random>";
                for (int k = 0; k < output.size(); ++k) {
                    aiml += "<li>" + output[k] + "</li>";
                }
                aiml += "</random></template>";
            }

            aiml += "</category>";
        }
    }

    aiml += "</aiml>";
}


