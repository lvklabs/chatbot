#include "mocklemmatizer.h"
#include "nlp-engine/sanitizerfactory.h"
#include "nlp-engine/word.h"

#include <QRegExp>
#include <QtDebug>


//--------------------------------------------------------------------------------------------------
// MockLemmatizer
//--------------------------------------------------------------------------------------------------

MockLemmatizer::MockLemmatizer()
    : m_preSanitizer(0), m_postSanitizer(0)
{
    m_preSanitizer = Lvk::Nlp::SanitizerFactory().createPreSanitizer();
    m_postSanitizer = Lvk::Nlp::SanitizerFactory().createPostSanitizer();
}

//--------------------------------------------------------------------------------------------------

MockLemmatizer::~MockLemmatizer()
{
    delete m_postSanitizer;
    delete m_preSanitizer;
}

//--------------------------------------------------------------------------------------------------

void MockLemmatizer::tokenize(const QString &input, QStringList &l)
{
    QStringList tokens = input.split(QRegExp("\\b"), QString::SkipEmptyParts);

    qDebug() << "MockLemmatizer: tokens:" << tokens;

    //l = tokens.filter(QRegExp("^\\S+$"));
    foreach (const QString &t, tokens) {
        QString tt = t.trimmed();
        if (!tt.isEmpty()) {
            l.append(tt);
        }
    }

    qDebug() << "MockLemmatizer: ftokens:" << l;
}

//--------------------------------------------------------------------------------------------------

void MockLemmatizer::lemmatize(const QString &input, Lvk::Nlp::WordList &words)
{
    words.clear();

    m_preSanitizer->sanitize(input);

    QStringList tokens;
    tokenize(input, tokens);

    foreach (const QString &token, tokens) {
        Lvk::Nlp::Word w(token, m_postSanitizer->sanitize(token), m_postSanitizer->sanitize(token));
        words.append(w);
    }

    qDebug() << "MockLemmatizer: lemmas:" << words;
}
