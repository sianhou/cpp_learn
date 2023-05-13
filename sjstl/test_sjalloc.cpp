//
// Created by dell on 2023/5/12.
//
#include "sjalloc.h"
#include "vector"

int main() {

    // test alloc
//    int ia[5] = {0, 1, 2, 3, 4};
    size_t i;
//
//    std::vector<int, sj::allocator<int>> iv(ia, ia + 5);
//    for (i = 0; i < iv.size(); ++i) {
//        std::cout << iv[i] << " ";
//    }
//    std::cout << std::endl;

    // test malloc_alloc
    auto handler = []() {
        std::cout << "allocate failed!" << std::endl;
        exit(EXIT_FAILURE);
    };
    sj::malloc_alloc::set_malloc_handler(handler);

    i = 512*512*512;
    void *p = sj::malloc_alloc::allocate(i*sizeof(int));
    i *= 2;
    p = sj::malloc_alloc::reallocate(p, i/2, i*sizeof(int));
    sj::malloc_alloc::deallocate(p, i*sizeof(int));

    // test
    p = sj::__default_alloc_template<true, 0>::allocate(16);
    void *q = sj::__default_alloc_template<true, 0>::allocate(16);
    sj::__default_alloc_template<true, 0>::deallocate(p, 16);
    sj::__default_alloc_template<true, 0>::deallocate(q, 16);
}