//-----------------------------------------------------------------------
//
// regex.hpp 1.0 Copyright (c) 2003 Peter Petersen (pp@on-time.de)
// Simple C++ wrapper for PCRE
//
// This source file is freeware. You may use it for any purpose without
// restriction except that the copyright notice as the top of this file as
// well as this paragraph may not be removed or altered.
//
//-----------------------------------------------------------------------
//
//  CHANGES made to regexp.hpp 1.0 to make it suitable for FreeLing:
//
//    original function:  char * Match(int)
//    converted to:       string Match(int)
//
//    original function:  RegEx (const char *, int)
//    converted to:       RegEx (const string&, int)
//
//    original function:  Search (const char *, int, int)
//    converted to:       Search (const string&, int, int)
//
//    Added:  Copy constructor: RegEx (const RegEx &)
//            assignation:      RegEx& operator=(const RegEx &)     
//            clone auxiliary:  void clone(const RegEx &)
//
/////////////////////////////////

#ifndef _REGEX_PCRE_H
#define _REGEX_PCRE_H

#include <string>
#include <cstring>

#ifndef _PCRE_H
#include "pcre.h"
#endif

//////////////////////////////////////////////////////////////////////
///
/// class RegEx, a simple and small API wrapper for PCRE.
///
/// RegEx::RegEx(string regex, int options = 0)
///
///    The constructor's first parameter is the regular expression the
///    created object shall implement. Optional parameter options can be
///    any combination of PCRE options accepted by pcre_compile(). If
///    compiling the regular expression fails, an error message string is
///    thrown as an exception.
///
/// RegEx::~RegEx()
///
///    The destructor frees all resources held by the RegEx object.
///
/// int RegEx::SubStrings(void) const
///
///    Method SubStrings() returns the number of substrings defined by
///    the regular expression. The match of the entire expression is also
///    considered a substring, so the return value will always be >= 1.
///
/// bool RegEx::Search(const char * subject, int len = -1, int options = 0)
///
///    Method Search() applies the regular expression to parameter subject.
///    Optional parameter len can be used to pass the subject's length to
///    Search(). If not specified (or less than 0), strlen() is used
///    internally to determine the length. Parameter options can contain
///    any combination of options PCRE_ANCHORED, PCRE_NOTBOL, PCRE_NOTEOL.
///    PCRE_NOTEMPTY. Search() returns true if a match is found.
///
/// bool RegEx::SearchAgain(int options = 0)
///
///    SearchAgain() again applies the regular expression to parameter
///    subject last passed to a successful call of Search(). It returns
///    true if a further match is found. Subsequent calls to SearchAgain()
///    will find all matches in subject. Example:
///
///       if (Pattern.Search(astring)) {
///          do {
///             printf("%s\n", Pattern.Match());
///          } while (Pattern.SearchAgain());
///       }
///
///    Parameter options is interpreted as for method Search().
///
/// string RegEx::Match(int i = 1)
///
///    Method Match() returns a pointer to the matched substring specified
///    with parameter i. Match() may only be called after a successful
///    call to Search() or SearchAgain() and applies to that last
///    Search()/SearchAgain() call. Parameter i must be less than
///    SubStrings(). Match(-1) returns the last searched subject.
///    Match(0) returns the match of the complete regular expression.
///    Match(1) returns $1, etc.
///
/// The bottom of this file contains an example using class RegEx. It's
/// the simplest version of grep I could come with. You can compile it by
/// defining REGEX_DEMO on the compiler command line.
///
/////////////////////////////////////////////////////////////////////

class RegEx
{
   public:
      /// store expression string, for tracing pruposes
      std::string expression; 

