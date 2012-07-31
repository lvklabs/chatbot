#ifndef _simplesearch_h

#define _simplesearch_h
#include <iostream>
#include <string>
#include <queue>
#include "aligner.h"
#include "golem.h"
#include "math.h"


class simplesearch{

private:

  class comparator{
    public:
      bool operator()(const aligner<int>::alin* a, const aligner<int>::alin* b) const{
        return a->score < b->score;
      }
  };

  aligner<int>* al;
  golem* g;
  string corpus;
  priority_queue<aligner<int>::alin*,vector<aligner<int>::alin*>, comparator> heap;
  static inline int max(const int& a, const int& b){ return a > b ? a : b; }
  static inline int min(const int& a, const int& b){ return a < b ? a : b; }


public:

    simplesearch(string const _scores, string const _corpus){
      al = new aligner<int>(_scores);
      corpus = _corpus;
    }


    /*
     * Finds the bests alignements of _s searching substrings of length _length
     */
    void search(char* const _s, const int _length, const int /*_debug*/){

      int const sizeq = strlen(_s);
      int const size = 4*sizeq;
      int const padding = (size - _length) / 2;

      char buf[size];
      char c;
      int b,j,k;
      int i=0,r=0;
      int hook = 0;
      int previous = 0;

      while( !heap.empty() ){heap.pop();}

      g = new golem(_s,sizeq,_length,1);
      //g->show();

      ifstream is;
      cout << "Searching file " << corpus << " for substring \"" << _s << "\"" << endl;

      is.open(corpus.c_str());
      while(!is.eof()){
	if(i<=r){
	  c = buf[ i%size ];
	} else {
	  is.read(&c,1);
	  buf[ i%size ] = c;
	}

	if( (b=g->read(c))!=0 && (i-previous)>=sizeq){
	  hook++;
	  previous = i;

	  // 1) Fill the buffer up to size
	  r = i>=r ? i+1 : r;
	  while( r<=i+padding && !is.eof() ){
	    is.read(&c,1);
	    buf[ r%size ] = c;
	    r++;
	  }

	  // 2) put the characters in the arrays
	  int left=0, right=0;
	  left = max(0,i-_length-padding+1);
	  right= min(r-1,i+padding);
	  char* target = new char[right-left+1];
	  cout << "Searching in i="<<i<<" (" << left << "," << right << "): ";
	  for(k=0,j=left;j<=right; j++,k++){
	    target[k] = buf[j%size];
	    cout << target[k];
	  }
	  cout << endl;
	  // 3) Align it!
	  aligner<int>::alin* result = al->align(_s,sizeq,target,right-left+1,SEMILOCAL);
	  result->begin += left;
	  result->end   += left;
	  result->seg = new char[result->end - result->begin+1];
	  for(k=0,j=result->begin;j<=result->end; j++,k++){
	    result->seg[k] = buf[j%size];
	  }

	  // 4) Save the result in a heap of n best solutions
	  heap.push(result);

	  delete[](target);
	} else {
	  //Nothing to do
	}
	i++;
      }

      is.close();
      delete(g);


      /* PRINT SOME RESULTS */
      cout << "Found " << hook << " hooks."<<endl;
      aligner<int>::alin* dummy;

      while( !heap.empty() ){
	dummy = heap.top();

	char* segment = new char[dummy->end - dummy->begin+1];
	cout << "MATCH:  ";
	  
	for(int i=0;i<=dummy->end-dummy->begin; i++){
	  cout << dummy->seg[i];
	}
	cout << endl << "   a:" << dummy->a << endl << "   b:" << dummy->b << endl;
	cout << "   Score = " << dummy->score << "   Begin = " << dummy->begin << " End = " << dummy->end << endl;

	heap.pop();
	delete[](dummy->seg);
	delete(dummy);
	delete[](segment);
      }

    } // search() function


    ~simplesearch(){
      delete(al);
    }

};

#endif

