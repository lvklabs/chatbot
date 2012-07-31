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

#ifndef _SPLITTER
#define _SPLITTER

#include <map>
#include <set>

#include "fries/language.h"


////////////////////////////////////////////////////////////////
///  Class splitter implements a sentence splitter, 
///  which accumulates lists of words until a sentence 
///  is completed, and then returns a list of 
///  sentence objects.
////////////////////////////////////////////////////////////////

class splitter {
   private:
      /// configuration options
      bool SPLIT_AllowBetweenMarkers;
      int SPLIT_MaxLines;
      /// Sentence delimiters
      std::set<std::string> starters;
      std::map<std::string,bool> enders;
      /// Open-close marker pairs (parenthesis, etc)
      std::map<std::string,int> markers;
  
      // current state
      bool betweenMrk;
      int no_split_count;
      std::list<int> mark_type;
      std::list<std::string> mark_form;

      /// accumulated list of returned sentences 
      //      std::list<sentence> ls; 
      /// accumulated words of current sentence
      sentence buffer; 

      /// check for sentence markers
      bool end_of_sentence(std::list<word>::const_iterator, const std::list<word> &) const;

   public:
      /// Constructor
      splitter(const std::string &);

      /// split sentences with default options
      void split(const std::list<word> &, bool, std::list<sentence> &ls);
      std::list<sentence> split(const std::list<word> &, bool);
};

#endif










