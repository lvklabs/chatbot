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

#ifndef _LANGUAGE
#define _LANGUAGE

#include <string>
#include <list>
#include <vector>
#include <map>

#include "fries/tree.h"
#include "regexp-pcre++.h"

class word; // predeclaration

////////////////////////////////////////////////////////////////
///   Class analysis stores a possible reading (lemma, PoS, probability, distance)
///   for a word.
////////////////////////////////////////////////////////////////

class analysis {

   private:
      /// lemma
      std::string lemma;
      /// PoS tag
      std::string parole;
      /// probability of that lemma-tag given the word
      double prob;
      /// distance from a added analysis from corrector to the original word
      double distance;
      ///  list of possible senses for that analysis, along with their rank as attributed by the ukb PPR algorithm
      std::list<std::pair<std::string,double> > senses;
      /// information to retokenize the word after tagging if this analysis is selected
      std::list<word> retok;

   public:
      /// user-managed data, we just store it.
      std::vector<std::string> user;

      /// constructor
      analysis();
      /// constructor
      analysis(const std::string &, const std::string &);
      /// assignment
      analysis& operator=(const analysis&);

      void set_lemma(const std::string &);
      void set_parole(const std::string &);
      void set_prob(double);
      void set_distance(double);
      void set_retokenizable(const std::list<word> &);

      bool has_prob(void) const;
      bool has_distance(void) const;
      std::string get_lemma(void) const;
      std::string get_parole(void) const;
      std::string get_short_parole(const std::string &) const;
      double get_prob(void) const;
      double get_distance(void) const;
      bool is_retokenizable(void) const;
      std::list<word> get_retokenizable(void) const;

      std::list<std::pair<std::string,double> > get_senses(void) const;
      void set_senses(const std::list<std::pair<std::string,double> > &);
      // useful for java API
      std::string get_senses_string(void) const;

      /// Comparison to sort analysis by *decreasing* probability
      int operator<(const analysis &) const;
};


////////////////////////////////////////////////////////////////
///   Class word stores all info related to a word:
///  form, list of analysis, list of tokens (if multiword).
////////////////////////////////////////////////////////////////

class word : public std::list<analysis> {
   private:
     /// lexical form
     std::string form;
     /// first selected analysis (if any)
     word::iterator selected;
     /// empty list if not a multiword
     std::list<word> multiword;
     /// alternative words (with analysis) provided by spell checker
     std::list<std::pair<word,double> > alternatives;
     /// token span
     unsigned long start, finish;
     /// word form found in dictionary
     bool in_dict;
     /// word morphological shouldn't be further modified
     bool locked;
     /// clone word (used by assignment/copy constructors)
     void clone(const word &);

   public:
      /// user-managed data, we just store it.
      std::vector<std::string> user;

      /// constructor
      word();
      /// constructor
      word(const std::string &);
      /// constructor
      word(const std::string &, const std::list<word> &);
      /// constructor
      word(const std::string &, const std::list<analysis> &, const std::list<word> &);
      /// Copy constructor
      word(const word &);
      /// assignment
      word& operator=(const word&);

      /// copy analysis from another word
      void copy_analysis(const word &);
      /// Get the number of selected analysis
      int get_n_selected(void) const;
      /// get the number of unselected analysis
      int get_n_unselected(void) const;
      /// true iff the word is a multiword compound
      bool is_multiword(void) const;
      /// get number of words in compound
      int get_n_words_mw(void) const;
      /// get word objects that compound the multiword
      std::list<word> get_words_mw(void) const;
      /// get word form
      std::string get_form(void) const;
      /// Get an iterator to the first selected analysis
      word::iterator selected_begin(void);
      /// Get an iterator to the first selected analysis
      word::const_iterator selected_begin(void) const;
      /// Get an iterator to the end of selected analysis list
      word::iterator selected_end(void);
      /// Get an iterator to the end of selected analysis list
      word::const_iterator selected_end(void) const;
      /// Get an iterator to the first unselected analysis
      word::iterator unselected_begin(void);
      /// Get an iterator to the first unselected analysis
      word::const_iterator unselected_begin(void) const;
      /// Get an iterator to the end of unselected analysis list
      word::iterator unselected_end(void);
      /// Get an iterator to the end of unselected analysis list
      word::const_iterator unselected_end(void) const;
      /// get lemma for the selected analysis in list
      std::string get_lemma(void) const;
      /// get parole for the selected analysis
      std::string get_parole(void) const;
      /// get parole (short version) for the selected analysis
      std::string get_short_parole(const std::string &) const;

