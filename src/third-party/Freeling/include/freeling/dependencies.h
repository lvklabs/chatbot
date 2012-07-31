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


#ifndef _DEP_TXALA
#define _DEP_TXALA

#include "fries/language.h"
#include "freeling/dep_rules.h"
#include "freeling/semdb.h"
#include "freeling/dependency_parser.h"

#include <string>
#include <map>
#include <set>
#include <vector>


////////////////////////////////////////////////////////////////
///
///  The class completer implements a parse tree completer,
/// which given a partial parse tree (chunker output), completes
/// the full parse according to some grammar rules.
///
////////////////////////////////////////////////////////////////

class completer {
  private:
    /// set of rules, indexed by labels of nodes
    std::map<std::pair<std::string,std::string>,std::list<completerRule> > chgram;
    /// set of active flags, which control applicability of rules
    std::set<std::string> active_flags;
    /// retrieve rule from grammar
    completerRule find_grammar_rule(const std::vector<parse_tree *> &, const size_t);
    /// apply a completion rule
    parse_tree * applyRule(const completerRule &, parse_tree *, parse_tree *);
    /// check if the extra lemma/form/class conditions are satisfied
    bool matching_condition(parse_tree::iterator, const std::list<std::string> &, RegEx &) const;
    /// check if the current context matches the given rule
    bool matching_context(const std::vector<parse_tree *> &, const size_t, const completerRule &) const;
    /// check if the operation is executable (for last_left/last_right cases)
    bool matching_operation(const std::vector<parse_tree *> &, const size_t, completerRule &) const;
    /// check left or right context
    bool match_side(const int, const std::vector<parse_tree *> &, const size_t, const std::vector<std::string> &, const size_t) const;
    /// Separate extra lemma/form/class conditions from the chunk label
    void extract_conds(std::string &, std::list<std::string> &, RegEx &) const;
    /// check if a chunk matches the given pattern "label<lemma>" or the like
    bool match_pattern(parse_tree::iterator, const std::string &) const;
    /// Find out if currently active flags enable the given rule
    bool enabled_rule(const completerRule &) const;

  public:  
    /// Constructor. Load a tree-completion grammar 
    completer(const std::string &);
    /// find best completions for given parse tree
    parse_tree complete(parse_tree &, const std::string &);

};


////////////////////////////////////////////////////////////////////////
///
/// depLabeler is class to set labels into a dependency tree
///
///////////////////////////////////////////////////////////////////////

class depLabeler {

  private:
    // set of rules
    std::map<std::string, std::list<ruleLabeler> > rules;
    // "unique" labels
    std::set<std::string> unique;
    // semantic database to check for semantic conditions in rules
    semanticDB * semdb;
    // parse a condition and create checkers.
    rule_expression* build_expression(const std::string &);

  public:
    /// Constructor. create dependency parser
    depLabeler(const std::string &);
    /// Destructor
    ~depLabeler();
    /// Label nodes in a dependency tree. (Initial call)
    void label(dep_tree*);
    /// Label nodes in a dependency tree. (recursive)
    void label(dep_tree*, dep_tree::iterator);
};



///////////////////////////////////////////////////////////////////////
///
/// dependencyMaker is a class for obtaining a dependency tree from chunks.
///  this implementation uses two subclasses:
/// completer: to complete the chunk analysis in a full parse tree
/// depLabeler: to set the labels once the class has build a dependency tree
///
///////////////////////////////////////////////////////////////////////

class dep_txala : public dependency_parser {

 private:
   /// tree completer  
   completer comp; 
   /// dependency labeler
   depLabeler labeler;
   // Root symbol used by the chunk parser when the tree is not complete.
   std::string start;
   /// compute dependency tree
   dep_tree* dependencies(parse_tree::iterator, parse_tree::iterator);

 public:   
   /// constructor
   dep_txala(const std::string &, const std::string &);
   /// Enrich all sentences in given list with a depenceny tree.
   void analyze(std::list<sentence> &);
   /// Enrich all sentences in given list, return a copy.
   std::list<sentence> analyze(const std::list<sentence> &);
};

#endif

