#include <cstdlib>
#include <iostream>
#include <string>

#include "gumbo.h"
#include "downloader.hpp"
#include "parser.hpp"
#include "threadpool.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: find_links <html filename>.\n";
    exit(EXIT_FAILURE);
  }
  url cur_url(argv[1], 1);
  downloader::links.push(std::move(cur_url));

  ThreadPool pool_download(4);
  ThreadPool pool_parse(4);
  while (parser::current_works() != 0 ||
         downloader::current_works() != 0 ||
         (!parser::queue_pages.is_empty()) ||
         (!downloader::links.is_empty())
         ){
      pool_download.enqueue(downloader::download_page);
      pool_parse.enqueue(parser::parse);
  }
  return 0;
}
