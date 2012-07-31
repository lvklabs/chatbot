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

#ifndef _POS_TAGGER
#define _POS_TAGGER

#include <list> 
#include "fries/language.h"

#define FORCE_NONE   0  // no selection forced
#define FORCE_TAGGER 1  // force select after tagger
#define FORCE_RETOK  2  // force select after retokenization

////////////////////////////////////////////////////////////////
///
///  The class POS_tagger is just an abstract class generalizing a PoS tagger.
///
////////////////////////////////////////////////////////////////

class POS_tagger {

   protected: 
      // remember whether retokenization is active
      bool retok;
      // remember whether the user asked to force only one tag per word
      unsigned int force;
      // retokenize words that may need it after tagging
      void retokenize(std::list<sentence> &);
      // force the selection of only one Pos tag per word
      void force_select(std::list<sentence> &);

   public:
      POS_tagger(bool,unsigned int);
      virtual ~POS_tagger() {};
      virtual void analyze(std::list<sentence> &)=0;
};


#endif
