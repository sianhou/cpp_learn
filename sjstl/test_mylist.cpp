#include "3mylist_iter.h"

template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &value) {
    while (first != last && *first != value) {
        ++first;
    }
    return first;
}

int main() {
    sj::List<int> mylist;
    for (int i = 0; i < 5; ++i) {
        mylist.insert_front(i);
        mylist.insert_end(i + 2);
    }
    mylist.display();

    sj::ListIter<sj::ListItem<int>> begin(mylist.front());
    sj::ListIter<sj::ListItem<int>> end(mylist.end());
    sj::ListIter<sj::ListItem<int>> it;

    it = find(begin, end, 3);
    if (it != end) {
        std::cout << "found " << it->value() << std::endl;
    } else {
        std::cout << "dont found" << std::endl;
    }

    it = find(begin, end, 7);
    if (it != end) {
        std::cout << "found " << it->value() << std::endl;
    } else {
        std::cout << "dont found" << std::endl;
    }

}