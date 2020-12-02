#pragma once

#ifndef LIB_INCLUDE_PARSER_HPP_
#define LIB_INCLUDE_PARSER_HPP_

#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/buffer.hpp>

#include <utility>


using asio_buf_iterator = boost::asio::buffers_iterator<boost::asio::const_buffers_1>;


std::pair<asio_buf_iterator, bool>
match_condition(asio_buf_iterator begin, asio_buf_iterator end);



#endif /* LIB_INCLUDE_PARSER_HPP_ */
