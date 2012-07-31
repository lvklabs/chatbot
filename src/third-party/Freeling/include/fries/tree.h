//////////////////////////////////////////////////////////////////
//
//    STL-like n-ary tree template 
//
//    Copyright (C) 2006   TALP Research Center
//                         Universitat Politecnica de Catalunya
//
//    This program is free software; you can redistribute it 
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

#ifndef _TREE_TEMPLATE
#define _TREE_TEMPLATE


template <class T> 
class tree { 

 private:
  bool isempty;
  tree *parent;        // parent node
  tree *first,*last;   // first/last child
  tree *prev,*next;    // prev/next sibling
  void clone(const tree<T>&);

 public:
  T info;
  class generic_iterator;
  class preorder_iterator;
  //  class const_preorder_iterator;
  class sibling_iterator;
  // class const_sibling_iterator;
  typedef preorder_iterator iterator;
  //typedef const_preorder_iterator const_iterator;
 
  tree();
  tree(const T&);
  tree(const tree<T>&);
  tree(const tree<T>::preorder_iterator&);
  ~tree();
  tree<T>& operator=(const tree<T>&);

  unsigned int num_children() const;
  sibling_iterator nth_child(unsigned int) const;
  iterator get_parent() const;
  tree<T> & nth_child_ref(unsigned int) const;
  T& get_info();
  void append_child(const tree<T> &);
  void hang_child(tree<T> &, bool);
  void clear();
  bool empty() const;

  sibling_iterator sibling_begin();
  sibling_iterator sibling_rbegin();
  //const_sibling_iterator sibling_begin() const;
  sibling_iterator sibling_end() const;
  sibling_iterator sibling_rend() const;

  preorder_iterator begin();
  // const_preorder_iterator begin() const;
  preorder_iterator end() const;

  class generic_iterator  { 
    protected:
       tree *pnode;
    public:
       generic_iterator();
       generic_iterator(tree *);
       tree<T>& operator*() const;
       tree<T>* operator->() const;
       bool operator==(const generic_iterator &) const;
       bool operator!=(const generic_iterator &) const;
  };

  /// traverse the tree in preorder (parent first, then children)
  class preorder_iterator : public generic_iterator { 
    public:
       preorder_iterator();
       preorder_iterator(tree *);
       preorder_iterator(sibling_iterator&);

       preorder_iterator& operator++();
       preorder_iterator& operator--();
       preorder_iterator& operator+=(unsigned int);
       preorder_iterator& operator-=(unsigned int);
  };

/*   class const_preorder_iterator : public generic_iterator {  */
/*     public: */
/*        const_preorder_iterator(); */
/*        const_preorder_iterator(const tree *); */
/*        const_preorder_iterator(const const_sibling_iterator &); */

/*        const_preorder_iterator& operator++(); */
/*        const_preorder_iterator& operator--(); */
/*        const_preorder_iterator& operator+=(unsigned int); */
/*        const_preorder_iterator& operator-=(unsigned int); */
/*   }; */

 /// traverse all children of the same node
 class sibling_iterator : public generic_iterator {
    friend class preorder_iterator;
    public:
       sibling_iterator();
       sibling_iterator(tree *);

       sibling_iterator& operator++();
       sibling_iterator& operator--();
       sibling_iterator& operator+=(unsigned int);
       sibling_iterator& operator-=(unsigned int);
  };

/*  class const_sibling_iterator : public generic_iterator { */
/*     friend class const_preorder_iterator; */
/*     public: */
/*        const_sibling_iterator(); */
/*        const_sibling_iterator(const tree *); */

/*        const_sibling_iterator& operator++(); */
/*        const_sibling_iterator& operator--(); */
/*        const_sibling_iterator& operator+=(unsigned int); */
/*        const_sibling_iterator& operator-=(unsigned int); */
/*   }; */
};


/// Method implementations for class tree

/// constructor: empty tree doesn't exist, it's a one-node tree with no info. Be careful

