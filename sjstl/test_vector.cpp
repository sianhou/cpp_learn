//
// Created by 侯思安 on 2023/5/20.
//
#include "sjvector.h"
#include "vector"
#include "iostream"
#include "algorithm"

class Shape {
  int n3, n2, n1;
};

int main() {

  int i;
  sj::Vector<int> iv(2, 9);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.push_back(1);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.push_back(2);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.push_back(3);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.push_back(4);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.push_back(5);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.display();

  iv.pop_back();
  iv.pop_back();
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  iv.pop_back();
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;

  sj::Vector<int>::iterator ivite = std::find(iv.begin(), iv.end(), 1);
  if (ivite != iv.end()) iv.erase(ivite);
  std::cout << "size = " << iv.size() << std::endl;
  std::cout << "capacity = " << iv.capacity() << std::endl;
  iv.display();

  ivite = std::find(iv.begin(), iv.end(), 2);

}
