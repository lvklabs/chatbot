
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

#ifndef _DATES_MOD
#define _DATES_MOD

#include <map>

#include "fries/language.h"
#include "freeling/automat.h"
#include "regexp-pcre++.h"

// Date/time regular expressions definitions

#define RE_ROMAN "^([IVXLCDM]+)$"

// Default:
#define RE_DATE_DF "^(?:(?:((?:[0-3])?(?:\\d))/)(?:((?:(?:[0-1])?(?:\\d)))/)(\\d{1,4}))$"
#define RE_TIME1_DF "^(?:((?:(?:[0-1])?(?:\\d))|(?:2(?:[01234])))(?:h|:)(?:((?:[0-5])?(?:\\d))(?:min|m)?)?)$"
#define RE_TIME2_DF "^(?:((?:[0-5])?(?:\\d))(?:min\\.|min|m\\.|m))$"

// Spanish:
#define RE_DATE_ES "^(?:(?:((?:[0-3])?(?:\\d))/)(?:((?:(?:[0-1])?(?:\\d))|enero|febrero|marzo|abril|mayo|junio|julio|agosto|septiembre|octubre|noviembre|diciembre|ene|feb|mar|abr|may|jun|jul|ago|sep|oct|nov|dic)/)(\\d{1,4}))$"
#define RE_TIME1_ES "^(?:((?:(?:[0-1])?(?:\\d))|(?:2(?:[01234])))(?:h|:)(?:((?:[0-5])?(?:\\d))(?:minutos|min|m)?)?)$"
#define RE_TIME2_ES "^(?:((?:[0-5])?(?:\\d))(?:minutos|min\\.|min|m\\.|m))$"

// Catalan:
#define RE_DATE_CA "^(?:(?:((?:[0-3])?(?:\\d))/)(?:((?:(?:[0-1])?(?:\\d))|gener|febrer|març|abril|maig|juny|juliol|agost|setembre|octubre|novembre|desembre|gen|feb|mar|abr|mai|jun|jul|ago|set|oct|nov|des)/)(\\d{1,4}))$"
#define RE_TIME1_CA "^(?:((?:(?:[0-1])?(?:\\d))|(?:2(?:[01234])))(?:h|:)(?:((?:[0-5])?(?:\\d))(?:minuts|min|m)?)?)$"
#define RE_TIME2_CA "^(?:((?:[0-5])?(?:\\d))(?:minuts|min\\.|min|m\\.|m))$"

// English:
#define RE_DATE_EN "^(?:(?:((?:[0-3])?(?:\\d))/)(?:((?:(?:[0-1])?(?:\\d))|january|february|march|april|may|june|july|august|september|october|november|december|jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)/)(\\d{1,4}))$"
#define RE_TIME1_EN "^(?:((?:(?:[0-1])?(?:\\d))|(?:2(?:[01234])))(?:h|:)(?:((?:[0-5])?(?:\\d))(?:minutes|min|m)?)?)$"
#define RE_TIME2_EN "^(?:((?:[0-5])?(?:\\d))(?:minutes|min\\.|min|m\\.|m))$"


////////////////////////////////////////////////////////////////
///  The abstract class dates_module generalizes temporal
/// expression recognizer for different languages.
////////////////////////////////////////////////////////////////

class dates_module: public automat {

   protected:
      /// translate month names to numbers
      std::map<std::string,int> nMes;
      /// translate weekday names to unified codes
      std::map<std::string,std::string> nDia;
      /// interpretation of the date-time expression
      std::string century,year,month,day,weekday,hour,minute,meridian;
      /// auxiliary for interpretation building
      int temp;
      int sign;  // for the catalan "un quart menys(-1)/i(1) cinc de sis" or 
                 //  fot English: a quarter to(-1)/past(1) five.
      int daytemp; // for special state Gbb in English
      bool inGbb; 
      /// translate particular strings to token codes
      std::map<std::string,int> tok;

      // required regular expressions objects
      RegEx RE_Date;
      RegEx RE_Time1;
      RegEx RE_Time2;
      RegEx RE_Roman;

   public:
      /// Constructor
      dates_module(const std::string &, const std::string &, const std::string &, const std::string &);
};


////////////////////////////////////////////////////////////////
///   The derived class dates_default implements a default date/time
///   recognizer (only simple patterns are recognized).
////////////////////////////////////////////////////////////////

class dates_default : public dates_module {

   private:
      int ComputeToken(int, sentence::iterator &, sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      dates_default();
};

////////////////////////////////////////////////////////////////
///   The derived class dates_es implements a Spanish date/time
///   recognizer.
////////////////////////////////////////////////////////////////

class dates_es : public dates_module {

   private:
      int ComputeToken(int, sentence::iterator &, sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      dates_es();
};


////////////////////////////////////////////////////////////////
///   The derived class dates_ca implements a Catalan date/time
///   recognizer.
////////////////////////////////////////////////////////////////

class dates_ca : public dates_module {

   private:
      int ComputeToken(int, sentence::iterator &, sentence &);
      void ResetActions();
      void StateActions(int, int, int, sentence::const_iterator);
      void SetMultiwordAnalysis(sentence::iterator, int);

   public:
      /// Constructor
      dates_ca();
};


////////////////////////////////////////////////////////////////
///   The derived class dates_en implements an English date/time
///   recognizer.
////////////////////////////////////////////////////////////////

class dates_en : public dates_module {

 private:
  int ComputeToken(int, sentence::iterator &, sentence &);
  void ResetActions();
  void StateActions(int, int, int, sentence::const_iterator);
  void SetMultiwordAnalysis(sentence::iterator, int);

  /// translate day numbers (first, second...) to a number (1, 2...)
  std::map<std::string,int> numDay;

 public:
  /// Constructor
  dates_en();
};


#endif

