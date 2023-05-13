//
// Created by sianh on 2023/5/13.
//

#ifndef LEARN_CPP_SJSTL_SJCONSTRUCT_H_
#define LEARN_CPP_SJSTL_SJCONSTRUCT_H_

#include "new"

namespace sj {
template<class T1, class T2>
inline void construct(T1 *p, const T2 &value) {
    new(p) T1(value);
}

template<class T>
inline void destroy(T *pointer) {
    pointer->~T();
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {

}

template<class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, std::false_type) {
    for (; first < last; ++first) {
        destroy(&*first);
    }
}

template<class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, std::true_type) {}

inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *) {}

}

#endif //LEARN_CPP_SJSTL_SJCONSTRUCT_H_
