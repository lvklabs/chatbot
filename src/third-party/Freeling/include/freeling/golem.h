/*
 * The Golem is an automaton for recognize at a time all the substrings of fixed length of a given string.
 * It stores a matrix of width 128, it do not take input outside of range 32..126 of chars
 * + April 2006 - pcomas@lsi.upc.edu
 *
 *   "These are not the droids you're looking for"
 */

#ifndef _golem_h

#define _golem_h
#include <string.h>
#include <cstring>
#include <math.h>
#include <iostream>
#include <stack>
#include <vector>
#include <set>
#include <map>

#define WIDTH 97   // Width of the DFA table (see below)
#define DESP  30   // Number of shiftes positions of each character in the DFA table (see below)

using namespace std;



class golem{

  private:

  /* NFA */
  int** nfa;
  int* t0;
  int st;
  int nst;            // Number of different states is 1 + t*(n-t+1)
  int bst;            // Number of "base" states for each substring
  set<int> nfa_final; // Set of final states for the NFA
  /* DFA */
  int* dfa;
  int nstates;

  /* OTHERS */
  char* query;
  int t;
  int debug;
  int status;
  int n;

  struct state {
    int length; //number of nstate
    int dstate;
    char letter;
    int* nstate;
  };

  

  /* Creates a NFA automaton for recognizing substrings of length t in _query */
  void createNFA(){

    bst = n-t+1;       // Number of "base" states for each substring
    nst = 1 + t*bst;   // Number of different states is 1 + t*(n-t+1)
    int nt  = 2+t;     // Number of transitions/state (at much) = 1+1+n
    st = bst+1;        // state counter

    if(debug){
      cerr << "n: " << n << endl;
      cerr << "bst: " << bst << endl;
      cerr << "nst: " << nst << endl;
      cerr << "nt: " << nt << endl;
      cerr << "st: " << st << endl;
    }

    //Create common transitions for each state
    t0 = new int[bst*2];
    for(int i=0;i<bst;i++){
      t0[2*i] = (int)query[i];
      t0[2*i+1] = i+1;
    }

    //Create the base nfa array
    nfa = new int*[nst];

    //Create the 0th state
    nfa[0] = new int[4];
    nfa[0][0] = '?';
    nfa[0][1] = 0;
    nfa[0][2] = '?';
    nfa[0][3] = 0;

    //create the first n-t+1 states
    for(int i=1; i<=bst; i++){

      nfa[i] = new int[4];
      // Transition to the next letter in the substring
      if(t==1){
	nfa[i][0] = '?';
	nfa[i][1] = 0;
	nfa_final.insert(i);
      }else{
	nfa[i][0] = query[i];
	nfa[i][1] = st;
      }
      nfa[i][2] = '?';
      nfa[i][3] = 0;

      if(st<nst){
	//Create the rest of the states after st
	//st = create_chain( nfa, st, i+1, nt, t, t0);
	for(int j=i+1; j<i+t-1; j++,st++){
	  //Create new state for ith letter
	  nfa[st]    = new int[4];
	  nfa[st][0] = query[j];
	  nfa[st][1] =  st+1;
	  nfa[st][2] = '?';
	  nfa[st][3] = 0;
	}

	//Create the last letter
	nfa[st] = new int[4];
	nfa[st][0] = '?';
	nfa[st][1] = 0;
	nfa[st][2] = '?';
	nfa[st][3] = 0;
	nfa_final.insert(st);
      }
      st++;

    }

    if(debug){
      cerr << endl << "THE GOLEM" << endl 
	   << "==========" << endl 
	   <<  "The commonest:" << endl 
	   << "\t";
      for(int j=0;j<bst;j++){
	cerr << (char)t0[2*j] <<"-->" << t0[2*j+1] << ",  ";
      }
      cerr << endl;
      for(int i=0;i<nst;i++){
	cerr << "state[" << i <<"]" << endl << "\t";
	for(int j=0;j<2;j++){
	  cerr << (char)nfa[i][j*2] <<"-->" << nfa[i][j*2+1] << ",  ";
	}
	cerr << endl;
      }
    }

  }



