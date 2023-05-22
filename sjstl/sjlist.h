//
// Created by sianh on 2023/5/22.
//

#ifndef LEARN_CPP_SJSTL_SJLIST_H_
#define LEARN_CPP_SJSTL_SJLIST_H_

#include "sjiterator.h"
#include "sjalloc.h"
#include "new"

namespace sj {
template<class T>
struct __list_node {
    typedef void *void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template<class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T &, T *> iterator;
    typedef __list_iterator<T, Ref, Ptr> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T> *link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node;

    //construct
    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator &x) : node(x.node) {}

    bool operator==(const self &x) const { return node == x.node; }
    bool operator!=(const self &x) const { return node != x.node; }

    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }

    self &operator++() {
        node = (link_type)((*node).next);
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self &operator--() {
        node = (link_type)((*node).prev);
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template<class T, class Alloc=alloc>
class List {
 protected:
    typedef __list_node<T> list_node;
    typedef simple_alloc<list_node, Alloc> list_node_allocator;
 public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef list_node *link_type;
    typedef const list_node *const_link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
 public:
    typedef __list_iterator<T, T &, T *> iterator;
    typedef __list_iterator<T, const T &, const T *> const_iterator;
 protected:
    link_type node;
 public:
    iterator begin() { return (link_type)((*node).next); }
    iterator end() { return node; }
    const_iterator cbegin() { return static_cast<const_link_type>(node->next); }
    const_iterator cend() { return static_cast<const_link_type>(node); }
    bool empty() { return node->next == node; }
    size_type size() {
        //TODO(housian):
        return std::distance(begin(), end());
    }
    reference front() {
        return *begin();
    }
    reference back() {
        return *(--end());
    }
 protected:
    link_type get_node() { return list_node_allocator::allocate(); }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }
    link_type create_node(const T &x) {
        link_type p = get_node();
        _construct(&p->data, x);
        return p;
    }
    void destroy_node(link_type p) {
        _destroy(&p->data);
        put_node(p);
    }

    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
 public:
    List() { empty_initialize(); }

    iterator insert(iterator position, const T &x) {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;

        (link_type(position.node->prev))->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }

    iterator erase(iterator position) {
        link_type next_node = link_type(position.node->next);
        link_type prev_node = link_type(position.node->prev);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.node);
        return iterator(next_node);
    }

    void push_front(const T &x) { insert(begin(), x); }
    void push_back(const T &x) { insert(end(), x); }

    void pop_front() { erase(begin()); }
    void pop_back() {
        iterator tmp = end();
        erase(--tmp);
    }

    void clear() {
        link_type cur = (link_type)node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = (link_type)cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    void remove(const T &x) {
        auto first = begin();
        auto last = end();
        while (first != last) {
            iterator next = first;
            ++next;
            if (*first == x) erase(first);
            first = next;
        }
    }

    void unique() {
        auto first = begin();
        auto last = end();
        if (first == last) return;
        auto next = first;
        while (++next != last) {
            if (*first == *next) {
                erase(first);
            } else {
                first = next;
            }
            next = first;
        }
    }
 protected:
    void transfer(iterator position, iterator first, iterator last) {
        if (position != last) {
            (*(link_type((*last.node).prev))).next = position.node;
            (*(link_type((*first.node).prev))).next = last.node;
            (*(link_type((*position.node).prev))).next = first.node;
            link_type tmp = link_type((*position.node).prev);
            (*position.node).prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = tmp;
        }
    }
 public:
    void splice(iterator position, List &x) {
        if (!x.empty()) {
            transfer(position, x.begin(), x.end());
        }
    }

    void splice(iterator position, List &, iterator i) {
        iterator j = i;
        ++j;
        if (position == i || position == j) return;
        transfer(position, i, j);
    }

    void splice(iterator position, List &, iterator first, iterator last) {
        if (first != last) {
            transfer(position, first, last);
        }
    }

    void merge(List &x) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            } else {
                ++first1;
            }
        }
        if (first2 != last2) {
            transfer(last1, first2, last2);
        }
    }

    void reverse() {
        if (node->next == node || link_type(node->next)->next == node) { return; }

        iterator first = begin();
        ++first;
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }
//TODO(HOUSIAN)
    void sort() {
        if (node->next == node || link_type(node->next)->next == node) { return; }

        List carry;
        List counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);

            }
        }
    }
};
}

#endif //LEARN_CPP_SJSTL_SJLIST_H_