template <class T>
tree<T>::tree() {
  isempty = true;
  parent=NULL;
  first=NULL; last=NULL;
  prev=NULL; next=NULL;
}


/// constructor: one-node tree

template <class T>
tree<T>::tree(const T &x) : info(x) {
  isempty = false;
  parent=NULL;
  first=NULL; last=NULL;
  prev=NULL; next=NULL;
}

/// constructor from an iterator

template <class T>
tree<T>::tree(const tree<T>::preorder_iterator &p) {
  clone(*p);
}

/// copy constructor

template <class T>
tree<T>::tree(const tree<T>& t) {
  clone(t);
}


/// assignment 

template <class T>
tree<T>& tree<T>::operator=(const tree<T>& t) {
  if (this != &t) {
    clear();
    clone(t);
  }
  return (*this);
}

///destructor

template <class T>
tree<T>::~tree() {
  tree *p=this->first;
  while (p!=NULL) {
    tree *q=p->next;
    delete p;
    p=q;
  }
}

template <class T>
void tree<T>::clear() {

  // delete children
  tree *p=this->first;
  while (p!=NULL) {
    tree *q=p->next;
    delete p;
    p=q;
  }

  // reset root node
  isempty = true;
  parent=NULL;
  first=NULL; last=NULL;
  prev=NULL; next=NULL;

}

/// number of children

template <class T>
unsigned int tree<T>::num_children() const {
tree *s;
  unsigned int n=0;
  for (s=this->first; s!=NULL; s=s->next) n++;
  return n;
}

/// access parent

template <class T>
typename tree<T>::iterator tree<T>::get_parent() const { 
  iterator i = this->parent;    
  return i;
}

/// access nth child

template <class T>
typename tree<T>::sibling_iterator tree<T>::nth_child(unsigned int n) const { 
  sibling_iterator i = this->first;    
  while (n>0 && i!=NULL) {
    i = i->next;
    n--;
  }
  return i;
}


/// access nth child ref (useful for Java API)

template <class T>
tree<T> & tree<T>::nth_child_ref(unsigned int n) const { 
  sibling_iterator i = this->first;
  while (n>0) {
    i = i->next;
    n--;
  }
  return (*i);
}

/// access info (useful for Java API)

template <class T>
T& tree<T>::get_info() {
  return info;
}


/// detect empty tree

template <class T>
bool tree<T>::empty() const {
  return isempty;
}

/// begin/end sibling iterator

template <class T>
typename tree<T>::sibling_iterator tree<T>::sibling_begin() {
   return sibling_iterator(this->first);
}

template <class T>
typename tree<T>::sibling_iterator tree<T>::sibling_rbegin() {
   return sibling_iterator(this->last);
}

/* template <class T> */
/* typename tree<T>::const_sibling_iterator tree<T>::sibling_begin() const { */
/*    return const_sibling_iterator(this->first); */
/* } */

template <class T>
typename tree<T>::sibling_iterator tree<T>::sibling_end() const {
   return sibling_iterator(NULL);
}

template <class T>
typename tree<T>::sibling_iterator tree<T>::sibling_rend() const {
   return sibling_iterator(NULL);
}

/// begin/end preorder iterator

template <class T>
typename tree<T>::preorder_iterator tree<T>::begin() {
   return preorder_iterator(this);
}

/* template <class T> */
/* typename tree<T>::const_preorder_iterator tree<T>::begin() const { */
/*    return const_preorder_iterator(this); */
/* } */

template <class T>
typename tree<T>::preorder_iterator tree<T>::end() const {
   return preorder_iterator(NULL);
}

/// append child to a tree

template <class T>
void tree<T>::append_child(const tree<T>& child) {

  // make a copy
  tree<T> *x = new tree<T>;
  x->clone(child);

  x->next = NULL;  x->prev = NULL;
  x->parent = this;

  if (this->first != NULL) {  // there are already children, join them
    x->prev = this->last;
    this->last->next = x;
    this->last = x;
  }
  else {
    // no children, new is the only one
    this->first = x; this->last = x;
  }
}