      /// get sense list for the selected analysis
      std::list<std::pair<std::string,double> > get_senses(void) const;
      // useful for java API
      std::string get_senses_string(void) const;
      /// set sense list for the selected analysis
      void set_senses(const std::list<std::pair<std::string,double> > &);

      /// get token span.
      unsigned long get_span_start(void) const;
      unsigned long get_span_finish(void) const;

      /// get in_dict
      bool found_in_dict(void) const;
      /// set in_dict
      void set_found_in_dict(bool);
      /// check if there is any retokenizable analysis
      bool has_retokenizable(void) const;
      /// mark word as having definitive analysis
      void lock_analysis();
      /// check if word is marked as having definitive analysis
      bool is_locked(void) const;

      /// add an alternative to the alternatives list
      void add_alternative(const word &, double);
      /// replace alternatives list with list given
      void set_alternatives(const std::list<std::pair<word,double> > &);
      /// find out if the speller checked alternatives
      bool has_alternatives() const;
      /// get alternatives list
      std::list<std::pair<word,double> > get_alternatives() const;
      /// get alternatives begin iterator
      std::list<std::pair<word,double> >::iterator alternatives_begin();
      /// get alternatives end iterator
      std::list<std::pair<word,double> >::iterator alternatives_end();

      /// add one analysis to current analysis list  (no duplicate check!)
      void add_analysis(const analysis &);
      /// set analysis list to one single analysis, overwriting current values
      void set_analysis(const analysis &);
      /// set analysis list, overwriting current values
      void set_analysis(const std::list<analysis> &);
      /// set word form
      void set_form(const std::string &);
      /// set token span
      void set_span(unsigned long, unsigned long);

      /// look for an analysis with a parole matching given regexp
      bool find_tag_match(RegEx &);

      /// get number of analysis in current list
      int get_n_analysis(void) const;
      /// empty the list of selected analysis
      void unselect_all_analysis();
      /// mark all analysisi as selected
      void select_all_analysis();
      /// add the given analysis to selected list.
      void select_analysis(word::iterator);
      /// remove the given analysis from selected list.
      void unselect_analysis(word::iterator);
      /// get list of analysis (useful for perl API)
      std::list<analysis> get_analysis(void) const;
      /// get begin iterator to analysis list (useful for perl/java API)
      word::iterator analysis_begin(void);
      word::const_iterator analysis_begin(void) const;
      /// get end iterator to analysis list (useful for perl/java API)
      word::iterator analysis_end(void);
      word::const_iterator analysis_end(void) const;
};



////////////////////////////////////////////////////////////////
///   Class node stores nodes of a parse_tree
///  Each node in the tree is either a label (intermediate node)
///  or a word (leaf node)
////////////////////////////////////////////////////////////////

class node {
  protected:
    /// node identifier
    std::string nodeid;
    /// is the node the head of the rule?
    bool head;
    /// is the node the root of a chunk? which?
    int chunk;
    /// node label
    std::string label;
    /// sentence word related to the node (if leaf)
    word * w;


