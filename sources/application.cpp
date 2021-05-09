//
// Created by niickson on 5/10/21.
//

#include "application.hpp"

application::application(std::string _link, size_t _depth, size_t _network_threads,
                         size_t _parser_threads, std::string _output)
    :   link(std::move(_link))
      , depth(_depth)
      , network_threads(_network_threads)
      , parser_threads(_parser_threads)
      , output(std::move(_output))
{}
void application::work(){
  struct url cur_url(link, depth);
  downloader::links.push(std::move(cur_url));

  ThreadPool pool_download(network_threads);
  ThreadPool pool_parse(parser_threads);
  while ((!parser::queue_pages.is_empty()) ||
         (!downloader::links.is_empty()) ||
         (downloader::current_works > 0) ||
         (parser::current_works > 0)
      ){
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
}
