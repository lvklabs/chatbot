#include "nlp-engine/defaultlemmatizer.h"

#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "freeling.h"

//--------------------------------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

const std::string DATA_PATH                 = "../../third-party/Freeling/data/es/";
const std::string TOKENIZER_CONFIG_FILE     = DATA_PATH + "tokenizer.dat";
const std::string SPLITTER_CONFIG_FILE      = DATA_PATH + "splitter.dat";
const std::string PROB_CONFIG_FILE          = DATA_PATH + "probabilitats.dat";
const std::string QUANTITIES_CONFIG_FILE    = DATA_PATH + "quantities.dat";
const std::string DICTIONARY_FILE           = DATA_PATH + "dicc.src";


void convert(const std::list<sentence> &ls, QString &str)
{
    for (list<sentence>::const_iterator lit = ls.begin(); lit != ls.end(); ++lit) {
        for (sentence::const_iterator wit = lit->begin(); wit != lit->end(); ++wit) {
            if (str.size() > 0 && wit->get_form() != ".") {
                str += " ";
            }
            str += QString::fromStdString(wit->get_form());
        }
    }
}

} // namespace


//--------------------------------------------------------------------------------------------------
// Lemmatizer
//--------------------------------------------------------------------------------------------------

Lvk::Nlp::DefaultLemmatizer::DefaultLemmatizer() :
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

//--------------------------------------------------------------------------------------------------

Lvk::Nlp::DefaultLemmatizer::~DefaultLemmatizer()
{
    delete m_morfo;
    delete m_sp;
    delete m_tk;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::Nlp::DefaultLemmatizer::lemmatize(const QString &input)
{
    unsigned long offs = 0;
    std::list<word> av = m_tk->tokenize(input.toStdString(), offs);
    std::list<sentence> ls = m_sp->split(av, false);
    m_morfo->analyze(ls);

    QString output;
    convert(ls, output);

    return output;
}