  public:
    /// constructors
    node();
    node(const std::string &);
    /// get node identifier
    std::string get_node_id() const;
    /// set node identifier
    void set_node_id(const std::string &);
    /// get node label
    std::string get_label(void) const;
    /// get node word
    word get_word(void) const;
    /// set node label
    void set_label(const std::string &);
    /// set node word
    void set_word(word &);
    /// find out whether node is a head
    bool is_head(void) const;
    /// set whether node is a head
    void set_head(const bool);
    /// find out whether node is a chunk
    bool is_chunk(void) const;
    /// set position of the chunk in the sentence
    void set_chunk(const int);
    /// get position of the chunk in the sentence
    int  get_chunk_ord(void) const;
};

////////////////////////////////////////////////////////////////
///   Class parse tree is used to store the results of parsing
////////////////////////////////////////////////////////////////

class parse_tree : public tree<node> {
 private:
  std::map<std::string,parse_tree::iterator> node_index;

 public:
   parse_tree();
   parse_tree(parse_tree::iterator p);
   parse_tree(const node &);

   /// assign an id to each node and build index
   void build_node_index();
   /// rebuild index maintaining node id's
   void rebuild_node_index();
   /// access the node with given id
   parse_tree::iterator get_node_by_id(std::string) const;

   static int nsentence;
};


////////////////////////////////////////////////////////////////
/// class denode stores nodes of a dependency tree and
///  parse tree <-> deptree relations
////////////////////////////////////////////////////////////////

class depnode : public node {

  private:
    /// corresponding node of the parse tree in the same sentence.
    parse_tree::iterator link;

  public:
    depnode();
    depnode(const std::string &);
    depnode(const node &);
    void set_link(const parse_tree::iterator);
    parse_tree::iterator get_link(void);
    /// get link ref (useful for Java API)
    tree<node>& get_link_ref(void);
};



////////////////////////////////////////////////////////////////
/// class dep_tree stores a dependency tree
////////////////////////////////////////////////////////////////

class dep_tree :  public tree<depnode> {
  public:
    dep_tree();
    dep_tree(const depnode &);
};


////////////////////////////////////////////////////////////////
///   Class sentence is just a list of words that someone
/// (the splitter) has validated it as a complete sentence.
/// It may include a parse tree.
////////////////////////////////////////////////////////////////

class sentence : public std::list<word> {
 private:
  parse_tree pt;
  dep_tree dt;
  /// clone sentence (used by assignment/copy constructors)
  void clone(const sentence &);

 public:
  sentence();
  sentence(const std::list<word>&);
  /// Copy constructor
  sentence(const sentence &);
  /// assignment
  sentence& operator=(const sentence&);

  void clear();
  void set_parse_tree(const parse_tree &);
  parse_tree & get_parse_tree(void);
  bool is_parsed() const;

  dep_tree & get_dep_tree();
  void set_dep_tree(const dep_tree &);
  bool is_dep_parsed() const;

  /// get word list (useful for perl API)
  std::vector<word> get_words() const;
  /// get iterators to word list (useful for perl/java API)
  sentence::iterator words_begin(void);
  sentence::const_iterator words_begin(void) const;
  sentence::iterator words_end(void);
  sentence::const_iterator words_end(void) const;
};

////////////////////////////////////////////////////////////////
///   Class paragraph is just a list of sentences that someone
///  has validated it as a paragraph.
////////////////////////////////////////////////////////////////

class paragraph : public std::list<sentence> {};

////////////////////////////////////////////////////////////////
///   Class document is a list of paragraphs. It may have additional
///  information (such as title)
////////////////////////////////////////////////////////////////

class document : public std::list<paragraph> {

  private:
    paragraph title;
    std::multimap<int,std::string> group2node;
    std::map<std::string,int> node2group;
    int last_group;

  public:
    document();
    /// Adds two nodes in a coreference group
    void add_positive(std::string node1, int group1);
    /// Adds two nodes in a coreference group
    void add_positive(std::string,std::string);
    /// Gets the id of the coreference group of the node
    int get_coref_group(std::string) const;
    /// Gets all the nodes in a coreference group id
    std::list<std::string> get_coref_nodes(int) const;
    /// Returns if two nodes are in the same coreference group
    bool is_coref(std::string,std::string) const;
};


#endif

