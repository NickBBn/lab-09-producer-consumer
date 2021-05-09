//
// Created by niickson on 5/10/21.
//

#ifndef TEMPLATE_APPLICATION_HPP
#define TEMPLATE_APPLICATION_HPP
#include <iostream>
#include <string>
#include <fstream>

#include "boost/program_options.hpp"
#include "gumbo.h"
#include "downloader.hpp"
#include "parser.hpp"
#include "threadpool.hpp"

class application {
 public:
  application(std::string _link,size_t _depth,size_t _network_threads,
              size_t _parser_threads, std::string _output);
  void work();

 private:
  std::string link{};
  size_t depth;
  size_t network_threads;
  size_t parser_threads;
  std::string output{};
};

#endif  // TEMPLATE_APPLICATION_HPP
