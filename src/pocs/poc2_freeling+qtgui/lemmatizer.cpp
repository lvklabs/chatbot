#include "lemmatizer.h"

#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace
{

const std::string DATA_PATH                 = "../../third-party/Freeling/data/es/";
const std::string TAGS_FILE                 = DATA_PATH + "tags.dat";
const std::string TOKENIZER_CONFIG_FILE     = DATA_PATH + "tokenizer.dat";
const std::string SPLITTER_CONFIG_FILE      = DATA_PATH + "splitter.dat";
const std::string PROB_CONFIG_FILE          = DATA_PATH + "probabilitats.dat";
const std::string QUANTITIES_CONFIG_FILE    = DATA_PATH + "quantities.dat";
const std::string DICTIONARY_FILE           = DATA_PATH + "dicc.src";
const std::string CORRECTOR_FILE            = DATA_PATH + "corrector/corrector.rules";
const std::string LOCUTIONS_FILE            = DATA_PATH + "locucions.dat";
const std::string AFFIXES_FILE              = DATA_PATH + "afixos.dat";
const std::string PUNCTUATION_FILE          = TAGS_FILE;

}

Lemmatizer::Lemmatizer() :
    m_tk(new tokenizer(TOKENIZER_CONFIG_FILE)),
    m_sp(new splitter(SPLITTER_CONFIG_FILE)),
    m_morfo(0)
{
    maco_options opt("es");

    // Initially set all modules disabled:

    opt.set_active_modules(false, false, false, false, false, false, false, false, NER_NONE, false);
    opt.set_data_files("", "", "", "", "", "", "", "");

    // Enable modules one by one:

    opt.ProbabilityAssignment = true;
    opt.DictionarySearch      = true;
    opt.QuantitiesDetection   = true;
    opt.NumbersDetection      = true;
    opt.AffixAnalysis         = true;
    opt.DatesDetection        = true;
    opt.PunctuationDetection  = true;
    //opt.OrthographicCorrection = false; // According documentation this module is under
                                          // development, and it is not operational yet.

    opt.ProbabilityFile = PROB_CONFIG_FILE;
    opt.DictionaryFile  = DICTIONARY_FILE;
    opt.QuantitiesFile  = QUANTITIES_CONFIG_FILE;
    opt.AffixFile       = AFFIXES_FILE;
    opt.CorrectorFile   = CORRECTOR_FILE;
    opt.LocutionsFile   = LOCUTIONS_FILE;
    opt.PunctuationFile = PUNCTUATION_FILE;

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
