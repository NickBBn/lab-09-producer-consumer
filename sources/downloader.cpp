//
// Created by niickson on 4/16/21.
//

#include "downloader.hpp"

safe_queue<url> downloader::links;
std::atomic_uint16_t downloader::_current_works(0);

void downloader::download_page() {
  if (!downloader::links.is_empty())
  {
    ++_current_works;
    url cur_url = downloader::links.front();
    std::cout << "download" << std::endl;
    page cur_page;
    cur_page.depth = cur_url.depth;
    parse_uri(cur_page, cur_url);
    if (cur_page.protocol == "http")
      download_http_page(cur_page);
    else if (cur_page.protocol == "https")
      download_https_page(cur_page);
    parser::queue_pages.push(std::move(cur_page));
    downloader::links.pop();
    --_current_works;
  }
}


void downloader::download_https_page(page &cur_page) {
  boost::asio::io_context ioc{};

  boost::asio::ssl::context ctx(boost::asio::ssl::context::tls_client);
  ctx.set_default_verify_paths();
  ctx.add_verify_path("/etc/ssl/certs/");
  ctx.set_verify_mode(boost::asio::ssl::verify_peer);

  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream(ioc, ctx);

  if (!SSL_set_tlsext_host_name(stream.native_handle(), cur_page.host.data())) {
    boost::beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                boost::asio::error::get_ssl_category()};
    throw boost::beast::system_error{ec};
  }

  auto const results = resolver.resolve(cur_page.host, "443");

  boost::beast::get_lowest_layer(stream).connect(results);

  stream.handshake(boost::asio::ssl::stream_base::client);

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, cur_page.target, 11};
  req.set(boost::beast::http::field::host, cur_page.host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.shutdown(ec);

  cur_page.page = res.body();
}

void downloader::download_http_page(page &cur_page) {
  boost::asio::io_context ioc{};

  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::beast::tcp_stream stream(ioc);

  auto const results = resolver.resolve(cur_page.host, "80");

  stream.connect(results);
  stream.expires_after(std::chrono::seconds(3));

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, cur_page.target, 11};
  req.set(boost::beast::http::field::host, cur_page.host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  cur_page.page = res.body();
}

void downloader::parse_uri(page &cur_page, url &cur_url) {
  size_t dd_pos = cur_url.link.find_first_of(':');
  cur_page.protocol = cur_url.link.substr(0, dd_pos);
  size_t path_start = cur_url.link.find('/', dd_pos + 3);
  if (path_start == std::string::npos) {
    cur_page.host = cur_url.link.substr(dd_pos + 3, cur_url.link.size() - dd_pos - 2);
    cur_page.target = "/";
  } else {
    cur_page.host = cur_url.link.substr(dd_pos + 3, path_start - dd_pos - 3);
    cur_page.target = cur_url.link.substr(path_start, cur_url.link.size() - path_start + 1);
  }
}

std::uint16_t downloader::current_works() { return _current_works; }
