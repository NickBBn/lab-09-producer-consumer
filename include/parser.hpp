//Copyright [2021] <nickgeo.winner@gmail.com>

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_
#include <gumbo.h>
#include <iostream>
#include <regex>
#include <string>

#include "downloader.hpp"
#include "safe_queue.hpp"
#include "url.hpp"
#include "page.hpp"

class parser {
 public:
  parser() = delete;
  static void parse();
  static safe_queue<page> queue_pages;
  static safe_queue<std::string> queue_writer;
  static std::atomic_int current_works;
};
#endif  // INCLUDE_PARSER_HPP_
