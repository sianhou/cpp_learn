//
// Created by 侯思安 on 2023/5/28.
//

#ifndef LEARN_CPP_SJSTL_SJSTACK_H_
#define LEARN_CPP_SJSTL_SJSTACK_H_

#include "sjdeque.h"

#define __STL_NULL_TMPL_ARGS <>

namespace sj {

template<class T, class Sequence = Deque<T, alloc, 8>>
class Stack {
  friend bool operator==(const Stack &, const Stack &);
  friend bool operator<(const Stack &, const Stack &);

 public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;
 protected:
  Sequence c;
 public:
  bool empty() { return c.empty(); }
  size_type size() { return c.size(); }
  reference top() { return c.back(); }
  void push(const value_type &x) { c.push_back(x); }
  void pop() { c.pop_back(); }
};

template<class T, class Sequence>
bool operator==(const Stack<T, Sequence> &x, const Stack<T, Sequence> &y) {
  return x.c == y.c;
}

template<class T, class Sequence>
bool operator<(const Stack<T, Sequence> &x, const Stack<T, Sequence> &y) {
  return x.c < y.c;
}

}

#endif //LEARN_CPP_SJSTL_SJSTACK_H_
