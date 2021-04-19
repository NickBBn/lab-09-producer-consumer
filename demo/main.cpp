#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

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
  while ((!parser::queue_pages.is_empty()) ||
         (!downloader::links.is_empty())){
      pool_download.enqueue(downloader::download_page);
      pool_parse.enqueue(parser::parse);
  }

  std::ofstream ofs("output.txt");
  while (!parser::queue_writer.is_empty()) {
    std::string _tmp = parser::queue_writer.front();
    parser::queue_writer.pop();
    ofs << _tmp << std::endl;
  }
  ofs.close();
  return 0;
}
