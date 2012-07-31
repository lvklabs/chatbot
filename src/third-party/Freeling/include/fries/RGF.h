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

//===========================================================
//=                                                         =
//=   This module is based on FEX project developed         =
//=   at U.Illinois, and distributed under GPL              =
//=   with the permission of its authors                    =
//=                                                         =
//=   For info about original version, visit                =
//=      http://l2r.cs.uiuc.edu/~cogcomp/                   =
//=   or contact                                            =
//=      University of Illinois at Urbana/Champaign         =
//=      Department of Computer Science                     =
//=      Dr. Dan Roth - Cognitive Computation Group         =
//=                                                         =
//===========================================================

#ifndef _RGF
#define _RGF

#include <string>
#include <vector>
#include <set>
#include "fries/language.h"
#include "fries/sensor.h"

struct RGF;

typedef enum {EXTRACT_LABEL, EXTRACT_CONJ, EXTRACT_DISJ, EXTRACT_COLOC,
                EXTRACT_SCOLOC, EXTRACT_LINK, EXTRACT_SENSOR, 
                EXTRACT_NOT, EXTRACT_CONJUNCT } ExtractMode;

typedef enum {TARGET_NULL = -2, TARGET_ALL = -1} TargetConstants;
const int RANGE_ALL = 1000000;

typedef std::vector<RGF> SubRGF;

struct RGF
{
public:
   // default constructor
   RGF();
   RGF(char* sensorName, char* p1, char* p2, char* p3, char* dpath);

   // constructor for sensors
   RGF(char* sensorName);

   // constructor for complex relations
   RGF( SubRGF& subR );

   // destructor for RGF
   ~RGF();

	// These are bogus prototypes (no implementation exists) because the
	// Visual C++ 5.0 STL implelmentation is broken.
	bool		      operator==( const RGF& rhs ) const;
	bool		      operator<=( const RGF& rhs ) const;
	bool		      operator<( const RGF& rhs ) const;
	bool		      operator>( const RGF& rhs ) const;
	bool		      operator>=( const RGF& rhs ) const;

   std::set<std::string>  Extract( const vector<const word*>& sent, int targIndex );
   // New Extract used for phrase case
   // Added by Scott Yih, 09/25/01
   std::set<std::string>  Extract( const vector<const word*>& sent, int targIndex, int targLength );
   // Another new Extract; used for ER:Relation case
   // Added by Scott Yih, 01/09/02
  //   std::set<std::string>  ExtractRelation ( vector<word>& sent, RelationInSentence& relSent, int posArg1, int posArg2 );

   int            TargetIndex() { return targetIndex; }
   void           Target(char* targ);                     // set target
   const char*    Target() const { return target; }       // return target
   void           IncludeLocation(bool val);
   void           LocationOffset(int val);
   int            LocationOffset() const { return locationOffset; }
   void           LeftOffset(int val) { leftOffset = val; }
   int            LeftOffset() const { return leftOffset; }
   void           RightOffset(int val);
   int            RightOffset() const { return rightOffset; }
   void           IncludeTarget(bool val);
   bool           IncludeTarget() const { return includeTarget; }
   void           IncludeMark(bool val);
   bool           IncludeMark() const { return includeMark; }
   void           Mode(ExtractMode mode);
   ExtractMode    Mode()                 { return extractMode; }
   void           Mask(char* val) { mask = val; }
   char*          Mask() { return mask; }
   void           Param(char* val);
   const char*    Param() {return optParam;}
   void           GenFeature(bool val) { genFeature = val; }
   void           Insert(RGF rel) { subRGFs.push_back(rel); }

   // Show internal state of RGF
	void		      Show();

protected:
  SubRGF             subRGFs;
  char*              optParam;
  bool               includeTarget;
  bool includeMark;
  int		          targetIndex;
  int		          leftOffset;
  int		          rightOffset;
  int		          locationOffset;
  ExtractMode	       extractMode;
  char* 		       target;
  char*              mask;
  bool               genFeature;
  Sensor             *pSensor;

  // Feature extraction functions
  std::set<std::string> Process( const vector<const word*>& sent,
			 int rec,
			 int targIndex,
			 int start,
			 int end);

};

#endif

