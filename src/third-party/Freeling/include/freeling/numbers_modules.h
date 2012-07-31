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


#ifndef _NUMBERS_MOD
#define _NUMBERS_MOD

#include <map>

#include "fries/language.h"
#include "freeling/automat.h"
#include "regexp-pcre++.h"

#define RE_NUM "^([0-9]+\\"+MACO_Thousand+")*[0-9]+(\\"+MACO_Decimal+")?[0-9]*$"
#define RE_CODE "^.*[0-9].*$"

// Auxiliary, kind of code
#define CODE 1
#define ORD 2


////////////////////////////////////////////////////////////////
///  The abstract class numbers_module generalizes nummeric
/// expression recognizer for different languages.
////////////////////////////////////////////////////////////////

class numbers_module : public automat {

   protected:
      // configuration options
      std::string MACO_Decimal, MACO_Thousand;

      /// to map words into numerical values
      std::map<std::string,float> value;
      /// to map words into token codes 
      std::map<std::string,int> tok;
      /// to map value of power words (billion, million)
      std::map<int,long double> power;

      /// partial value of partially build number expression
      long double bilion,milion,units;
      int block;
      int iscode;
      RegEx RE_number;
      RegEx RE_code;
 
   public:
      /// Constructor
      numbers_module(const std::string &, const std::string &);
};

////////////////////////////////////////////////////////////////
///   The derived class numbers_default implements a default 
///   number recognizer (only numbers in digits are recognized).
////////////////////////////////////////////////////////////////

class numbers_default : public numbers_module {

   private: 
      int ComputeToken(int,sentence::iterator&,sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      numbers_default(const std::string &, const std::string &);
};


////////////////////////////////////////////////////////////////
///   The derived class numbers_es implements a Spanish
///   number recognizer.
////////////////////////////////////////////////////////////////

class numbers_es : public numbers_module {

   private:
      int ComputeToken(int,sentence::iterator&, sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      numbers_es(const std::string &, const std::string &);
};


////////////////////////////////////////////////////////////////
///   The derived class numbers_ca implements a Catalan
///   number recognizer.
////////////////////////////////////////////////////////////////

class numbers_ca : public numbers_module {

   private:
      int ComputeToken(int,sentence::iterator&,sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      numbers_ca(const std::string &, const std::string &);
};

////////////////////////////////////////////////////////////////
///   The derived class numbers_ca implements a Galician
///   number recognizer.
////////////////////////////////////////////////////////////////

class numbers_gl : public numbers_module {

   private:
      int ComputeToken(int,sentence::iterator&,sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      numbers_gl(const std::string &, const std::string &);
};


////////////////////////////////////////////////////////////////
///   The derived class numbers_it implements a Italian
///   number recognizer.
////////////////////////////////////////////////////////////////

class numbers_it : public numbers_module {

   private:
      int  ComputeToken(int,sentence::iterator&, sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

      // !! unify process with other languages !! 
      long double hundreds;	//this is additional.
      long double thousands;	//this is additional.
      long double floatUnits;   // "e tre quarto". This variable will count how
                                // many "halfs", "quartrs" we have

   public:
      /// Constructor
      numbers_it(const std::string &, const std::string &);
};


////////////////////////////////////////////////////////////////
///   The derived class numbers_en implements an English
///   number recognizer.
////////////////////////////////////////////////////////////////

class numbers_en : public numbers_module {

   private: 
      int ComputeToken(int,sentence::iterator&, sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      numbers_en(const std::string &, const std::string &);
};


#endif

