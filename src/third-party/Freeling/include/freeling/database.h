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

#ifndef _DATABASE
#define _DATABASE

#include <string>
#include <map>

#ifdef USE_LIBDB
#include <db_cxx.h>  // header of BerkeleyDB C++ interface

// define apropriate open calls depending on Berkeley-DB version.
#if (DB_VERSION_MAJOR==4 && DB_VERSION_MINOR==0)
  #define OPEN(name,db,type,flags,mode) open(name,db,type,flags,mode)
#else
#if DB_VERSION_MAJOR>=4
  #define OPEN(name,db,type,flags,mode) open(NULL,name,db,type,flags,mode)
#endif
#endif

#endif


///////////////////////////////////////////////////////////////
///  Class to wrap a berkeley DB database and unify access.
///  All databases in Freeling use a string key to index string data.
///////////////////////////////////////////////////////////////

#ifdef USE_LIBDB
class database : Db {
#else
class database {
#endif
 
  private:
    /// remember if we are using a DB dictionary or a RAM one.
    bool usingDB;
    /// dictionary loaded into RAM (if no DB is used)
    std::map<std::string,std::string> dbmap;

  public:
    /// constructor
    database();
    /// destructor
    ~database();

    ///  Open a BerkeleyDB file and link it to the database object
    void open_database(const std::string &);
    ///  close the database.
    void close_database();
    ///  search for a string key in the DB, return associated string data.
    std::string access_database(const std::string &);
};

#endif
