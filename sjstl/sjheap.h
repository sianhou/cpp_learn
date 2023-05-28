//
// Created by 侯思安 on 2023/5/28.
//

#ifndef LEARN_CPP_SJSTL_SJHEAP_H_
#define LEARN_CPP_SJSTL_SJHEAP_H_

#include "sjiterator.h"

namespace sj {

template<class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
  // find the father node
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex > topIndex && *(first + parent) < value) {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}

template<class RandomAccessIterator, class Distance, class T>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance *, T *) {
  __push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template<class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
  __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template<class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
  Distance topIndex = holeIndex;
  Distance secondChild = 2 * holeIndex + 2;
  while (secondChild < len) {
    if (*(first + secondChild) < *(first + secondChild - 1)) {
      secondChild--;
    }
    *(first + holeIndex) = *(first + secondChild);
    holeIndex = secondChild;
    secondChild = 2 * (secondChild + 1);

  }
  if (secondChild == len) {
    *(first + holeIndex) = *(first + secondChild - 1);
    holeIndex = secondChild - 1;
  }
  __push_heap(first, holeIndex, topIndex, value);
}

}

#endif //LEARN_CPP_SJSTL_SJHEAP_H_
