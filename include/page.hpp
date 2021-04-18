//
// Created by niickson on 4/18/21.
//

#ifndef TEMPLATE_PAGE_HPP
#define TEMPLATE_PAGE_HPP
#include <iostream>

struct page {
  std::string page;
  std::string protocol;
  std::string host;
  std::string target;
  size_t depth;
};

#endif  // TEMPLATE_PAGE_HPP
