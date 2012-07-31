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

#ifndef _EXAMPLE
#define _EXAMPLE

#include <map>
#include <vector>

class category {
 friend class example;
 protected:
  bool belongs;
  double weight;
  double prediction;   
};

////////////////////////////////////////////////////////////////
///  Class example is a vector of real-valued features, each 
/// identified by an integer label, plus info about to which
/// classes the example belongs to, and with prediction storage
/// functionalities.
////////////////////////////////////////////////////////////////

class example : public std::map<int,double> {

 private:
  /// highest integer label 
  int dimension;
  /// label management
  std::vector<category> labels;
  int nlabels;

 public:  
  /// constructors
  example(int nl);
  /// copy constructor
  example(const example &e);

  /// new example is f1*i1 + f2*i2
  example(double f1, const example& i1, double f2, const example& i2);

  /// compute norm of example as a vector
  double norm();

  /// update functions
  void add_feature(int l, double v = 1.0);

  /// consultors
  int get_nlabels() const;
  double get_feature_value(int label) const;
  int get_dimension() const;

  /// compute inner product with given feature vector
  double inner_product(const example &i2) const;
  /// add given feature vector with given weight
  void add_vector(double f, const example &i2);

  /// label and prediction management
  void set_label(int l, bool b, double w, double pr);
  void set_belongs(int l, bool b);
  bool belongs(int l) const;
  int sign(int l) const;
  void set_weight(int l, double w);
  double get_weight(int l) const;
  void set_prediction(int l, double pr);
  double get_prediction(int l);
};


#endif
