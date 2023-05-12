//
// Created by dell on 2023/5/12.
//
#include "sjalloc.h"
#include "vector"

int main() {

    int ia[5] = {0, 1, 2, 3, 4};
    unsigned int i;

    std::vector<int, sj::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < iv.size(); ++i) {
        std::cout << iv[i] << " ";
    }
    std::cout << std::endl;

}