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
    QString token;

    foreach (const QChar &c, input) {
        if (c.isLetterOrNumber()) {
            token.append(c);
        } else {
            if (!token.isEmpty()) {
                l.append(token);
                token.clear();
            }
            if (!c.isSpace()) {
                l.append(c);
            }
        }
    }

    if (!token.isEmpty()) {
        l.append(token);
    }

    qDebug() << "MockLemmatizer: tokens:" << l;
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
