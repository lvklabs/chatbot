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

#ifndef _RELAX
#define _RELAX

#include <list>
#include <vector>

// just declaring to break ciclic references
class constraint;

////////////////////////////////////////////////////////////////
///
///  The class problem stores the structure of a problem,
///  namely, a vector with a position for each variable
///  to consider, and for each variable, a list of initial
///  weights for each possible label.
///   Variables and labels are unnamed, and sequentially 
///  numbered. The caller application must keep track of 
///  the meaning of each variable and label position.
///
////////////////////////////////////////////////////////////////

class problem : std::vector<std::list<double> > {
  friend class relax;
  public:
    /// Constructor
    problem(int);
    /// add a label (and its weight) to the given variable
    void add_label(int, double);
};


////////////////////////////////////////////////////////////////
///
///  The class label stores all information related to a 
/// variable label in the relaxation labelling algorithm.
///
////////////////////////////////////////////////////////////////

class label {
  friend class relax;
  protected:
    /// label weigth at current and next iterations
    double weight[2];
    /// list of constraints for the label
    std::list<constraint> constraints;

  public:
    /// Constructor
    label();

};

////////////////////////////////////////////////////////////////
///
///  The class constraint implements a constraint for the 
/// relaxation labelling algorithm.
///
////////////////////////////////////////////////////////////////

class constraint : public std::vector<std::vector<double*> > {
  private:
    double compatibility;

  public:
    /// Constructor
    constraint();

    /// set/get compatibility value
    void set_compatibility(double);
    double get_compatibility() const;
};

////////////////////////////////////////////////////////////////
///
///  The class relax implements a generic solver for consistent
/// labelling problems, using relaxation labelling algorithm.
///
////////////////////////////////////////////////////////////////

class relax {
   private:
     /// table with variable-labels in the CLP.
     std::vector<std::vector<label> > vars;
     /// which of both weight sets are we using and which are we computing
     int CURRENT, NEXT;
     /// Maximum number of iterations in case of not converging
     int MaxIter;
     /// Scale factor for label supports
     double ScaleFactor;
     /// epsilon value to decide whether or not an iteration has caused relevant weight changes
     double Epsilon;

     /// private methods
     double NormalizeSupport(double) const;
     bool there_are_changes() const;

   public:
       /// Constructor
       relax(int, double, double);

       /// Prepare for a new problem (i.e. free tables and alloc for the new problem)
       void reset(const problem &);
       /// add a new constraint to the problem
       void add_constraint (int, int, const std::list<std::list<std::pair<int,int> > > &, double);
       /// solve consistent labelling problem
       void solve();
       /// get best label(s) --hopefully only one-- for given variable
       std::list<int> best_label(int) const;
};


#endif
