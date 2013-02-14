#ifndef MOCKLEMMATIZER_H
#define MOCKLEMMATIZER_H

#include "nlp-engine/lemmatizer.h"
#include "nlp-engine/sanitizer.h"

class MockLemmatizer : public Lvk::Nlp::Lemmatizer
{
public:
    MockLemmatizer();

    ~MockLemmatizer();

    virtual void tokenize(const QString &input, QStringList &l);

    virtual void lemmatize(const QString &input, Lvk::Nlp::WordList &words);


private:
    MockLemmatizer(const MockLemmatizer&);
    MockLemmatizer & operator=(const MockLemmatizer&);

    Lvk::Nlp::Sanitizer *m_preSanitizer;
    Lvk::Nlp::Sanitizer *m_postSanitizer;

    QString getLemma(const QString &word);
};

#endif // MOCKLEMMATIZER_H
