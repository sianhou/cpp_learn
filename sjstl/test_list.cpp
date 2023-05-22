//
// Created by sianh on 2023/5/22.
//

#include "sjlist.h"

#include "iostream"

int main() {

    sj::List<int> ilist;
    std::cout << "size = " << ilist.size() << std::endl;

    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_back(3);
    ilist.push_back(4);
    std::cout << "size = " << ilist.size() << std::endl;
}