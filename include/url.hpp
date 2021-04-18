//
// Created by niickson on 4/18/21.
//

#ifndef TEMPLATE_URL_HPP
#define TEMPLATE_URL_HPP
#include <iostream>

struct url {
  std::string link;
  size_t depth;
  url(const std::string& p_link, const size_t& p_depth);
};

#endif  // TEMPLATE_URL_HPP
