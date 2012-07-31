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

#ifndef _RELAX_TAGGER
#define _RELAX_TAGGER

#include <list> 
#include <string>

#include "fries/language.h"
#include "freeling/tagger.h"
#include "freeling/relax.h"
#include "freeling/constraint_grammar.h"

#include "regexp-pcre++.h"

// RegExp to detect user field usage
#define USER_RE "^u\\.([0-9]+)=(.+)$"


////////////////////////////////////////////////////////////////
///
///  The class relax_tagger implements a PoS tagger based on
/// relaxation labelling algorithm. It does so using the 
/// generic solver implemented by class relax.
///
////////////////////////////////////////////////////////////////

class relax_tagger : public POS_tagger {
   private:
      /// Generic solver instance
      relax solver;
      /// PoS constraints.
      constraint_grammar c_gram;

      // RegExp to detect user field usage. It's stored here precompiled for efficiency.
      RegEx RE_user;

      /// check a condition of a RuleCG.
      /// Add to the given constraint& solver-encoded constraint info for the condition
      bool CheckCondition(const sentence &, sentence::const_iterator, int, 
                          const condition &, std::list<std::list<std::pair<int,int> > > &);
      /// check whether a word matches a simple list of terms.
      /// Return (via list<pair<int,int>>&) a solver-encoded term for the condition
      bool CheckWordMatchCondition(const std::list<std::string> &, bool, int, sentence::const_iterator,
                                   std::list<std::pair<int,int> > &);
      /// check whether a word matches one of all possible condition patterns
      bool check_possible_matching(const std::string &, word::const_iterator,
                                   sentence::const_iterator); 
      /// check a match of two literals, taking into account lemma, tag, wildcards, etc.
      bool check_match(const std::string &, const std::string &) const;

   public:
       /// Constructor, given the constraint file and config parameters
       relax_tagger(const std::string &, int, double, double, bool, unsigned int);

       /// analyze sentences with default options
       void analyze(std::list<sentence> &);
       /// analyze sentences and return analyzed copy (for java API)
       std::list<sentence> analyze(const std::list<sentence> &);
};


#endif
