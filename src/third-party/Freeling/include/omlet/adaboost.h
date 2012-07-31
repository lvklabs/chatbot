//////////////////////////////////////////////////////////////////
//
//    Omlet - Open Machine Learning Enhanced Toolkit
//
//    Copyright (C) 2006   TALP Research Center
//                         Universitat Politecnica de Catalunya
//
//    This file is part of the Omlet library
//
//    The Omlet library is free software; you can redistribute it 
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

//
// Author: Xavier Carreras
//

#ifndef _ADABOOST
#define _ADABOOST

#include "omlet/weakrule.h"
#include "omlet/dataset.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>


////////////////////////////////////////////////////////////////
///  Class AdaBoost implement a generic AB learner/classifier,
/// which may be based on any kind of weak rule.
////////////////////////////////////////////////////////////////

class adaboost : public std::list<weak_rule*> {
private:
  /// class parameters
  bool option_initialize_weights;

  /// type of used weak rules
  std::string wr_type;

  adaboost::const_iterator pcl_pointer; // partial classification pointer
  int         nrules;
  int         nlabels;   
  std::vector<std::string> labels;  //label names, loaded from model file
  std::string  label_others;

  /// output 
  std::ostream *out;

  /// auxiliar learning functions
  void initialize_weights(dataset &ds);
  void update_weights(weak_rule *wr, double Z, dataset &ds);
  void add_weak_rule(weak_rule *wr);

  /// copy constructor forbidden
  adaboost(const adaboost &old_bab); 

public:
  /// constructors, destructor and access methods
  adaboost(int nl, std::string t);
  adaboost(const std::string &file);
  int n_rules();

  int get_nlabels();
  std::string get_label(int lb);
  std::string default_class();

  /// classification methods. 
  /// Important: pred is an array of predictions, one for each label
  ///            the function *assigns* its predicion for each label.
  void classify(const example &i,  double pred[]);
  /// classification returning vector: useful for Java API
  std::vector<double> classify(const example &i);

  /// partial classification
  void pcl_ini_pointer();
  int  pcl_advance_pointer(int steps);
  /// Important: pred is an array of predictions, one for each label
  ///            the function *adds* its predicion for each label.
  void pcl_classify(const example &i, double *pred, int nrules);

  /// learning methods
  void learn(dataset &ds, int nrounds, bool init, wr_params *p);
  void learn(dataset &ds, int nrounds, bool init, wr_params *p, const std::string &outf);

  /// I/O methods
  void set_output(std::ostream *os);
  void read_from_stream(std::istream *in);
  void read_from_file(const std::string &f);

  void set_initialize_weights(bool b);
};



#endif 
