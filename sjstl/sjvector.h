//
// Created by 侯思安 on 2023/5/20.
//

#ifndef LEARN_CPP_SJSTL_SJVECTOR_H_
#define LEARN_CPP_SJSTL_SJVECTOR_H_

#include "sjalloc.h"
#include "sjconstruct.h"

namespace sj {
template<class T, class Alloc=alloc>
class Vector {
 public:
  typedef T value_type;
  typedef value_type *pointer;
  typedef value_type *iterator;
  typedef value_type &reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 protected:
  typedef simple_alloc<value_type, Alloc> data_allocator;
  iterator start;
  iterator finish;
  iterator end_of_storage;

  void insert_aux(iterator position, const T &x);
  void deallocate() {
    if (start) {
      data_allocator::deallocate(start, end_of_storage - start);
    }
  }

  iterator allocate_and_fill(size_type n, const T &x) {
    iterator result = data_allocator::allocate(n);
    uninitialized_fill_n(result, n, x);
    return result;
  }

  void fill_initialized(size_type n, const T &value) {
    start = allocate_and_fill(n, value);
    finish = start + n;
    end_of_storage = finish;
  }
 public:
  iterator begin() { return start; }
  iterator end() { return finish; }
  size_type size() { return size_type(begin() - end()); }
  size_type capacity() { return size_type(end_of_storage - begin()); }
  bool empty() { return begin() == end(); }
  reference operator[](size_type n) { return *(begin() + n); }

  Vector() : start(0), finish(0), end_of_storage(0) {}
  Vector(size_type n, const T &value) { fill_initialized(n, value); }
  Vector(int n, const T &value) { fill_initialized(n, value); }
  Vector(long n, const T &value) { fill_initialized(n, value); }
  explicit Vector(size_type n) { fill_initialized(n, T()); }

  ~Vector() {
    destory(start, finish);
    deallocate();
  }

  reference front() { return *begin(); }
  reference back() { return *(end() - 1); }
  void push_back(const T &x) {
    if (finish != end_of_storage) {
      construct(finish, x);
      ++finish;
    } else {
      insert_aux(end(), x);
    }
  }

  void pop_back() {
    --finish;
    destroy(finish);
  }

  iterator erase(iterator position) {
    if (position + 1 != end()) {
      copy(position + 1, finish, position);
      --finish;
      destroy(finish);
      return position;
    }
  }

  void resize(size_type new_size, const T &x) {
    if (new_size < size()) {
      erase(begin() + new_size, end());
    } else {
      //TODO(HOUSIAN): insert
    }
  }

  void resize(size_type new_size) {
    resize(new_size, T());
  }

  void clear() {
    erase(begin(), end());
  }

};
}

#endif //LEARN_CPP_SJSTL_SJVECTOR_H_
