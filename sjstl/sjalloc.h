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
    T *tmp = (T *)(::operator new((size_t)(size*sizeof(T))));
    if (tmp == nullptr) {
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
        return _allocte((difference_type)n, (pointer)0);
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
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }

    size_type max_size() const {
        return size_type(UINT_MAX/(sizeof(T)));
    }

};

#if 0
#include "new"
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include "iostream"
#define __THROW_BAD_ALLOC do { \
        std::cerr << "out of memory" << std::endl; \
        exit(EXIT_FAILURE);         \
        } while(0)
#endif

template<int inst>
class __malloc_alloc_template {
 private:
    //oom: out of memory
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (*__malloc_alloc_oom_handler)();

 public:
    static void *allocate(size_t n) {
        void *result;
        if (nullptr == (result = malloc(n))) { // use malloc directly in the first class allocator
            result = oom_malloc(n);   // use oom when malloc fail
        }
        return result;
    }

    static void *deallocate(void *p, size_t) {
        free(p);
        return p;
    }

    static void *reallocate(void *p, size_t, size_t new_size) {
        void *result;
        if (nullptr == (result = realloc(p, new_size))) {
            result = oom_realloc(p, new_size);
        }
        return result;
    }

    static void (*set_malloc_handler(void (*f)()))() {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
};

// default 0, you must set it
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = nullptr;

template<int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void (*my_malloc_handler)();
    void *result;

    do {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (nullptr == my_malloc_handler) {
            __THROW_BAD_ALLOC;
        }
        (*my_malloc_handler)(); // 调用处理函数，试图释放内存
        if (0 != (result = malloc(n))) {
            return result;
        }
    } while (1);
}

template<int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) {
    void (*my_malloc_handler)();
    void *result;

    do {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) {
            __THROW_BAD_ALLOC;
        }
        (*my_malloc_handler)(); // 调用处理函数，试图释放内存
        if (0 != (result = realloc(p, n))) {
            return result;
        }
    } while (1);
}

// __default_alloc_template
enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES/__ALIGN };

template<bool threads, int inst>
class __default_alloc_template {
 private:
    // round up to 8 times
    static size_t ROUND_UP(size_t bytes) {
        return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
    }
 private:
    union obj {
        union obj *free_list_link;
        char client_data[1];
    };
 private:
    // 16 free-lists
    static obj *volatile free_list[__NFREELISTS];
    // calculate the index
    static size_t FREELIST_INDEX(size_t bytes) {
        return ((bytes + __ALIGN - 1)/__ALIGN - 1);
    }

    // return a n size obj, may add a n block to free list
    static void *refill(size_t n) {
        int nobjs = 20;

        // use chunk_alloc
        char *chunk = chunk_alloc(n, nobjs);
        obj *volatile *my_free_list;
        obj *result;
        obj *current_obj, *next_obj;

        if (1 == nobjs) return (chunk);

        my_free_list = free_list + FREELIST_INDEX(n);
        result = (obj *)chunk;
        *my_free_list = next_obj = (obj*)(chunk + n);
        for (int i = 1;; ++i) {
            current_obj = next_obj;
            next_obj = (obj*)((char *)next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->free_list_link = nullptr;
                break;
            } else {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }

    // allocate a big space which contains nobjs chunk
    // if nobjs is inavailable, nobjs may decrease
    // size must be times of 8
    static char *chunk_alloc(size_t size, int &nobjs) {
        char *result;
        size_t total_bytes = size*nobjs;
        size_t bytes_left = end_free - start_free; // left space in memory pool

        if (bytes_left >= total_bytes) {
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if (bytes_left >= size) {
            nobjs = bytes_left/size;
            total_bytes = size*nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {
            size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size >> 4);

            // use the left bytes in memory pool
            if (bytes_left > 0) {
                obj *volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj *)start_free;
            }

            // allocate heap
            start_free = (char *)malloc(bytes_to_get);
            if (nullptr == start_free) {
                // malloc fail
                int i;
                obj *volatile *my_free_list, *p;
                for (int i = size; i < __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (nullptr != p) {
                        *my_free_list = p->free_list_link;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        // use every small blocks
                        return (chunk_alloc(size, nobjs));
                    }
                }
            }
            end_free = nullptr;
            start_free = (char *)__malloc_alloc_template<0>::allocate(bytes_to_get);
            heap_size += bytes_to_get;
            end_free = start_free + heap_size;
            return chunk_alloc(size, nobjs);
        }
    }

    // chunk allocation state
    static char *start_free;
    static char *end_free;
    static size_t heap_size;

 public:
    static void *allocate(size_t n) {
        // n must be > 0
        obj *volatile *my_free_list;
        obj *result;

        // use the first class allocate if n > 128
        if (n > (size_t)__MAX_BYTES) {
            return (__malloc_alloc_template<0>::allocate(n));
        }

        // find a proper list in free lists
        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if (result == 0) {
            // don't find available free list
            void *r = refill(ROUND_UP(n));
            return r;
        }

        // set free list
        *my_free_list = result->free_list_link;
        return result;
    }

    static void deallocate(void *p, size_t n) {
        obj *q = (obj *)p;
        obj *volatile *my_free_list;

        // if n > 128
        if (n > (size_t)__MAX_BYTES) {
            __malloc_alloc_template<0>::deallocate(p, n);
            return;
        }

        // find index
        my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    static void *reallocate(void *p, size_t old_size, size_t new_size) {}

};
template<bool threads, int inst>
char *__default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char *__default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj *volatile
    __default_alloc_template<threads, inst>::free_list[__NFREELISTS] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

#ifdef __USE_MALLOC
typedef sj::__malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef sj::__default_alloc_template<false, 0> alloc;
#endif

template<class T, class Alloc>
class simple_alloc {
 public:
    static T *allocate(size_t n) {
        return 0 == n ? nullptr : (T *)Alloc::allocate(n*sizeof(T));
    }
    static T *allocate() {
        return (T *)Alloc::allocate(sizeof(T));
    }
    static void deallocate(T *p, size_t n) {
        if (0 != n) {
            Alloc::deallocate(p, n*sizeof(T));
        }
    }
    static void deallocate(T *p) {
        Alloc::deallocate(p, sizeof(T));
    }
};
}
#endif //LEARN_CXX_SJSTL_SJALLOC_H_
