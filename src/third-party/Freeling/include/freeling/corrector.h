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

#ifndef _CORRECTOR
#define _CORRECTOR

#include "fries/language.h"
#include "freeling/database.h"
#include "freeling/dictionary.h"
#include "freeling/phonetics.h"
#include "freeling/phoneticDistance.h"
#include "freeling/similarity.h"

#include "regexp-pcre++.h"

////////////////////////////////////////////////////////////////
///
///  The class corrector implements a spell checker that will try 
/// to correct spelling errors
///
////////////////////////////////////////////////////////////////

#define EDIT_DISTANCE 1
#define PHONETIC_DISTANCE 2

class corrector {
   private:
      
      /// indexed file with similar words
      database similar_words;
	
      /// The dictionary that FreeLing is currently using
      dictionary* dict; 
      /// The class that translates a word into phonetic sounds
      phonetics* ph;
      /// Whether phonetic conversions should use a sound dictionary
      bool useSoundDict;
      /// The class that calculate the phonetic distante
      /// between two phonetic transcriptions
      phoneticDistance* phd;
      /// the class that computes edit distance between two words
      similarity* sm;
      /// The method to calculate the distance between words
      int distanceMethod;
      /// Minimum similariy to consider a word as a possible correction for a known word
      double SimThresholdKnown;
      /// Minimum similariy to consider a word as a possible correction for an
      /// unknown word, when no exact phonetic match is found
      double SimThresholdUnknownLow;
      /// Minimum similariy to consider a word as a possible correction for an
      /// unknown word, when some exact phonetic match is found
      double SimThresholdUnknownHigh;
      /// Maximum lentgh difference to consider a word as a possible correction
      int MaxSizeDiff;
      /// contains the tags for which the words that are present 
      /// in the dictionary will be checked 
      RegEx dictionaryCheck;
      ///  whether words not present in the dictionary are to be spell checked
      bool noDictionaryCheck;
      /// returns the consonant key of a word
      std::string getKey(std::string);
      /// adds the new words that are posible correct spellings from original word to the word analysys data
      void putWords(std::string, word &);
           
 public:
      /// Constructor
      corrector(const std::string &, dictionary &);
      /// Destructor
      ~corrector();
      
      /// Navigates the sentence adding alternative words (possible correct spelling data)
      void annotate(sentence &);
};

#endif
