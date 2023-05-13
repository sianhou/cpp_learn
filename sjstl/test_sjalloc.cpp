//
// Created by dell on 2023/5/12.
//
#include "sjalloc.h"
#include "sjconstruct.h"

int main() {
    // test simple_alloc
    float *p = sj::simple_alloc<float, sj::alloc>::allocate(32);
    float *q = sj::simple_alloc<float, sj::alloc>::allocate(64);
    sj::simple_alloc<float, sj::alloc>::deallocate(p, 32);
    sj::simple_alloc<float, sj::alloc>::deallocate(q, 64);

}