  /* Determinizes the former NFA automaton */
  void createDFA(){

    /*
     * nfa  points to a NFA to determinize
     *      each state in nfa has two transitions
     * t0   is the common set of transitions to all states in nfa
     */

    stack<state*> p;         // An stack of nfa states
    map<char,set<int> > d0; // The set of transitions from the t0 state in nfa
    int dst;                //state counter for our brand new dfa
    dst = 0;
    int n;
    vector<int*> SDFA;       // holds the list of states created for the dfa and the set of nfa states
    vector<int> SNDFA;       // holds the number of nfa states in each SDFA state
    SDFA.reserve(nst*2);
    set<int> dfa_final;      // holds the set of final states for the dfa
    set<int> new_states;     // holds a set of states to use in the 4th step
    map<int, map<char,int> > transitions;
    map<char, set<int> >::iterator it;

    // Create d0. Each char points to a set of states
    for(int i=0;i<bst;i++){ d0[(char)t0[2*i]].insert(t0[2*i+1]); }

    // Create the first state for nfa(0)
    state* e2;
    state* e = new state;
    e->length    = 1;
    e->nstate = new int[e->length];
    e->nstate[0] = 0;    // Sequence representing the state in the NFA
    e->dstate    = 0;    // State in the DFA which leats to nstate with letter
    e->letter    = '?';  // Letter which leads to nstate(NFA) from dstate(DFA)

    // Push the first state transition
    p.push(e);

    while(!p.empty()){

      e = p.top();
      p.pop();

   /* 
    * 1 Search for e->nstate in the current set of states
    */
      if(debug){
	cerr << endl << "POP ESTAT " << e->dstate 
	     << "---(" << e->letter << ")-->[";
	for(int i=0;i<e->length;i++){ cerr << e->nstate[i] << ",";}
	cerr << endl;
      }

      int final = 0;
      for(unsigned int it=0; it<SDFA.size() && !final; it++){
	if( e->length != SNDFA[it] ) {
	  final = 0;
	} else {
	  final = 1;
	  for(int i=0; i<e->length && final; i++){
	    final = final && (e->nstate[i] == SDFA[it][i] );
	  }          
	  // 2 If e->nstate exists is has number N
	  n = final ? it : n;
	}
      }

      if( final ){
	if(debug){
	  cerr << " - Found that it is state " << n << endl;
	  cerr << " + ADD TRANSITION " << e->dstate 
	       << "---(" << e->letter << ")-->" << n << endl;
	}
	transitions[e->dstate][e->letter] = n;

      }else{         
   /*
    * 3 If e->nstate does not exist, create it with number N
    */
	n=dst++;
	SDFA.push_back(new int[e->length]);
	memcpy( SDFA[n], e->nstate, sizeof(int)*e->length);
	SNDFA.push_back(e->length);

	for(int i=0 ; i<e->length ; i++ ){
	  if(nfa_final.find(SDFA[n][i])!=nfa_final.end() ) dfa_final.insert(n);
	}
	transitions[e->dstate][e->letter] = n;

	if(debug){
	  cerr << " + CREATED DFA STATE " << n << " = [";
	  for(int i=0 ; i<e->length ; i++ ) cerr << SDFA[n][i] << ",";
	  cerr << "]" << endl;
	  cerr << " + ADD TRANSITION " << e->dstate 
	       << "---(" << e->letter << ")-->" << n << endl;
	}

   /* 
    * 4 For each letter in transitions(e) U d0 push new state N---(letter)-->nstate      
    */

	// Create a set of transitions that may not appear in d0
	map<char,set<int> > tr_tmp;
	for(int i=0; i<e->length; i++){
	  if((char)nfa[e->nstate[i]][0] != '?' )
	    tr_tmp[ (char)nfa[e->nstate[i]][0] ].insert( nfa[e->nstate[i]][1] );
	}

	// 4A  Firstly add all the transitions from d0
	for(map<char, set<int> >::iterator it=d0.begin(); it!=d0.end(); it++){

	  if(debug) cerr << " + Estat " << e->dstate
			 << ", tractant amb el caràcter " << (*it).first << endl;

	  new_states.clear();

	  // + Genate list of states for e2->letter
	  // - First states from d0
	  for( set<int>::iterator it2=(*it).second.begin(); it2!=(*it).second.end(); it2++){
	    new_states.insert( (*it2) );
	  }

	  for(int i=0; i<e->length; i++){
	    // Then states from e->nstate
	    if( nfa[e->nstate[i]][0] == (int)(*it).first ){
	      new_states.insert(nfa[e->nstate[i]][1]);
	    }
	  }

	  // Now create the new state to push on the stack
	  e2 = new state;
	  e2->dstate = n;
	  e2->letter = (*it).first;
	  e2->length = new_states.size();
	  e2->nstate = new int[e2->length];
	  int j=0;
	  for(set<int>::iterator it2=new_states.begin(); it2!=new_states.end(); it2++,j++){
	    e2->nstate[j] = (*it2);
	  }
	  p.push(e2);
	  
	  if(debug){
	    cerr << "  + Push nou estat " << e2->dstate << "---(" << e2->letter << ")--->[";
	    for(int k=0;k<e2->length; cerr << e2->nstate[k++] << ","){}
	    cerr << "]" << endl;
	  }
	  tr_tmp.erase((char)(*it).first);

	}

	// 4B  Secondly add all the transitions from e->nstate
	for(map<char, set<int> >::iterator it=tr_tmp.begin(); it!=tr_tmp.end(); it++){
	  // Now create the new state to push on the stack
	  e2 = new state;
	  e2->dstate = n;
	  e2->letter = (*it).first;
	  e2->length = (*it).second.size();
	  e2->nstate = new int[e2->length];
	  int j=0;
	  for(set<int>::iterator it2=(*it).second.begin(); it2!=(*it).second.end(); it2++,j++){
	    e2->nstate[j] = (*it2);
	  }
	  p.push(e2);
	  
	  if(debug){
	    cerr << "  + Push nou estat " << e2->dstate << "---(" << e2->letter << ")--->[";
	    for(int k=0;k<e2->length; cerr << e2->nstate[k++] << ","){}
	    cerr << "]" << endl;
	  }
	  
	}

      }

      delete[](e->nstate);
      delete(e);

      /*
       *   5 <- Nothing to do
       */

    } // end while(p.empty)

    nstates = transitions.size();
    dfa = new int[WIDTH*nstates];  
    for(int i=0; i<WIDTH*nstates; i++) 
      dfa[i]=0;

    for( map<int,map<char,int> >::iterator it = transitions.begin(); it!=transitions.end(); it++){
      dfa[(*it).first*WIDTH] = (*it).first;
      if( dfa_final.find((*it).first)!=dfa_final.end() ) dfa[(*it).first*WIDTH+1] = 1;
      for( map<char,int>::iterator it2 = (*it).second.begin(); it2!=(*it).second.end(); it2++){
	dfa[ (*it).first*WIDTH + (int)(*it2).first - DESP] = (*it2).second;
      }
    }

    for(vector<int*>::iterator it=SDFA.begin(); it!=SDFA.end(); it++){
      delete[](*it);
    }

  } // end createDFA


