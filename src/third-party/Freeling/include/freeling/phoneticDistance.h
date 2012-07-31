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

#ifndef _PHONETICDISTANCE
#define _PHONETICDISTANCE

#include <string>
#include "aligner.h"
#include "golem.h"
#include "simplesearch.h"

using namespace std;

////////////////////////////////////////////////////////////////
/// This class is wrapper to calculate the phonetic distance between two words
////////////////////////////////////////////////////////////////

class phoneticDistance {

   private:
	
	/// object that calculate the phonetic distance between toe words
	aligner<int>* al;
	

   public:
      /// Constructor with the path of the features file as argument
      phoneticDistance(string);
      /// Destructor
      ~phoneticDistance();

	/// Returns the phonetic distance between two words
	int getPhoneticDistance(string,string);


};

#endif
 
 
