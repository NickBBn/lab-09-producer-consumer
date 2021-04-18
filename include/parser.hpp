//
// Created by niickson on 4/16/21.
//

#ifndef TEMPLATE_PARSER_HPP
#define TEMPLATE_PARSER_HPP
#include <gumbo.h>
#include <iostream>
#include <regex>
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
  static std::uint16_t current_works();

 private:
  static std::atomic_uint16_t _current_works;
};
#endif  // TEMPLATE_PARSER_HPP
