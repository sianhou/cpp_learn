//
// Created by sianh on 2023/5/13.
//

#ifndef LEARN_CPP_SJSTL_3MYLIST_ITER_H_
#define LEARN_CPP_SJSTL_3MYLIST_ITER_H_

#include "3mylist.h"

namespace sj {
template<class Item>
struct ListIter {
    Item *ptr;
    ListIter(Item *p = nullptr) : ptr(p) {}

    Item &operator*() const { return *ptr; }
    Item *operator->() const { return ptr; }

    ListIter &operator++() {
        ptr = ptr->next();
        return *this;
    }

    ListIter operator++(int) {
        ListIter tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const ListIter &other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const ListIter &other) const {
        return ptr != other.ptr;
    }
};

template<class T>
bool operator!=(const ListItem<T> &item, const T &t) {
    return item.value() != t;
}
}

#endif //LEARN_CPP_SJSTL_3MYLIST_ITER_H_
