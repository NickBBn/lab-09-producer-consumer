//
// Created by niickson on 4/16/21.
//

#ifndef TEMPLATE_DOWNLOADER_HPP
#define TEMPLATE_DOWNLOADER_HPP
#include <openssl/ssl.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/beast/version.hpp>
#include <iostream>

#include "safe_queue.hpp"

struct url {

};

class downloader {
 public:
  std::string download_page(std::string_view url);
  void parseUri(std::string_view url,
                std::string& protocol, std::string& host, std::string& target);

 private:
  std::string download_https_page(std::string&& host,
                                std::string&& target);
  std::string download_http_page(std::string&& host,
                                std::string&& target);

 private:
  static safe_queue<url> links;
};

#endif  // TEMPLATE_DOWNLOADER_HPP
