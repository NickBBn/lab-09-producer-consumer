//Copyright [2021] <nickgeo.winner@gmail.com>

#ifndef INCLUDE_URL_HPP_
#define INCLUDE_URL_HPP_
#include <iostream>
#include <string>

struct url {
  std::string link;
  size_t depth;

  url(const std::string& p_link, const size_t& p_depth)
      :
        link(p_link)
      , depth(p_depth)
  {}
};

#endif  // INCLUDE_URL_HPP_
