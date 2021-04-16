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
  std::string_view url = argv[1];
  downloader d;
  std::string res = d.download_page(url);

  GumboOutput* output = gumbo_parse(res.c_str());
  parser p;
  p.search_for_links(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);

  ThreadPool pool_download(4);
  ThreadPool pool_parse(4);


}
