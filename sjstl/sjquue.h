//
// Created by 侯思安 on 2023/5/28.
//

#ifndef LEARN_CPP_SJSTL_SJQUUE_H_
#define LEARN_CPP_SJSTL_SJQUUE_H_
#include "sjdeque.h"
namespace sj {
template<class T, class Sequence = Deque<T, alloc, 8>>
class Queue {
  friend bool operator==(const Queue &, const Queue &);
  friend bool operator<(const Queue &, const Queue &);

 public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;
 protected:
  Sequence c;
 public:
  bool empty() { return c.empty(); }
  size_type size() { return c.size(); }
  reference front() { return c.front(); }
  reference back() { return c.back(); }
  void push(const value_type &x) { c.push_back(x); }
  void pop() { c.pop_front(); }
};

template<class T, class Sequence>
bool operator==(const Queue<T, Sequence> &x, const Queue<T, Sequence> &y) {
  return x.c == y.c;
}

template<class T, class Sequence>
bool operator<(const Queue<T, Sequence> &x, const Queue<T, Sequence> &y) {
  return x.c < y.c;
}

}

#endif //LEARN_CPP_SJSTL_SJQUUE_H_
