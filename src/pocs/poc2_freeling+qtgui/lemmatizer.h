#ifndef LEMMATIZER_H
#define LEMMATIZER_H

#include <string>
#include "freeling.h"


class Lemmatizer
{
public:
    Lemmatizer();
    ~Lemmatizer();

    std::list<sentence> lemmatize(const std::string& input);

private:
    tokenizer *m_tk;
    splitter *m_sp;
    maco *m_morfo;
};

#endif // LEMMATIZER_H
