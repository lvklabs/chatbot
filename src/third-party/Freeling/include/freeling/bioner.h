//////////////////////////////////////////////////////////////////
//
//    FreeLing - Open Source Language Analyzers
//
//    Copyright (C) 2004   TALP Research Center
//                         Universitat Politecnica de Catalunya
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//    contact: Lluis Padro (padro@lsi.upc.es)
//             TALP Research Center
//             despatx C6.212 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
////////////////////////////////////////////////////////////////

#ifndef _BIONER
#define _BIONER

#include <set>

#include "fries.h"
#include "omlet.h"



//---------- Visible Viterbi Class  ----------------------------------

////////////////////////////////////////////////////////////////
///  The class vis_viterbi implements the viterbi algorithm
///   given the weights of different labels, without
///   hidden information
////////////////////////////////////////////////////////////////

class vis_viterbi {

  private:

    ///N:  number of classes
    int N;
    /// p_ini: vector with initial probabilities for each class
    std::vector<double> p_ini;
    /// p_trans: matrix with the probability transitions from one class to another
    // e.g. P(B,B), P(O,B), etc
    std::vector<std::vector<double> > p_trans;

  public:
    /// Constructor: Create dynammic storage for the best path
    vis_viterbi (const std::string &);

    /// find_best_path: perform viterbi algortihm given the weights matrix
    std::vector<int> find_best_path (double**, int) const;

};


//---------- bioner Class  ----------------------------------

////////////////////////////////////////////////////////////////
///  The class bioner implements an AdaBoost-Based NE recognizer
////////////////////////////////////////////////////////////////

class bioner: public ner {
  
  private:
    /// translation from name to class number
    std::map<int,std::string> class_name;
    /// feature extractor
    fex* extractor;
    /// lexicon to translate symbolic features to integer indexes
    std::map<std::string,int> lexicon;
    /// adaboost classifier
    adaboost* classifier;
    /// viterbi class to compute best path
    vis_viterbi vit;

  public:
    /// Constructor
    bioner ( const std::string & );
    /// Destructor
    ~bioner();

    /// Recognize NEs in given sentence
    //    void analyze ( std::list<sentence> & ) const;
    void annotate ( sentence & );
    
    /// Build a Multiword and sets its analysis
    sentence::iterator BuildMultiword(sentence &, sentence::iterator, sentence::iterator, bool&) const;
    void SetMultiwordAnalysis(sentence::iterator) const;
    bool ValidMultiWord(const word &) const;
};

#endif

