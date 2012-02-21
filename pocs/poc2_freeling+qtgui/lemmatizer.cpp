#include "lemmatizer.h"

#include <sstream>
#include <iostream>
#include <map>
#include <vector>


Lemmatizer::Lemmatizer(const std::string &tokenizerConfigFile, const std::string &splitterConfigFile, const std::string &dictFile)
    : m_tk(new tokenizer(tokenizerConfigFile)), m_sp( new splitter(splitterConfigFile)), m_morfo(0)
{
    maco_options opt ("es");
    // We use the bare minimum of modules(ner = NONE = no NER)
    // set_active_modules(bool suf, bool mw, bool num, bool pun, bool dat, bool qt, bool dic, bool prb, int ner, bool corr)
    opt.set_active_modules(false, false, false, false, false, false, true, false, NER_NONE, false);
    opt.set_data_files("", "", "", "", dictFile, "", "", "");

    // create tha analyzer with the above options.
    m_morfo = new maco(opt);

}

Lemmatizer::~Lemmatizer()
{
    delete m_morfo;
    delete m_sp;
    delete m_tk;
}

std::list<sentence> Lemmatizer::lemmatize(const std::string &input)
{
    unsigned long offs = 0;
    std::list<word> av = m_tk->tokenize(input, offs);
    std::list<sentence> ls = m_sp->split(av, false);
    m_morfo->analyze(ls);

    return ls;
}
