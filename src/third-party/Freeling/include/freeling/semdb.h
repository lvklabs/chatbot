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

#ifndef _SEMDB
#define _SEMDB


#include <string>
#include <list>

#include "freeling/database.h"
#include "fries/language.h"


////////////////////////////////////////////////////////////////
/// Class sense_info stores several semantic info about a sense
////////////////////////////////////////////////////////////////

class sense_info {
 public:
  /// sense code
  std::string sense;
  /// Part-of-speech
  std::string pos;
  /// hyperonyms
  std::list<std::string> parents;
  /// WN semantic file code
  std::string semfile;
  /// list of synonyms (words in the synset)
  std::list<std::string> words;
  /// list of EWN top ontology properties
  std::list<std::string> tonto;

  /// constructor
  sense_info(const std::string &,const std::string &,const std::string &);
  /// useful for java API
  std::string get_parents_string() const;
};

////////////////////////////////////////////////////////////////
/// Class semanticDB implements a semantic DB interface
////////////////////////////////////////////////////////////////

class semanticDB {
   private:
      /// C++ Interface to BerkeleyDB C API
      database sensesdb;
      database wndb;

   public:
      /// Constructor
      semanticDB(const std::string &, const std::string &); 
      /// Destructor
      ~semanticDB();
 
      /// get list of words for a sense+pos
      std::list<std::string> get_sense_words(const std::string &, const std::string &);
      /// get list of senses for a lemma+pos
      std::list<std::string> get_word_senses(const std::string &, const std::string &);
      /// get sense info for a sensecode+pos
      sense_info get_sense_info(const std::string &, const std::string &);
};

#endif