/// hang a tree as last child of another (no copying!!)

template <class T>
void tree<T>::hang_child(tree<T>& child, bool last=true) {

  // remove child from its current location:
  // 1- remove it from siblings chain
  if (child.prev) child.prev->next=child.next;
  if (child.next) child.next->prev=child.prev;  
  // 2- adujst parent pointers if first or last child
  if (child.parent) {
    if (!child.prev) child.parent->first=child.next;
    if (!child.next) child.parent->last=child.prev;
  }

  // hang child on new location
  child.prev=NULL;
  child.next=NULL;
  child.parent = this;

  if (this->first == NULL) { 
    // there are no children, new is the only one
    this->first = &child;
    this->last = &child;
  }
  else {
    // already children, join them
    if (last) {
      // append new node as last child
      child.prev = this->last;
      this->last->next = &child;
      this->last = &child;
    }
    else {
      // append new node as first child
      child.next = this->first;
      this->first->prev = &child;
      this->first = &child;      
    }
  }
}

/// clone an entire tree

template <class T>
void tree<T>::clone(const tree<T>& t) {

  this->isempty = t.isempty;
  this->info = t.info;
  this->parent = NULL;
  this->first = NULL;
  this->last = NULL;
  this->prev=NULL;
  this->next=NULL;

  for (tree* p=t.first; p!=NULL; p=p->next) {

    tree<T>* c = new tree<T>;
    c->clone(*p);
    c->next = NULL;  
    c->prev = NULL;
    c->parent = this;

    if (this->first != NULL) {
      c->prev = this->last;
      this->last->next = c;
      this->last = c;
    }
    else {
      this->first = c; 
      this->last = c;
    }
  }
}


/////////////////// Method implementations for class generic_iterator

template <class T>
tree<T>::generic_iterator::generic_iterator() {pnode = NULL;}

template <class T>
tree<T>::generic_iterator::generic_iterator(tree *t) {pnode = t;}

template <class T>
tree<T>& tree<T>::generic_iterator::operator*() const {return (*pnode);}

template <class T>
tree<T>* tree<T>::generic_iterator::operator->() const {return pnode;}

template <class T>
bool tree<T>::generic_iterator::operator==(const generic_iterator &t) const {return (t.pnode==this->pnode); }

template <class T>
bool tree<T>::generic_iterator::operator!=(const generic_iterator &t) const {return (t.pnode!=this->pnode); }




/////////////////// Method implementations for class preorder_iterator

template <class T>
tree<T>::preorder_iterator::preorder_iterator() : generic_iterator() {}

template <class T>
tree<T>::preorder_iterator::preorder_iterator(tree *t) : generic_iterator(t) {}

template <class T>
tree<T>::preorder_iterator::preorder_iterator(sibling_iterator &t) : generic_iterator(t) {}

template <class T>
typename tree<T>::preorder_iterator& tree<T>::preorder_iterator::operator++() {
  if (this->pnode->first != NULL) 
    this->pnode=this->pnode->first;
  else {
    while (this->pnode!=NULL && this->pnode->next==NULL) 
      this->pnode=this->pnode->parent;
    if (this->pnode!=NULL) this->pnode=this->pnode->next;
  }
  return *this;
}

template <class T>
typename tree<T>::preorder_iterator& tree<T>::preorder_iterator::operator--() {
  if (this->pnode->prev!=NULL) {
    this->pnode=this->pnode->prev;
    while (this->pnode->last != NULL)
      this->pnode=this->pnode->last;
  }
  else
    this->pnode = this->pnode->parent;

  return *this;
}

template <class T>
typename tree<T>::preorder_iterator& tree<T>::preorder_iterator::operator+=(unsigned int n) {
  for (; n>0; n--) ++(*this);
  return *this;
}

template <class T>
typename tree<T>::preorder_iterator& tree<T>::preorder_iterator::operator-=(unsigned int n) {
  for (; n>0; n--) --(*this);
  return *this;
}

