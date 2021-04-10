#include <cstdlib>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "gumbo.h"

namespace ssl = boost::asio::ssl;

void parseUri(std::string_view url, std::string& host, std::string& target) {
  size_t dd_pos = url.find(':');
  size_t path_start = url.find('/', dd_pos + 3);
  if (path_start == std::string::npos) {
    host = url.substr(dd_pos + 3, url.size() - dd_pos - 2);
    target = "/";
  } else {
    host = url.substr(dd_pos + 3, path_start - dd_pos - 3);
    target = url.substr(path_start, url.size() - path_start + 1);
  }
}

std::string downloadPage(std::string_view url) {
  boost::asio::io_context ioc{};


  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::beast::tcp_stream stream(ioc);

  std::string host{};
  std::string target{};
  parseUri(url, host, target);

  auto const results = resolver.resolve(host, "80");

  stream.connect(results);

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, target, 11};
  req.set(boost::beast::http::field::host, host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  std::cout << res.body();

  return res.body();
}

static void search_for_links(GumboNode* node) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    std::cout << href->value << std::endl;
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]));
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: find_links <html filename>.\n";
    exit(EXIT_FAILURE);
  }
  const char* url = argv[1];
  std::string res = downloadPage(url);
  std::cout << "Wow we got here"<< std::endl;

  GumboOutput* output = gumbo_parse(res.c_str());
  search_for_links(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);


  /*
  std::ifstream in(res, std::ios::in | std::ios::binary);
  if (!in) {
    std::cout << "File not found!\n";
    exit(EXIT_FAILURE);
  }

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput* output = gumbo_parse(contents.c_str());
  search_for_links(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  */

}