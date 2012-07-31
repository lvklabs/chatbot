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

#ifndef _SOUNDCHANGE
#define _SOUNDCHANGE

#include <string>
#include <map>
#include <vector>


////////////////////////////////////////////////////////////////
/// Auxiliar class to store a phonetic change rule
////////////////////////////////////////////////////////////////

class ph_rule {
 public:
   /// what to search and replace
   std::string from;
   /// what is the replacement
   std::string to;
   /// in which context
   std::string env;
};


////////////////////////////////////////////////////////////////
/// This class is a sound change applier. With it, you can construct
/// objects which will generate consistent sound changes.
////////////////////////////////////////////////////////////////

class soundChange {

 private:
   /// rules
   std::vector<ph_rule> rules;
   
   /// find in a text a string and replace it with another string under certain conditions
   std::string apply_rule(const std::string &, const ph_rule&) const;
   /// check that the conditions for the sound change are true
   bool check_cond(const std::string &, const std::string &, int, const std::string &) const;

 public:
   /// Constructor with vars and rules
   soundChange(const std::map<std::string,std::string> &, const std::vector<std::string> &);
   /// Destructor
   ~soundChange();
   
   /// Returns the sound changed for the word
   std::string change(const std::string &) const;
};


#endif
 
