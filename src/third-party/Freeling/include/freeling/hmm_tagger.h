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

#ifndef _TAGGER
#define _TAGGER

#include <map>
#include <list>
#include <set>

#include "fries/language.h"
#include "freeling/tagger.h"

////////////////////////////////////////////////////////////////
///
///  The class viterbi stores the two maps for each
/// observation: The delta map stores the maximum probability
/// for each state in that observation, the phi map stores
/// the backpath to maximize the probability.
///  An instance of this class is created for each sentence
/// to be tagged, and destroyed when work is finished.
///
////////////////////////////////////////////////////////////////

class viterbi {  
  public:
    /// Constructor
    viterbi(int);
    /// Destructor
    ~viterbi();
  
    /// Space for delta tables used in Viterbi algorithm
    std::map <std::string, double> *delta_log;
    /// Space for phi tables used in Viterbi algorithm
    std::map <std::string, std::string> *phi;
};


////////////////////////////////////////////////////////////////
///
///  The class emission_states stores the list of 
/// states in the HMM that *may* be generating a given
/// word given the two previous words (and their valid tags).
///
////////////////////////////////////////////////////////////////

class emission_states: public std::set<std::string> {};


////////////////////////////////////////////////////////////////
///
///  The class hmm_tagger implements the syntactic analyzer
/// and is the main class, which uses all the others.
///
////////////////////////////////////////////////////////////////

class hmm_tagger: public POS_tagger {
   private:
      // Configuration options
      std::string Language;

      /// maps to store the probabilities
      std::map <std::string, double> PTag;
      std::map <std::string, double> PBg;
      std::map <std::string, double> PTrg;
      std::map <std::string, double> PInitial;
      std::map <std::string, double> PWord;

      /// set of hand-specified forbidden bigram and trigram transitions
      std::multimap <std::string, std::string> Forbidden;
      /// log prob for zero
      float ZERO_logprob;

      /// coeficients to compute linear interpolation
      double c[3];

      bool is_forbidden(const std::string &, sentence::const_iterator) const;
      double ProbA_log(const std::string &, const std::string &, sentence::const_iterator) const;
      double ProbB_log(const std::string &, const word &) const;
      double ProbPi_log(const std::string &) const;

      /// compute possible emission states for each word in sentence.
      std::list<emission_states> FindStates(const sentence &) const;

   public:
       /// Constructor
       hmm_tagger(const std::string &, const std::string &, bool, unsigned int);

       /// analyze given sentences 
       void analyze(std::list<sentence> &);
       /// analyze sentences and return analyzed copy (for java API)
       std::list<sentence> analyze(const std::list<sentence> &);
};

#endif

