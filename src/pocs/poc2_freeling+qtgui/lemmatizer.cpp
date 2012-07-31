#include "lemmatizer.h"

#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace
{

const std::string DATA_PATH                 = "../../third-party/Freeling/data/es/";
const std::string TOKENIZER_CONFIG_FILE     = DATA_PATH + "tokenizer.dat";
const std::string SPLITTER_CONFIG_FILE      = DATA_PATH + "splitter.dat";
const std::string PROB_CONFIG_FILE          = DATA_PATH + "probabilitats.dat";
const std::string QUANTITIES_CONFIG_FILE    = DATA_PATH + "quantities.dat";
const std::string DICTIONARY_FILE           = DATA_PATH + "dicc.src";

}

Lemmatizer::Lemmatizer() :
    m_tk(new tokenizer(TOKENIZER_CONFIG_FILE)),
    m_sp(new splitter(SPLITTER_CONFIG_FILE)),
    m_morfo(0)
{
    maco_options opt("es");

    // We use the bare minimum of modules(ner = NONE = no NER)
    opt.set_active_modules(false, false, false, false, false, false, false, false, NER_NONE, false);
    opt.set_data_files("", "", "", "", "", "", "", "");

    opt.ProbabilityAssignment = true;
    opt.DictionarySearch = true;
    opt.QuantitiesDetection = true;

    opt.ProbabilityFile = PROB_CONFIG_FILE;
    opt.DictionaryFile = DICTIONARY_FILE;
    opt.QuantitiesFile = QUANTITIES_CONFIG_FILE;

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
