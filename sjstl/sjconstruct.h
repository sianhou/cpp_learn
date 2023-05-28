//
// Created by sianh on 2023/5/13.
//

#ifndef LEARN_CPP_SJSTL_SJCONSTRUCT_H_
#define LEARN_CPP_SJSTL_SJCONSTRUCT_H_

#include "new"
#include "sjiterator.h"

namespace sj {
template<class T1, class T2>
inline void construct(T1 *p, const T2 &value) {
  new(p) T1(value);
}

template<class T>
inline void destroy(T *pointer) {
  pointer->~T();
}

template<class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
  for (; first < last; ++first) {
    destroy(&*first);
  }
}

template<class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

template<class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T *) {
  typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
  __destroy_aux(first, last, trivial_destructor());
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
  __destroy(first, last, value_type(first));
}

inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *) {}

// uninitialized_copy
template<class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
  //TODO(HOUSIAN): use std now, change to personal stl later
  return std::copy(first, last, result);
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
  ForwardIterator cur = result;
  while (first != last) {
    construct(&*cur, *first);
    first++;
    cur++;
  }
  return cur;
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T *) {
  typedef typename __type_traits<InputIterator>::is_POD_type is_POD;
  return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
  return __uninitialized_copy(first, last, result, value_type(first));
}

template<>
inline char *uninitialized_copy(const char *first, const char *last, char *result) {
  memmove(result, first, last - first);
  return result + (last - first);
}

template<>
inline wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

// uninitialized_fill
template<class ForwardIterator, class T>
inline void
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __true_type) {
  //TODO(HOUSIAN): use std now, change to personal stl later
  std::fill(first, last, x);
}

template<class ForwardIterator, class T>
inline void
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __false_type) {
  ForwardIterator cur = first;
  for (; cur != last; ++cur) {
    construct(&*cur, x);
  }
}

template<class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x, T1 *) {
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  __uninitialized_fill_aux(first, last, x, is_POD());
}

template<class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x) {
  __uninitialized_fill(first, last, x, value_type(first));
}

// uninitialized_fill_n

template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &value) {
  for (; n > 0; --n, ++first) {
    *first = value;
  }
  return first;
}

template<class ForwardIterator, class Size, class T>
ForwardIterator
__uninitialzied_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type) {
  ForwardIterator cur = first;
  for (; n > 0; --n, ++cur) {
    construct(&*cur, x);
  }
  return cur;
}

template<class ForwardIterator, class Size, class T>
ForwardIterator
__uninitialzied_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type) {
  return fill_n(first, n, x);
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, T1 *) {
  typedef typename __type_traits<T1>::is_POD_type is_POD;
  return __uninitialzied_fill_n_aux(first, n, x, is_POD());
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x) {
  return __uninitialized_fill_n(first, n, x, value_type(first));
}

}

#endif //LEARN_CPP_SJSTL_SJCONSTRUCT_H_
