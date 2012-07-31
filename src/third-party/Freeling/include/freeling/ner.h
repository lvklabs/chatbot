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

#ifndef _NER
#define _NER

#include <set>

#include "fries.h"
#include "omlet.h"

// codes for NER algorithms
#define NER_BASIC 0
#define NER_BIO 1
#define NER_NONE 2


////////////////////////////////////////////////////////////////
///  The class ner is an abstract class that implements a general NE Recognizer
////////////////////////////////////////////////////////////////

class ner {
  
  protected: 
    /// length beyond which a multiword made of all capitialized words ("WRECKAGE: TITANIC 
    /// DISAPPEARS IN NORTHERN SEA") will be considered a title and not a proper noun.
    /// A value of zero deactivates this behaviour.
    unsigned int Title_length;
    
    /// Tag to assign to detected NEs
    std::string NE_tag;
    
    /// if we want to split NEs, set this to true
    bool splitNPs;

 public:
      /// Detect NEs in sentence
      virtual void annotate(sentence &)=0 ;
};

#endif

