/*
 * Phonetic Distance Scorer for the PHAST package
 * + See the features.ALL file for input description
 * + April 2006 pcomas@lsi.upc.edu
 *
 */
#ifndef _phd_h
#define _phd_h

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <math.h>
using namespace std;

#define MAX 128

template <typename T=int> class phd{

private:
  T csub, cexp, cvowel, cskip, cspace;
  T distance[MAX][MAX];
  set<char> svowels;      // set of vowel phonemes
  set<char> sconsonants;  // set of consonant phonemes
  int debug;

  inline T V(char a){ return svowels.find(a) != svowels.end() ? cvowel : 0; }
  inline T max(const T& a, const T& b){ return a > b ? a : b; }

public:

  phd(string const fname){

    debug = 0;
    ifstream is;
    string s;
    char c;
    T t;
    int i/*,fcount*/;
    map<const string, int> flist;   // set of the features' names with its index
    map<const string, T> fweight; // set of the features' saliences
    map<const string, T> values;  // set of the numerical values of the multivaluated features
    set<string> svfeatures;  // set of attributes for vowel comparison
    set<string> scfeatures;  // set of attributes for other comparisons
    csub = 0;
    cskip = 0;
    cexp = 0;
    cvowel = 0;

    /**************************************************************
     *
     * READ INPUT FILES, BUILD MATRIX OF FEATURES
     *
     **************************************************************/

    T features [MAX][MAX];

    try{
      is.open(fname.c_str());
    } catch(...){
      cerr << "Some problem opening " << fname << endl;
      cerr << "Exiting..." << endl;
      exit(-1);
    }

    //fcount = 0;

    while(!is.eof()){

      is >> s;

      if( s[0] == '#'){ 
	getline(is,s);

      } else if( s=="FON:") {
	is >> c;     // this is the phoneme
	//cerr << "FONEMA "<< c << endl;
	getline(is,s); 
	stringstream ss(s,stringstream::in);
	i = 0;
	while(ss>>s){
	  if(s=="+"){
	    features[(int)c][i] = 100;
	  }else if(s=="-"){
	    features[(int)c][i] = 0;
	  }else{  // is a multivaluated feature
	    features[(int)c][i] = values[s];
	  }
	  //cerr << "Posant " << features[c][i] << " a " << i << " (" << s << ")"<< endl;
	  i++;
	}

      } else if( s=="VALUE:") {
	is >> s >> t; // feature value is i
	values[s] = t;
	//cerr << "VALUE ADD: " << s << " <-- " << i << endl;

      } else if( s=="WEIGHT:") {
	is >> s >> t; // feature s weights i
	fweight[s] = t;

      } else if( s=="CONSTANT:") {
	is >> s >> t; // s takes value i
	if (s=="Cskip")   { cskip = t;}
	else if(s=="Csub"){ csub  = t;}
	else if(s=="Cexp"){  cexp = t;}
	else if(s=="Cvowel"){ cvowel = t;}
	else if(s=="Cspace"){ cspace = t;}
	else{ cerr << "UNEXPECTED CONSTANT DEFINITION" << s << endl; }

      } else if( s=="VOWELS:") {
	//create a list with the vocalic phonemes
	getline(is,s); 
	stringstream ss(s, stringstream::in);
	while( ss>>c ){  svowels.insert(c); }

      } else if( s=="CONSONANTS:") {
	//create a set with the consonantic phonemes
	getline(is,s); 
	stringstream ss(s, stringstream::in);
	while( ss>>c ){  sconsonants.insert(c); }

      } else if( s=="FEATURES:") {
	//create a list with the index inside the matrix for each feature
	getline(is,s); 
	stringstream ss(s, stringstream::in);
	i = 0;
	while( ss>>s ){ flist[s]=i; i++; }

      } else if( s=="FVOWELS:") {
	//create a set with 
	getline(is,s); 
	stringstream ss(s, stringstream::in);
	while( ss>>s ){ svfeatures.insert(s); }

      } else if( s=="FOTHER:") {
	//create a set with 
	getline(is,s); 
	stringstream ss(s, stringstream::in);
	while( ss>>s ){ scfeatures.insert(s); }

      } else {
	//skip
      }
      
    }
    
    is.close();


    /**************************************************************
     *
     * BUILD MATRIX OF DISTANCES
     *
     **************************************************************/
    /*
    */
    
    set<char>::iterator it1;
    set<char>::iterator it2;
    set<string>::iterator it3;
    T d;
    int f;

    for(int i=0;i<MAX;i++){
      for(int j=0;j<MAX;j++){
	distance[i][j]= i==j ? 0 : (T)8000;
      }
    }

    //Build vowels vs vowels

    for( it1 = svowels.begin(); it1!=svowels.end(); ++it1){
      for( it2 = svowels.begin(); it2!=it1; ++it2){
	//calculate distance between it1 and it2 using features in it3
	d=0;
	for(it3 = svfeatures.begin(); it3!=svfeatures.end(); ++it3){
	  f = flist[(*it3)];
	  d += abs( features[(int)(*it1)][(int)f] - features[(int)(*it2)][(int)f] ) * fweight[(*it3)];
	}
	distance[(int)(*it1)][(int)(*it2)] = d;
	distance[(int)(*it2)][(int)(*it1)] = d;
      }
    }


    //Build vowels vs consonants
    for( it2 = sconsonants.begin(); it2!=sconsonants.end(); ++it2){
      for( it1 = svowels.begin(); it1!=svowels.end(); ++it1){
	//calculate distance between it1 and it2 using features in it3
	d=0;
	for(it3 = scfeatures.begin(); it3!=scfeatures.end(); ++it3){
	  f = flist[(*it3)];
	  d += abs( features[(int)(*it1)][(int)f] - features[(int)(*it2)][(int)f] ) * fweight[(*it3)];
	}
	distance[(int)(*it1)][(int)(*it2)] = d;
	distance[(int)(*it2)][(int)(*it1)] = d;
      }
    }


    //Build consonants vs consonants
    for( it1 = sconsonants.begin(); it1!=sconsonants.end(); ++it1){
      for( it2 = sconsonants.begin(); it2!=it1; ++it2){
	//calculate distance between it1 and it2 using features in it3
	d=0;
	for(it3 = scfeatures.begin(); it3!=scfeatures.end(); ++it3){
	  f = flist[(*it3)];
	  d += abs( features[(int)(*it1)][(int)f] - features[(int)(*it2)][(int)f] ) * fweight[(*it3)];
	}
	distance[(int)(*it1)][(int)(*it2)] = d;
	distance[(int)(*it2)][(int)(*it1)] = d;
      }
    }

    if(debug>2){
      cerr << "\t";
      for( int i=85; i<MAX; i++ ){
	cerr << (char)i << "\t";
      }
      cerr << endl;

      for( int i=85; i<MAX; i++ ){
	cerr << (char)i << "\t";
	for( int j=85; j<MAX; j++ ){
	  cerr << distance[i][j] << "\t";
	}
	cerr << endl;
      }

    }


  } //constructor


  void show(ostream &o){

    set<char>::iterator it1;
    set<char>::iterator it2;
    set<string>::iterator it3;

    o << "Distances between phonemes" << endl << "==========================" << endl << endl;

    o << "Read values: cskip:" << cskip << ", csub:" << csub << ", cexp:" << cexp << ", cvowel:" << cvowel << endl;


    o << "\t";
    for( it1 = svowels.begin(); it1!=svowels.end(); ++it1) o << (*it1) << "\t";
    o << endl;

    for( it1 = svowels.begin(); it1!=svowels.end(); ++it1){
      o << (*it1) << "\t";
      for( it2 = svowels.begin(); it2!=it1; ++it2){
	o << distance[(int)(*it1)][(int)(*it2)] << "\t";
      }
      o << endl;
    }

    o << endl << "\t";
    for( it1 = svowels.begin(); it1!=svowels.end(); ++it1) o << (*it1) << "\t";
    o << endl;

    // vowels vs consonants
    for( it2 = sconsonants.begin(); it2!=sconsonants.end(); ++it2){
      o << (*it2) << "\t";
      for( it1 = svowels.begin(); it1!=svowels.end(); ++it1){
	o << distance[(int)(*it1)][(int)(*it2)] << "\t";
      }
      o << endl;
    }

    o << endl << "\t";
    for( it1 = sconsonants.begin(); it1!=sconsonants.end(); ++it1) o << (*it1) << "\t";
    o << endl;

    // consonants vs consonants
    for( it1 = sconsonants.begin(); it1!=sconsonants.end(); ++it1){
      o << (*it1) << "\t";
      for( it2 = sconsonants.begin(); it2!=it1; ++it2){
	o << distance[(int)(*it1)][(int)(*it2)] << "\t";
      }
      o << endl;
    }
  }


  T getCskip(){
    return cskip;
  }

  T dSkip(int c){
    return c==' ' || c=='_' ? cskip+cspace : cskip;
    //return cskip;
  }

  T dSub(int const a, int const b){
    if( ( (char)a==' ' || (char)a=='_' ) && ( (char)b==' ' || (char)b=='_' ) ){ return cspace; }
    return (char)a=='_' || (char)a==' ' || (char)b==' ' || (char)b=='_' ? -cspace/2 : csub - distance[a][b] - V(a) - V(b);
  }

  T dExp(int const a, int const b, int const c){
    return cexp - distance[a][b] - distance[a][c] - V(a) - max(V(b),V(c));
  }
  
};



#endif
