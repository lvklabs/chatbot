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

/// Token definition for flex scanners for parser grammar files 
/// and constraint grammar files

// tokens used by both flex scanners
#define CATEGORY 1
#define FORM     2
#define LEMMA    3
#define COMMENT  4
#define ERROR    5

// tokens used by readgram flex scanner to read a chart parser grammar
#define ARROW    6
#define BAR      7
#define COMMA    8
#define DOT      9
#define FLAT     10
#define HIDDEN   11
#define NOTOP    12
#define ONLYTOP  13
#define PRIOR    14
#define START    15
#define FILENAME 16
#define HEAD     17

// tokens used by readCG flex scanner to read a relax constraint grammar
#define BARRIER   18
#define CPAR      19
#define FLOAT     20
#define NOT       21
#define OR        22
#define OPAR      23
#define OUT       24
#define POSITION  25
#define SEMICOLON 26
#define USER      27
#define SETS      28
#define CONSTRAINTS 29
#define IS         30
#define SETREF     31
#define SENSE      32

