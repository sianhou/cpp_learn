//
// Created by dell on 2023/5/12.
//

#ifndef LEARN_CXX_SJSTL_SJALLOC_H_
#define LEARN_CXX_SJSTL_SJALLOC_H_

#include "cstddef"
#include "new"
#include "iostream"
#include "climits"

namespace sj {

template<class T>
inline T *_allocte(ptrdiff_t size, T *) {
    std::set_new_handler(0);
    T *tmp = (T *) (::operator new((size_t) (size * sizeof(T))));
    if (tmp == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(EXIT_FAILURE);
    }
    return tmp;
}

template<class T>
inline void _deallocate(T *buffer) {
    ::operator delete(buffer);
}

template<class T1, class T2>
inline void _construct(T1 *p, const T2 &value) {
    //通过查询了解到这个操作叫做placement new，就是在指针p所指向的内存空间创建一个T1类型的对象，但是对象的内容是从T2类型的对象转换过来的（调用了T1的构造函数，T1::T1(value)）。
    //就是在已有空间的基础上重新调整分配的空间，类似于realloc函数。这个操作就是把已有的空间当成一个缓冲区来使用，这样子就减少了分配空间所耗费的时间，因为直接用new操作符分配内存的话，在堆中查找足够大的剩余空间速度是比较慢的。
    //————————————————
    //版权声明：本文为CSDN博主「cylianging」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
    //原文链接：https://blog.csdn.net/u014209688/article/details/90047713
    new(p) T1(value);
}

template<class T>
inline void _destroy(T *ptr) {
    ptr->~T();
}

template<typename T>
class allocator {
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

// rebind alloctor of type U
  template<class U>
  struct rebind {
    using other = allocator<U>;
  };

  // hint used for locality
  pointer allocate(size_type n, const void *hint = 0) {
      return _allocte((difference_type) n, (pointer) 0);
  }

  void deallocate(pointer p, size_type n) {
      _deallocate(p);
  }

  void construct(pointer p, const T &value) {
      _construct(p, value);
  }

  void destroy(pointer p) {
      _destroy(p);
  }

  pointer address(reference x) {
      return (pointer) &x;
  }

  const_pointer const_address(const_reference x) {
      return (const_pointer) &x;
  }

  size_type max_size() const {
      return size_type(UINT_MAX / (sizeof(T)));
  }

};
}

#endif //LEARN_CXX_SJSTL_SJALLOC_H_
