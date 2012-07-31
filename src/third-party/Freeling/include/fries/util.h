//////////////////////////////////////////////////////////////////
//
//    Fries - Feature Retriever for Intensional Encoding of Sentences
//
//    Copyright (C) 2006   TALP Research Center
//                         Universitat Politecnica de Catalunya
//
//    This file is part of the Fries library
//
//    The Fries library is free software; you can redistribute it 
//    and/or modify it under the terms of the GNU General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, 5th Floor, Boston, MA 02110-1301 USA
//
//    contact: Lluis Padro (padro@lsi.upc.es)
//             TALP Research Center
//             despatx Omega.S112 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
////////////////////////////////////////////////////////////////

#ifndef _UTIL
#define _UTIL

#include <list>
#include <string>
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////
///  Class util implements some utilities for NLP analyzers:
///  "tolower" for latin alfabets, parole tags manipulation,
///  string2number and viceversa conversions, etc.
/////////////////////////////////////////////////////////////

class util {
 public:
   /// Lowercase a string, even with latin characters
   static std::string lowercase(const std::string &);
   /// uppercase an string, possibly with accents.
   static std::string uppercase(const std::string &);
   /// find out whether a string is an integer number
   static bool isanumber(const std::string &);
   /// find out whether a char is alphanumeric, even latin caracters 
   static bool isalphanum(const char);
   /// find out whether a string contains a lowercase char, even latin caracters 
   static bool has_lowercase(const std::string &);
   /// find out whether a char is uppercase, even latin caracters 
   static bool isuppercase(const char);
   /// Find out whether a string is all capitalized
   static bool all_caps(const std::string &);
   /// filename management
   static std::string absolute(const std::string &, const std::string &);
   /// eliminate from a string a list of chars
   static std::string eliminateChars(const std::string &, const std::string &);
   /// the split from perl in c++ version
   static std::vector<std::string> split(const std::string &, const std::string &);
   /// string handling
   static void find_and_replace(std::string &, const std::string &, const std::string &);
   /// conversion utilities
   static int string2int(const std::string &);
   static std::string int2string(const int);
   static double string2double(const std::string &);
   static std::string double2string(const double);
   static long double string2longdouble(const std::string &);
   static std::string longdouble2string(const long double);
   static std::string vector2string(const std::vector<std::string> &, const std::string &);
   static std::string list2string(const std::list<std::string> &, const std::string &);
   static std::string pairlist2string(const std::list<std::pair<std::string, double> > &, const std::string &, const std::string &);
   static std::list<std::string> string2list(const std::string &, const std::string &);
   static std::vector<std::string> string2vector(const std::string &, const std::string &);
   static std::set<std::string> string2set(const std::string &, const std::string &);
   static std::string set2string(const std::set<std::string> &, const std::string &);
};

#endif
