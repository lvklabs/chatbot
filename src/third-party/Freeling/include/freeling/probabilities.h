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

#ifndef _PROBABILITIES
#define _PROBABILITIES

#include <map>

#include "fries/language.h"

#define RE_FZ "^[FZ]"

////////////////////////////////////////////////////////////////
/// Class probabilities sets lexical probabilities for
/// each PoS tag of each word in a sentence.
////////////////////////////////////////////////////////////////

class probabilities {
   private:
      /// Auxiliary regexps
      RegEx RE_PunctNum;

      /// Probability threshold for unknown words tags
      double ProbabilityThreshold;
      std::string Language;

      /// Weight factor to favor checher over guesser for unknown words
      double CheckerOverGuesser;
      /// Weight factor to favor spell chekings with exact phonetic match
      double ExactMatchBonus; 
      /// Mass threshold to reduce count of analysis for an alternative
      double AlternativeAnalysisMass;

      /// unigram probabilities
      std::map<std::string,double> single_tags;
      /// probabilities for usual ambiguity classes
      std::map<std::string,std::map<std::string,double> > class_tags;
      /// lexical probabilities for frequent words 
      std::map<std::string,std::map<std::string,double> > lexical_tags;
      /// list of tags and probabilities to assign to unknown words
      std::map<std::string,double> unk_tags;
      /// list of tag frequencies for unknown word suffixes
      std::map<std::string,std::map<std::string,double> > unk_suffs;
      /// unknown words suffix smoothing parameter;
      double theeta;
      /// length of longest suffix
      std::string::size_type long_suff;

      /// Smooth probabilities for the analysis of given word
      void smoothing(word &);
      /// Compute p(tag|suffix) using recursively shorter suffixes.
      double compute_probability(const std::string &, double, const std::string &);
      /// Guess possible tags, keeping some mass for previously assigned tags    
      double guesser(word &, double);

   public:
      /// Constructor
      probabilities(const std::string &, const std::string &, double);

      /// Assign probabilities to tags using default options
      void annotate(sentence &);
      /// Assign probabilities for each analysis of given word
      void annotate_word(word &);

};

#endif
