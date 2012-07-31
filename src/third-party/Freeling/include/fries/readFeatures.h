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


#ifndef _FEATURES
#define _FEATURES

#include "fries/RGF.h"
SubRGF* DoParse(const char *Scriptfile);

#include "fries/tok_features.h"
enum {T_COLOC, T_LABEL, T_NOT, T_CONJ, T_SCOLOC, T_DISJ, T_LINK};


#endif