  /*
   *  SHAPE OF THE DFA MATRIX:
   *
   *  Width = 97
   *  Column 0: State number
   *  Column 1: Final state flag
   *  Initial state: 0
   *  Input symbols: Characters between ASCI: 32..126. The characters are shifted 31 positions.
   *
   */



public:

  
  golem(const char* const _query, const int _length, const int _t, int _debug){
    debug= _debug>0 ? debug : 0;
    t = _t;
    n = _length;
    query = new char[_length+1];
    strncpy(query,_query,_length);
    query[_length]=0;
    createNFA();
    createDFA();
    status = 0;
  }

  void reset(){
    status = 0;
  }

  int read(char const c){
    if(c<DESP) return 0;
    //cerr << status;
    status = dfa[ status*WIDTH + (int)c-DESP];
    //cerr <<"->"<<c<< "->"<<status<<" ("<<dfa[status*WIDTH+1]<<")"<<endl;
    return dfa[status*WIDTH+1];
  }

  void show(){
    cerr << endl << "AQUEST ES L'AUTOMAT DFA: " << endl;
    for( int i=0; i<nstates; i++){
      cerr << dfa[i*WIDTH];
      if( dfa[i*WIDTH+1] == 1 ) cerr << "+"; 
      cerr << "\t";
      for(int j=2; j<WIDTH; j++){
	cerr << (char)(j+DESP) << "-" << dfa[i*WIDTH+j] << " ";
      }
      cerr << endl;
    }
  }

  ~golem(){
    delete[](dfa);
    delete[](t0);
    delete[](query);
    for(int i=0; i<nst; i++){
      delete[](nfa[i]);
    }
    delete[](nfa);

  }

};

#endif

