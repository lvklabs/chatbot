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

#ifndef _PHONETICS
#define _PHONETICS

#include <string>
#include <map>
#include <vector>
#include "freeling/soundChange.h"

using namespace std;



////////////////////////////////////////////////////////////////
/// This class is a will calculate the phonetic translation of a word 
////////////////////////////////////////////////////////////////

class phonetics {

   private:
	
	/// language freeling is working on
	string language;
	/// If we use the dictionary for phonetic translation
	bool useDict;
	///variables and valor extracted from rule file for the translation
	map<string, string> vars;
	/// internal chace with phonetic translation of previous words
	map<string, string> cache;
	/// rules extracted from the rule file
	vector <string> rules;
	/// translation rules from the sampa file
	map<string, string> SAMPA;
	/// words and his phonetic translation from the dictionary file
	map<string, string> DIC;
	/// class that apply the rules to the variables for the phonetic translation
	soundChange* sc;

	

   public:
      /// Constructor with language, rule file, dictFile, sampaFile, useDictFile
      phonetics(string, string, string, bool);
      /// Destructor
      ~phonetics();

	/// Returns the phonetic sound of the word
	string getSound(string);



};



#endif
 
