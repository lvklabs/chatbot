#include "aimlengine.h"
#include "nlprule.h"
#include "random.h"

#include "ProgramQ/aimlparser.h"

#include <QStringList>
#include <QFile>
#include <cassert>


Lvk::Nlp::AimlEngine::AimlEngine()
    : m_aimlParser(0)
{
    QFile *logfile = new QFile("aiml_parser.log");
    logfile->open(QFile::WriteOnly);
    m_aimlParser = new AIMLParser(logfile);
}

Lvk::Nlp::AimlEngine::~AimlEngine()
{
    delete m_aimlParser;
}

const Lvk::Nlp::RuleList & Lvk::Nlp::AimlEngine::rules() const
{
    return m_rules;
}

Lvk::Nlp::RuleList & Lvk::Nlp::AimlEngine::rules()
{
    return m_rules;
}

void Lvk::Nlp::AimlEngine::setRules(const Lvk::Nlp::RuleList &rules)
{
    m_rules = rules;

    QString aiml;
    buildAiml(aiml);
    m_aimlParser->loadAimlFromString(aiml);
}

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

QList<QString> Lvk::Nlp::AimlEngine::getAllResponses(const QString &input, MatchList &matches)
{
    QString response = m_aimlParser->getResponse(input);

    QList<QString> responses;

    if (response != "Internal Error!") {
        responses.append(response);
        matches.append(QPair<RuleId, int>(0, 0)); // FIXME
    }

    return responses;
}

void Lvk::Nlp::AimlEngine::buildAiml(QString &aiml)
{
    aiml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
    aiml += "<aiml>";

    for (int i = 0; i < m_rules.size(); ++i) {
        const QStringList &input = m_rules[i].input();
        const QStringList &output = m_rules[i].output();
        for (int j = 0; j < input.size(); j++) {
            if (output.size() == 1) {
                aiml += QString("<category><pattern>%1</pattern><template>%2</template></category>")
                        .arg(input[j])
                        .arg(output[0]);
            } else if (output.size() > 1) {
                aiml += QString("<category><pattern>%1</pattern><template>%2</template></category>")
                        .arg(input[j])
                        .arg(output[0]); // FIXME
            }
        }
    }

    aiml += "</aiml>";
}

