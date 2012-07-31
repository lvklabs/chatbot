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

#ifndef _MACO_OPTIONS
#define _MACO_OPTIONS

#include <string>

////////////////////////////////////////////////////////////////
///  Class maco_options implements a set of specific options
/// of the morphological analyzer. Other modules do not have
/// such a class because they deal with a reduced number of
/// options
////////////////////////////////////////////////////////////////

class maco_options {

 public:
    // Language analyzed
    std::string Lang;
    /// Morphological analyzer options
    bool AffixAnalysis,   MultiwordsDetection, 
         NumbersDetection, PunctuationDetection, 
         DatesDetection,   QuantitiesDetection, 
         DictionarySearch, ProbabilityAssignment,
	 OrthographicCorrection;
    int NERecognition;
    /// Morphological analyzer options
    std::string Decimal, Thousand;
    /// Morphological analyzer options
    std::string LocutionsFile, QuantitiesFile, AffixFile, 
           ProbabilityFile, DictionaryFile, 
           NPdataFile, PunctuationFile,
           CorrectorFile;

    double ProbabilityThreshold;

    /// constructor
    maco_options(const std::string &); 

    /// Option setting methods provided to ease perl interface generation. 
    /// Since option data members are public and can be accessed directly
    /// from C++, the following methods are not necessary, but may become
    /// convenient sometimes.
    void set_active_modules(bool,bool,bool,bool,bool,bool,bool,bool,int,bool);
    void set_nummerical_points(const std::string &,const std::string &);
    void set_data_files(const std::string &,const std::string &,const std::string &,const std::string &,
                        const std::string &,const std::string &,const std::string &, const std::string &);
    void set_threshold(double);
};

#endif

