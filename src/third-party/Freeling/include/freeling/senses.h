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

#ifndef _SENSES
#define _SENSES

#include <string>
#include <list>

#include "fries/language.h"
#include "freeling/semdb.h"

////////////////////////////////////////////////////////////////
/// Class senses implements a sense annotator
////////////////////////////////////////////////////////////////

class senses {
   private:
      semanticDB *semdb;
      /// flag to remember whether analysis are duplicated for each possible sense
      bool duplicate;

   public:
      /// Constructor
      senses(const std::string &, bool); 
      /// Destructor
      ~senses(); 
 
      /// sense annotate selected analysis for each word in given sentences
      void analyze(std::list<sentence> &);
      /// sense annotate selected analysis for each word in given sentence, return copy (useful for python API)
      std::list<sentence> analyze(const std::list<sentence> &);
};

#endif

