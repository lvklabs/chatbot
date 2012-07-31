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

#ifndef _NP
#define _NP

#include <set>
#include <map>

#include "fries/language.h"
#include "freeling/automat.h"
#include "freeling/ner.h"
#include "regexp-pcre++.h"

#define RE_NA "^(NC|AQ)"
#define RE_DNP  "^[FWZ]"
#define RE_CLO  "^[DSC]"

////////////////////////////////////////////////////////////////
///  The class np implements a dummy proper noun recognizer.
////////////////////////////////////////////////////////////////

class np: public ner, public automat {
  
  private: 
    /// set of function words
    std::set<std::string> func;
    /// set of special punctuation tags
    std::set<std::string> punct;
    /// set of words to be considered possible NPs at sentence beggining
    std::set<std::string> names;
    /// set of words/tags to be ignored as NE parts, even if they are capitalized
    std::map<std::string,int> ignore_tags;
    std::map<std::string,int> ignore_words;
    /// sets of NE affixes
    std::set<std::string> prefixes;
    std::set<std::string> suffixes;

    /// it is a noun at the beggining of the sentence
    bool initialNoun;

    RegEx RE_NounAdj;
    RegEx RE_Closed;
    RegEx RE_DateNumPunct;

    int ComputeToken(int,sentence::iterator &, sentence &);
    void ResetActions();
    void StateActions(int, int, int, sentence::const_iterator);
    void SetMultiwordAnalysis(sentence::iterator, int);
    bool ValidMultiWord(const word &);
    sentence::iterator BuildMultiword(sentence &, sentence::iterator,sentence::iterator, int, bool &);
  public:
    /// Constructor
    np(const std::string &); 
    /// Specify that "annotate" must be inherited from "automat" and not from "ner"
    void annotate(sentence &);


};

#endif
