#pragma once

#ifndef LIB_INCLUDE_AT_READ_HPP_
#define LIB_INCLUDE_AT_READ_HPP_

#include "parser.hpp"

#include "asio_types.hpp"
#include "logging.hpp"

#include "spdlog/spdlog.h"

#include <boost/asio/read_until.hpp>
#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>

#include <cstddef>
#include <string>
#include <iterator>
#include <algorithm>


namespace Ringbeller
{


template<
    typename SyncReadStream,
    typename DynamicBuffer,
    typename Body,
    typename Sequence>
std::size_t
read(SyncReadStream & stream, DynamicBuffer & buffer, response<Body, Sequence> & msg, ec_t & ec)
{
    FN_ENTER();

    std::size_t bytes_read = boost::asio::read_until(stream, buffer, match_condition, ec);

    buffer.commit(bytes_read);

    LOG_CHECK_EC(ec);

    if (not ec)
    {
        auto b = buffer.data();

        result_code rc(result_code::none);
        std::string rc_text;
        std::vector<std::string> body;

        parse_response(
            boost::asio::buffers_begin(b),
            boost::asio::buffers_end(b),
            rc,
            rc_text,
            body);

        msg.result_code = rc;

        std::copy(rc_text.cbegin(), rc_text.cend(), std::back_inserter(msg.rc_text));
        for (auto const & ll : body)
        {
            auto & seq = msg.body;
            seq.emplace(seq.end(), ll.cbegin(), ll.cend());
        }
    }

    buffer.consume(bytes_read);

    FN_LEAVE();

    return bytes_read;
}


template<
    typename SyncReadStream,
    typename DynamicBuffer,
    typename Body,
    typename Sequence>
std::size_t
read(SyncReadStream & stream, DynamicBuffer & buffer, response<Body, Sequence> & msg)
{
    FN_ENTER();

    ec_t ec;

    auto const bytes_transferred = read(stream, buffer, msg, ec);

    if (ec)
    {
        LOG_EXCEPTION(ec);
        BOOST_THROW_EXCEPTION(boost::system::system_error{ec});
    }

    FN_LEAVE();

    return bytes_transferred;

}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_AT_READ_HPP_ */
