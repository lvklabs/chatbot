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

#ifndef _GRAMMAR
#define _GRAMMAR

#include <string>
#include <list>
#include <map>
#include <set>

#include "freeling/tokens.h"

////////////////////////////////////////////////////////////////
///   Class rule implements a rule of a grammar.
////////////////////////////////////////////////////////////////

class rule {
 protected:
  /// Head of the rule
  std::string head;
  /// Right part of the rule 
  std::list<std::string> right;
  /// Position of the rule Governor of 
  int gov;

 public:
  /// Constructors of the subclass rule 
  rule(const std::string &, const std::list<std::string> &, const int);
  rule(const rule & r);
  rule();
  rule & operator=(const rule&);

  /// set rule governor
  void set_governor(const int);
  /// get rule governor
  unsigned int get_governor(void) const;
  /// get rule head.
  std::string get_head() const;
  /// get right part of the rule.
  std::list<std::string> get_right() const;
};

////////////////////////////////////////////////////////////////
///   Class grammar implements a CFG, ready to be used 
/// from a chart parser.
////////////////////////////////////////////////////////////////

class grammar : public std::multimap<std::string,rule> {

 private:
  /// Non-terminal symbols in the grammar
  std::set<std::string> nonterminal;
  /// rules starting with a wildcarded token, indexed by first char in category.
  std::multimap<std::string,rule> wild;
  /// map to store files appearing in grammar rules
  std::multimap<std::string,std::string> filemap;
  /// symbol priorities to build the tree 
  std::map<std::string,int> prior;
  /// Non-terminal symbols that must not be seen in the tree
  std::set<std::string> hidden;
  /// Non-terminal symbols that must be flattened in final tree when recursive
  std::set<std::string> flat;
  /// Non-terminal symbols that must not be considered tree roots
  std::set<std::string> notop;
  /// Non-terminal symbols that are visible only when are at tree root.
  std::set<std::string> onlytop;
  /// start symbol
  std::string start;
  /// Create and store a new rule, indexed by 1st category in its right part.
  void new_rule(const std::string &, const std::list<std::string> &, bool, const int rgov);

 public:

  // no-governor mark
  static unsigned int NOGOV;
  // default governor (first element in rule)
  static unsigned int DEFGOV;

  /// Create a grammar loading it from a file
  grammar(const std::string &);

  // obtain the specificity of a terminal symbol
  int get_specificity(const std::string &) const;
  // obtain the priority of a non-terminal symbol
  int get_priority(const std::string &) const;
  /// obtain the start symbol of the grammar
  std::string get_start_symbol() const;
  /// Check whether a symbol must disappear of final tree
  bool is_hidden(const std::string &) const; 
  /// Check whether a symbol must be flattened when recursive
  bool is_flat(const std::string &) const;
  /// Check whether a symbol can not be used as a tree root
  bool is_notop(const std::string &) const;
  /// Check whether a symbol is hidden unless when at tree root
  bool is_onlytop(const std::string &) const;
  /// Check whether a symbol is terminal or not
  bool is_terminal(const std::string &) const;
  /// Get all rules with a right part beggining with the given category
  std::list<rule> get_rules_right(const std::string &) const;
  /// Get all rules with a right part beggining with a wilcarded category
  std::list<rule> get_rules_right_wildcard(const std::string &) const;
  /// search given string in filemap, and check whether it maps to the second
  bool in_filemap(const std::string &, const std::string &) const;
};

#endif





