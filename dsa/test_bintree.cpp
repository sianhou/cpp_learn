//
// Created by sianh on 2023/6/18.
//

#include "bintree.h"
#include "iostream"

void visit(const int &data) {
    std::cout << data << std::endl;
}

int main() {
    sj::BinNode<int> root(0);
    root.insertAsLC(1);
    root.insertAsRC(2);

    sj::travPre_R(&root, visit);
    sj::travIn_R(&root, visit);
    sj::travPost_R(&root, visit);

    return 0;
}