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

#ifndef _DICTIONARY
#define _DICTIONARY

#include <map>

#include "fries/language.h"
#include "freeling/database.h"
#include "freeling/suffixes.h"


////////////////////////////////////////////////////////////////
///
///  The class dictionary implements dictionary search and suffix
///  analysis for word forms. 
///
////////////////////////////////////////////////////////////////

class dictionary {
   private:
      /// configuration options
      bool AffixAnalysis;

      /// suffix analyzer
      affixes* suf;

      /// C++ Interface to BerkeleyDB C API to acces the dictionary
      database morfodb;

      /// Fills the analysis list of a word and checks for suffixes
      void annotate_word(word &);
      /// check whether the word is a contraction, and if so, fill the list with the contracted words
      bool check_contracted(const word &, std::list<word> &);

   public:
      /// Constructor
      dictionary(const std::string &, const std::string &, bool, const std::string &);
      /// Destructor
      ~dictionary();

      /// Get the analysis list from a given form 
      void search_form(const std::string &, std::list<analysis> &);
      /// Search words in sentence using default options
      void annotate(sentence &);
};

#endif
