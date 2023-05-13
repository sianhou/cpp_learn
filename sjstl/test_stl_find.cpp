//
// Created by sianh on 2023/5/13.
//

#include "vector"
#include "list"
#include "deque"
#include "algorithm"
#include "iostream"

using namespace std;

int main() {

    const int array_size = 7;
    int ia[array_size] = {0, 1, 2, 3, 4, 5, 6};

    vector<int> ivect(ia, ia + 7);
    list<int> ilist(ia, ia + 7);
    deque<int> ideque(ia, ia + 7);

    vector<int>::iterator it1 = find(ivect.begin(), ivect.end(), 4);
    if (it1 != ivect.end()) {
        std::cout << " 4 found in vector" << *it1 << std::endl;
    }

    list<int>::iterator it2 = find(ilist.begin(), ilist.end(), 4);
    if (it2 != ilist.end()) {
        std::cout << " 4 found in list" << *it2 << std::endl;
    }

    deque<int>::iterator it3 = find(ideque.begin(), ideque.end(), 4);
    if (it3 != ideque.end()) {
        std::cout << " 4 found in deque" << *it3 << std::endl;
    }

    return 0;
}