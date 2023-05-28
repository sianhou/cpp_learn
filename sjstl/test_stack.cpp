//
// Created by 侯思安 on 2023/5/28.
//

#include "sjstack.h"

int main() {
  sj::Stack<int> istack;
  istack.push(1);
  istack.push(3);
  istack.push(5);
  istack.push(7);

  std::cout << istack.size() << std::endl;

  std::cout << istack.top() << std::endl;
  istack.pop();
  std::cout << istack.top() << std::endl;
  istack.pop();
  std::cout << istack.top() << std::endl;
  istack.pop();
  std::cout << istack.top() << std::endl;
  istack.pop();

  std::cout << istack.size() << std::endl;
}