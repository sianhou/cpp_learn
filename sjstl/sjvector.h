//
// Created by 侯思安 on 2023/5/20.
//

#ifndef LEARN_CPP_SJSTL_SJVECTOR_H_
#define LEARN_CPP_SJSTL_SJVECTOR_H_

#include "sjalloc.h"
#include "sjconstruct.h"
#include "algorithm"

namespace sj {
template<class T, class Alloc=alloc>
class Vector {
 public:
  typedef T value_type;
  typedef value_type *pointer;
  typedef value_type *iterator;
  typedef const value_type *const_iterator;
  typedef value_type &reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 protected:
  typedef simple_alloc<value_type, Alloc> data_allocator;
  iterator start;
  iterator finish;
  iterator end_of_storage;

  void insert_aux(iterator position, const T &x) {
    if (finish != end_of_storage) {
      //TODO(HOUSIAN): p122
    } else {
      const size_type old_size = size();
      const size_type len = old_size != 0 ? 2 * old_size : 1;

      iterator new_start = data_allocator::allocate(len);
      iterator new_finish = new_start;

      try {
        new_finish = uninitialized_copy(start, position, new_start);
        construct(new_finish, x);
        ++new_finish;
        new_finish = uninitialized_copy(position, finish, new_finish);
      } catch (...) {
        // "commit or rollback"
        destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, len);
        throw;
      }

      destroy(begin(), end());
      deallocate();
      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + len;
    }
  }
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
  const_iterator cbegin() const { return static_cast<const_iterator>(start); };
  iterator end() { return finish; }
  const_iterator cend() const { return static_cast<const_iterator>(finish); };
  size_type size() { return size_type(end() - begin()); }
  size_type capacity() { return size_type(end_of_storage - begin()); }
  bool empty() { return begin() == end(); }
  reference operator[](size_type n) { return *(begin() + n); }

  Vector() : start(0), finish(0), end_of_storage(0) {}
  Vector(size_type n, const T &value) { fill_initialized(n, value); }
  Vector(int n, const T &value) { fill_initialized(n, value); }
  Vector(long n, const T &value) { fill_initialized(n, value); }
  explicit Vector(size_type n) { fill_initialized(n, T()); }

  ~Vector() {
    destroy(start, finish);
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
      std::copy(position + 1, finish, position);
      --finish;
      destroy(finish);
      return position;
    }
  }

  iterator erase(iterator first, iterator last) {
    iterator i = std::copy(last, finish, first);
    destroy(i, finish);
    finish = finish - (last - first);
    return first;
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

  void insert(iterator position, size_type n, const T &x) {
    if (n != 0) {
      // the remaining space is enough
      if (size_type(end_of_storage - finish) >= n) {
        T x_copy = x;
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if (elems_after > n) {
          uninitialized_copy(finish - n, finish, finish);
          finish += n;
          std::copy_backward(position, old_finish - n, old_finish);
          std::fill(position, position + n, x_copy);
        } else {
          uninitialized_fill_n(finish, n - elems_after, x_copy);
          finish += n;
          uninitialized_copy(position, old_finish, finish);
          finish += elems_after;
          std::fill(position, old_finish, x_copy);
        }
      } else {
        const size_type old_size = size();
        const size_type len = old_size + (old_size >= n ? old_size : n);
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        new_finish = uninitialized_copy(start, position, new_start);
        new_finish = uninitialized_fill_n(new_finish, n, x);
        new_finish = uninitialized_copy(position, finish, new_finish);
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
      }
    }
  }

  void display(std::ostream &os = std::cout) const {
    for (const_iterator item = this->cbegin(); item != this->cend(); ++item) {
      os << *item << std::endl;
    }
  }
};

}

#endif //LEARN_CPP_SJSTL_SJVECTOR_H_
