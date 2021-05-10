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
  static bool isImage(const std::string& url);
  static void search_for_links(GumboNode* node, const page& p);
  static safe_queue<page> queue_pages; //очередь для парсинга
  static safe_queue<std::string> queue_writer; //очередь для записи в файл
  static std::atomic_int current_works;
};
#endif  // INCLUDE_PARSER_HPP_
