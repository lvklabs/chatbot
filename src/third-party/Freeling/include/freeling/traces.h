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

#ifndef _TRACES
#define _TRACES

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include "fries/language.h"
#include "fries/util.h"

/// possible values for MOD_TRACECODE
#define SPLIT_TRACE         0x00000001
#define TOKEN_TRACE         0x00000002
#define MACO_TRACE          0x00000004
#define OPTIONS_TRACE       0x00000008
#define NUMBERS_TRACE       0x00000010
#define DATES_TRACE         0x00000020
#define PUNCT_TRACE         0x00000040
#define DICT_TRACE          0x00000080
#define AFF_TRACE           0x00000100
#define LOCUT_TRACE         0x00000200
#define NP_TRACE            0x00000400
#define PROB_TRACE          0x00000800
#define QUANT_TRACE         0x00001000
#define NEC_TRACE           0x00002000
#define AUTOMAT_TRACE       0x00004000
#define TAGGER_TRACE        0x00008000
#define HMM_TRACE           0x00010000
#define RELAX_TRACE         0x00020000
#define RELAX_TAGGER_TRACE  0x00040000
#define CONST_GRAMMAR_TRACE 0x00080000
#define SENSES_TRACE        0x00100000
#define CHART_TRACE         0x00200000
#define GRAMMAR_TRACE       0x00400000
#define DEP_TRACE           0x00800000
#define COREF_TRACE         0x01000000
#define UTIL_TRACE          0x02000000
#define CORRECTOR_TRACE     0x04000000
#define PHONETICS_TRACE     0x08000000
#define DATABASE_TRACE      0x10000000


/// MOD_TRACECODE and MOD_TRACENAME are empty. The class 
/// using the trace is expected to set them
#undef MOD_TRACECODE
#undef MOD_TRACENAME

////////////////////////////////////////////////////////////////
///  Class traces implements trace and error handling utilities.
////////////////////////////////////////////////////////////////

class traces {
 public:
    // current trace level
    static int TraceLevel;
    // modules to trace
    static unsigned long TraceModule;

    static void error_crash(const std::string &, const std::string &, unsigned long);
    static void warning(const std::string &, const std::string &, unsigned long);
    static void trace(int,const std::string &, const std::string &, unsigned long);
    static void trace_word (int lv, const word &, const std::string &, unsigned long);
    static void trace_word_list(int,const std::list<word> &, const std::string &, unsigned long);
    static void trace_sentence(int,const sentence &, const std::string &, unsigned long);
    static void trace_sentence_list(int,const std::list<sentence> &, const std::string &, unsigned long);
};

/// static trace methods definition.  Inlined for efficiency
//---------------------------------
inline void traces::error_crash(const std::string &msg, const std::string &modname, unsigned long modcode) {
  std::cerr<<modname<<": "<<msg<<std::endl; 
  exit(1);
}

//---------------------------------
inline void traces::warning(const std::string &msg, const std::string &modname, unsigned long modcode) {
  std::cerr<<modname<<": "<<msg<<std::endl; 
}

//---------------------------------
inline void traces::trace(int lv, const std::string &msg, const std::string &modname, unsigned long modcode) {
  if (traces::TraceLevel>=lv && (traces::TraceModule&modcode))
     std::cerr<<modname<<": "<<msg<<std::endl;
}


//---------------------------------
inline void traces::trace_word (int lv, const word &wd, const std::string &modname, unsigned long modcode) {
word::const_iterator an; 
std::list<word>::iterator p; 
std::list<word> mw;

  std::string sel="";
  traces::trace(lv, "Word form ["+wd.get_form()+"] ("+util::int2string(wd.get_span_start())+","+util::int2string(wd.get_span_finish())+")",modname,modcode); 
  
  for (an=wd.unselected_begin(); an!=wd.unselected_end(); an++) {    
    traces::trace(lv, "   analysis: <"+an->get_lemma()+","+an->get_parole()+","+util::double2string(an->get_prob())+">",modname,modcode); 
  }
  for (an=wd.selected_begin(); an!=wd.selected_end(); an++) {    
    traces::trace(lv, "   analysis: <"+an->get_lemma()+","+an->get_parole()+","+util::double2string(an->get_prob())+"> **",modname,modcode); 
  }
  
  if (wd.is_multiword()) { 
    traces::trace(lv, "   is a multiword composed by:",modname,modcode); 
    mw = wd.get_words_mw(); 
    for (p=mw.begin(); p!=mw.end(); p++) 
      traces::trace(lv, "     ("+p->get_form()+")",modname,modcode); 
  }
}

//---------------------------------
inline void traces::trace_word_list(int lv, const std::list<word> &wl, const std::string &modname, unsigned long modcode) {
std::list<word>::const_iterator wd; 

  if (traces::TraceLevel>=lv && (traces::TraceModule&modcode)) {
    for (wd=wl.begin(); wd!=wl.end(); wd++) { 
      traces::trace_word(lv, *wd, modname, modcode);
    }
  }
}

//---------------------------------
inline void traces::trace_sentence(int lv, const sentence &s, const std::string &modname, unsigned long modcode) {
sentence::const_iterator wd; 
  
  if (traces::TraceLevel>=lv && (traces::TraceModule&modcode)) {
    traces::trace(lv, "BEGIN sentence",modname,modcode); 
 
    if (traces::TraceLevel>=lv && (traces::TraceModule&modcode)) {
      for (wd=s.begin(); wd!=s.end(); wd++) { 
	traces::trace_word(lv, *wd, modname, modcode);
      }
    }
    
    traces::trace(lv,"END sentence",modname,modcode); 
  }
}

//---------------------------------
inline void traces::trace_sentence_list(int lv, const std::list<sentence> &ls, const std::string &modname, unsigned long modcode) {
  std::list<sentence>::const_iterator s; 

  if (traces::TraceLevel>=lv && (traces::TraceModule&modcode)) {
    for(s=ls.begin(); s!=ls.end(); s++) traces::trace_sentence(lv,*s,modname,modcode);  
  }
}


/// Macros that must be used to put traces in the code.
/// They may be either defined or null, depending on -DVERBOSE compilation flag. 

#define ERROR_CRASH(msg) traces::error_crash(msg,MOD_TRACENAME,MOD_TRACECODE)
#define WARNING(msg) traces::warning(msg,MOD_TRACENAME,MOD_TRACECODE)

/// Tracing macros. Compile *without* -D VERBOSE to get a 
/// faster, non-traceable, exploitation version
#ifdef VERBOSE   
  /// ifdef VERBOSE --> TRACE macros exists
   #define TRACE(x,y) traces::trace(x,y,MOD_TRACENAME,MOD_TRACECODE)
   #define TRACE_WORD(x,y) traces::trace_word(x,y,MOD_TRACENAME,MOD_TRACECODE)
   #define TRACE_WORD_LIST(x,y) traces::trace_word_list(x,y,MOD_TRACENAME,MOD_TRACECODE)
   #define TRACE_SENTENCE(x,y) traces::trace_sentence(x,y,MOD_TRACENAME,MOD_TRACECODE)
   #define TRACE_SENTENCE_LIST(x,y) traces::trace_sentence_list(x,y,MOD_TRACENAME,MOD_TRACECODE)
#endif
#ifndef VERBOSE
  /// ifndef VERBOSE --> No messages displayed. Faster code.
  #define TRACE(x,y)
  #define TRACE_WORD(x,y)
  #define TRACE_WORD_LIST(x,y)
  #define TRACE_SENTENCE(x,y)
  #define TRACE_SENTENCE_LIST(x,y)
#endif




#endif
