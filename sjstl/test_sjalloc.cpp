//
// Created by dell on 2023/5/12.
//
#include "sjalloc.h"
#include "vector"

int main() {

    // test alloc
//    int ia[5] = {0, 1, 2, 3, 4};
//    size_t i;
//
//    std::vector<int, sj::allocator<int>> iv(ia, ia + 5);
//    for (i = 0; i < iv.size(); ++i) {
//        std::cout << iv[i] << " ";
//    }

    // test
    float *p = sj::simple_alloc<float, sj::alloc>::allocate(32);
    float *q = sj::simple_alloc<float, sj::alloc>::allocate(64);
    sj::simple_alloc<float, sj::alloc>::deallocate(p, 32);
    sj::simple_alloc<float, sj::alloc>::deallocate(q, 64);
}