      /// Constructor
      RegEx(const std::string &regex, int options = 0)
      {
	const char * error;
	int          erroffset;

 	 expression=regex;
         re = pcre_compile(regex.c_str(), options, &error, &erroffset, NULL);
         if (re == NULL) 
	   throw error;
         pe = pcre_study(re, 0, &error);
         pcre_fullinfo(re, pe, PCRE_INFO_CAPTURECOUNT, &substrcount);
         substrcount++;
         ovector = new int[3*substrcount];
         matchlist = NULL;
      };

      /// Copy constructor
      RegEx (const RegEx &y)
      {
        clone(y);
      };

      /// Assignation
      RegEx& operator=(const RegEx &y)
      {
	if (this != &y) {
	  ClearMatchList();
	  delete[] ovector;
	  if (pe) pcre_free(pe);
	  pcre_free(re);
	  clone(y);
	}
	return (*this);
      };

      /// Destructor
      ~RegEx()
      {
         ClearMatchList();
         delete[] ovector;
         if (pe) pcre_free(pe);
         pcre_free(re);
      }

      /// Get number of substrings
      inline int SubStrings(void) const
      {
         return substrcount;
      }

      /// Search RE in string
      bool Search(const std::string &subject, int len = -1, int options = 0)
      {
         ClearMatchList();
         return pcre_exec(re, pe, lastsubject = subject.c_str(), slen = (len >= 0) ? len : strlen(subject.c_str()), 0, options, ovector, 3*substrcount) > 0;
      }

      /// Search RE in C-style string
      bool Search(const char * subject, int len = -1, int options = 0)
      {
	ClearMatchList();
	return pcre_exec(re, pe, lastsubject = subject, slen = (len >= 0) ? len : strlen(subject), 0, options, ovector, 3*substrcount) > 0;
      }

      ///  Search RE in string again
      bool SearchAgain(int options = 0)
      {
         ClearMatchList();
         return pcre_exec(re, pe, lastsubject, slen, ovector[1], options, ovector, 3*substrcount) > 0;
      }

      /// Get match for i-th substring in regexp
      std::string Match(int i = 1)
      {
         if (i < 0)
            return lastsubject;
         if (matchlist == NULL)
            pcre_get_substring_list(lastsubject, ovector, substrcount, &matchlist);

         std::string mch (matchlist[i], strlen(matchlist[i]));
         return mch;
      }
      
      
       /// Get match positions for i-th substring in regexp
      void MatchPositions(int i, int &ini, int &end)
      {
            if ((i>=0) and (i<3*substrcount)){
		ini=ovector[i*2];
	    	end=ovector[i*2+1];
	    }
	    else { ini=-1; end=-1;}
         
    
      }

   private:

      /// auxiliary for copy and assignation constructors
      void clone(const RegEx &y) {
	size_t size;
	
	if ( !y.re ) return;

	pcre_fullinfo(y.re, 0, PCRE_INFO_SIZE, &size);
	re = (pcre *) pcre_malloc(size);
	if ( !re ) throw "not enough memory";
	memcpy(re, y.re, size);

	pcre_fullinfo(y.re, y.pe, PCRE_INFO_STUDYSIZE, &size);
	if (size>0) {
	  pe = (pcre_extra *) pcre_malloc(size + sizeof(pcre_extra)); 
	  if ( !pe ) throw "not enough memory";
	  memcpy(pe, y.pe, size+sizeof(pcre_extra));
	  // study_data needs reset
	  void * study = (void *)((char *)pe + sizeof(pcre_extra));
	  pe->study_data = study;
	} 
	else
	  pe = NULL;

        expression = y.expression;
	substrcount = y.substrcount;
	ovector = new int[3*substrcount];
	matchlist = NULL; 
     }

      /// Clear match list
      inline void ClearMatchList(void)
      {
         if (matchlist)
            pcre_free_substring_list(matchlist),
            matchlist = NULL;
      }

      pcre * re;
      pcre_extra * pe;
      int substrcount;
      int * ovector;
      const char * lastsubject;
      int slen;
      const char * * matchlist;
};


#endif // _REGEX_H
