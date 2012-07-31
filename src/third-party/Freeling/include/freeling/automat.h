////////////////////////////////////////////////////////////////
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

#ifndef _AUTOMAT
#define _AUTOMAT

#include <set>

#include "fries/language.h"

#define MAX_STATES 100
#define MAX_TOKENS 50

////////////////////////////////////////////////////////////////
///
///   Abstract class to implement a 
///   Finite-State Automaton which is used by modules 
///   recognizing multiwords (dates, numbers, quantities, ...).
///
/// Details:
///
///   - Child classes must implement a DFA, by means
///   of a transition table. 
///   - IMPORTANT: Since many matches may be found in a
///   sentence, when arriving to a error or final state, 
///   the transition table must provide a default transition
///   to "stopState" where the automata will stop, and 
///   check for longest match found.
///   - Specific actions may be associated to each state 
///   using the "StateActions" virtual function. 
///   - Token codes for a word must be provided by the
///   "ComputeToken" virtual function.
///
///   Child classes must provide a constructor that:
///     - fills the "final" set
///     - fills the "trans" table
///     - sets initialState and stopState
///     - initializes any other information required by the child
///
///   Child classes must provide the virtual functions:
///
///      - virtual int ComputeToken(int,sentence::iterator, sentence &);
///       (computes the token code, given the state and word. 
///        Sentence reference is provided just in case extra info 
///        is necessary)
///
///      - virtual void ResetActions();
///      (reset any variables required by state actions)
///
///      -  virtual void StateActions(int, int, int, sentence::iterator);
///      (perform specific state actions)
///
///      - virtual void SetMultiwordAnalysis(sentence::iterator);
///      (once a mw has been detected and build, set its lemma&parole)
///
///   Child classes must declare and manage any private attribute or function
///   they may need to perform the expected computations
///
//////////////////////////////////////////////////////////////////

class automat {
 private:
      /// pure virtual function to be provided by the child class.
      /// Computes token code for current word in current state.
      virtual int ComputeToken(int, sentence::iterator &, sentence &) =0;
      /// pure virtual function to be provided by the child class .
      /// Resets automaton internal variables when a new search is started.
      virtual void ResetActions() =0;
      /// pure virtual function to be provided by the child class.
      /// Performs appropriate internal actions, given origin and destinanation
      /// states, token code and word.
      virtual void StateActions(int, int, int, sentence::const_iterator) =0;
      /// pure virtual function to be provided by the child class.
      /// Sets analysis for pattern identified as a multiword.
      virtual void SetMultiwordAnalysis(sentence::iterator, int) =0;
      /// virtual function (true by default). Allows the child class to perform a
      /// last-minute check before effectively building the multiword.
      virtual bool ValidMultiWord(const word &);

      /// Private function to re-arrange sentence when match found
      virtual sentence::iterator BuildMultiword(sentence &,sentence::iterator,sentence::iterator, int, bool &);

 protected:
      /// state code of initial state
      int initialState;
      /// state code for stop State
      int stopState;
      /// Transition tables
      int trans[MAX_STATES][MAX_TOKENS];
      /// set of final states
      std::set<int> Final;

 public:
      /// Constructor
      automat();
      /// Destructor
      virtual ~automat() {};

      /// Detect patterns in sentence
      void annotate(sentence &);
      /// Detect patterns starting at a specific word
      bool annotate(sentence &, sentence::iterator &);
};

#endif