/////////////////// Method implementations for class const_preorder_iterator

/* template <class T> */
/* tree<T>::const_preorder_iterator::const_preorder_iterator() : generic_iterator() {} */

/* template <class T> */
/* tree<T>::const_preorder_iterator::const_preorder_iterator(const tree *t) : generic_iterator(t) {} */

/* template <class T> */
/* tree<T>::const_preorder_iterator::const_preorder_iterator(const const_sibling_iterator &t) : generic_iterator(t) {} */

/* template <class T> */
/* typename tree<T>::const_preorder_iterator& tree<T>::const_preorder_iterator::operator++() { */
/*   if (this->pnode->first != NULL)  */
/*     this->pnode=this->pnode->first; */
/*   else { */
/*     while (this->pnode!=NULL && this->pnode->next==NULL)  */
/*       this->pnode=this->pnode->parent; */
/*     if (this->pnode!=NULL) this->pnode=this->pnode->next; */
/*   } */
/*   return *this; */
/* } */

/* template <class T> */
/* typename tree<T>::const_preorder_iterator& tree<T>::const_preorder_iterator::operator--() { */
/*   if (this->pnode->prev!=NULL) { */
/*     this->pnode=this->pnode->prev; */
/*     while (this->pnode->last != NULL) */
/*       this->pnode=this->pnode->last; */
/*   } */
/*   else */
/*     this->pnode = this->pnode->parent; */

/*   return *this; */
/* } */

/* template <class T> */
/* typename tree<T>::const_preorder_iterator& tree<T>::const_preorder_iterator::operator+=(unsigned int n) { */
/*   for (; n>0; n--) ++(*this); */
/*   return *this; */
/* } */

/* template <class T> */
/* typename tree<T>::const_preorder_iterator& tree<T>::const_preorder_iterator::operator-=(unsigned int n) { */
/*   for (; n>0; n--) --(*this); */
/*   return *this; */
/* } */


/// Method implementations for class sibling_iterator

template <class T>
tree<T>::sibling_iterator::sibling_iterator() : generic_iterator() {}

template <class T>
tree<T>::sibling_iterator::sibling_iterator(tree *t) : generic_iterator(t) {}

template <class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator++() {
  this->pnode = this->pnode->next; 
  return *this;
}

template <class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator--() {
  this->pnode = this->pnode->prev; 
  return *this;
}

template <class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator+=(unsigned int n) {
  for (; n>0; n--) ++(*this);
  return *this;
}

template <class T>
typename tree<T>::sibling_iterator& tree<T>::sibling_iterator::operator-=(unsigned int n) {
  for (; n>0; n--) --(*this);
  return *this;
}


/// Method implementations for class const_sibling_iterator

/* template <class T> */
/* tree<T>::const_sibling_iterator::const_sibling_iterator() : generic_iterator() {} */

/* template <class T> */
/* tree<T>::const_sibling_iterator::const_sibling_iterator(const tree *t) : generic_iterator(t) {} */

/* template <class T> */
/* typename tree<T>::const_sibling_iterator& tree<T>::const_sibling_iterator::operator++() { */
/*   this->pnode = this->pnode->next;  */
/*   return *this; */
/* } */

/* template <class T> */
/* typename tree<T>::const_sibling_iterator& tree<T>::const_sibling_iterator::operator--() { */
/*   this->pnode = this->pnode->prev;  */
/*   return *this; */
/* } */

/* template <class T> */
/* typename tree<T>::const_sibling_iterator& tree<T>::const_sibling_iterator::operator+=(unsigned int n) { */
/*   for (; n>0; n--) ++(*this); */
/*   return *this; */
/* } */

/* template <class T> */
/* typename tree<T>::const_sibling_iterator& tree<T>::const_sibling_iterator::operator-=(unsigned int n) { */
/*   for (; n>0; n--) --(*this); */
/*   return *this; */
/* } */

#endif

