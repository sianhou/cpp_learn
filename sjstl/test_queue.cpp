//
// Created by 侯思安 on 2023/5/28.
//

#include "sjquue.h"

int main() {
  sj::Queue<int> iqueue;
  iqueue.push(1);
  iqueue.push(3);
  iqueue.push(5);
  iqueue.push(7);

  std::cout << iqueue.size() << std::endl;

  std::cout << iqueue.front() << std::endl;
  iqueue.pop();
  std::cout << iqueue.front() << std::endl;
  iqueue.pop();
  std::cout << iqueue.front() << std::endl;
  iqueue.pop();
  std::cout << iqueue.front() << std::endl;
  iqueue.pop();

  std::cout << iqueue.size() << std::endl;
}