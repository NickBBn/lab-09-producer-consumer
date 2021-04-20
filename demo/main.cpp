#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#include "boost/program_options.hpp"
#include "gumbo.h"
#include "downloader.hpp"
#include "parser.hpp"
#include "threadpool.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv) {
  po::options_description desc("Options");
  desc.add_options()("url", po::value<std::string>(), "адрес HTML страницы")(
      "depth", po::value<size_t>(), "глубина поиска по странице")(
      "network_threads", po::value<size_t>(),
      "количество потоков для скачивания страниц")(
      "parser_threads", po::value<size_t>(),
      "количество потоков для обработки страниц")(
      "output", po::value<std::string>(), "путь до выходного файл")(
      "help", "вспомогательное сообщение");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  std::string link{};
  size_t depth;
  size_t network_threads;
  size_t parser_threads;
  std::string output{};

  link = (vm.count("url")) ?
        vm["url"].as<std::string>() : "https://github.com/";

  depth = (vm.count("depth")) ? vm["depth"].as<size_t>() : 1;

  network_threads = (vm.count("network_threads")) ?
                    vm["network_threads"].as<size_t>() : 1;

  parser_threads = (vm.count("parser_threads")) ?
                   vm["parser_threads"].as<size_t>() : 1;

  output = (vm.count("output")) ?
           vm["output"].as<std::string>() : "output.txt";

  struct url cur_url(link, depth);
  downloader::links.push(std::move(cur_url));

  ThreadPool pool_download(network_threads);
  ThreadPool pool_parse(parser_threads);
  while ((!parser::queue_pages.is_empty()) ||
         (!downloader::links.is_empty())){
      pool_download.enqueue(downloader::download_page);
      pool_parse.enqueue(parser::parse);
  }

  std::ofstream ofs(output);
  while (!parser::queue_writer.is_empty()) {
    std::string _tmp = parser::queue_writer.front();
    parser::queue_writer.pop();
    ofs << _tmp << std::endl;
  }
  ofs.close();
  return 0;
}
