//
// Created by 侯思安 on 2023/5/28.
//
#include "sjdeque.h"
#include "iostream"

int main() {
  sj::Deque<int, sj::alloc, 8> ideq(20, 9);
  std::cout << ideq.size() << std::endl;

  for (auto i = 0; i < ideq.size(); ++i) {
    ideq[i] = i;
  }

  for (auto i = 0; i < ideq.size(); ++i) {
    std::cout << ideq[i] << std::endl;
  }
  std::cout << std::endl;

  for (int i = 0; i < 3; ++i) {
    ideq.push_back(i);
  }
  for (auto i = 0; i < ideq.size(); ++i) {
    std::cout << ideq[i] << std::endl;
  }
  std::cout << ideq.size() << std::endl;
  std::cout << std::endl;

  ideq.push_front(99);
  ideq.push_front(98);
  ideq.push_front(97);
  for (auto i = 0; i < ideq.size(); ++i) {
    std::cout << ideq[i] << std::endl;
  }
  std::cout << ideq.size() << std::endl;
  std::cout << std::endl;

  sj::Deque<int, sj::alloc, 8>::iterator it;
  it = std::find(ideq.begin(), ideq.end(), 99);
  std::cout << *it << std::endl;
  std::cout << *(it.cur) << std::endl;
}