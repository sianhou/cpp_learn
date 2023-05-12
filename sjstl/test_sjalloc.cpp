//
// Created by dell on 2023/5/12.
//
#include "sjalloc.h"
#include "vector"

int main() {

    // test alloc
    int ia[5] = {0, 1, 2, 3, 4};
    size_t i;

    std::vector<int, sj::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < iv.size(); ++i) {
        std::cout << iv[i] << " ";
    }
    std::cout << std::endl;

    // test malloc_alloc
    sj::malloc_alloc malloc_alloc;

    auto handler = []() {
      std::cout << "allocate failed!" << std::endl;
      exit(EXIT_FAILURE);
    };
    malloc_alloc.set_malloc_handler(handler);

    i = 1024;
    void *p = malloc_alloc.allocate(i * sizeof(int));
    do {
        i *= 2;
        std::cout << "allocate " << i * sizeof(int) / (8 * 1024.0f * 1024.0f * 1024.0f) << "GBs!" << std::endl;
        p = malloc_alloc.reallocate(p, i / 2, i);
    } while (i);

}