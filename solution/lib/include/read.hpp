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
#include <utility>


namespace Ringbeller
{


namespace detail
{


template<
    typename DynamicBuffer,
    typename Body,
    typename Sequence>
void populate_response_from_buffer(
    DynamicBuffer const & buffer,
    response<Body, Sequence> & msg)
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


}


/**
 *  Read a complete message from a stream.
 *
 *  This function is used to read a complete message from a stream.
 *  The call will block until one of the following conditions is true:
 *
 *  @li The entire message is read.
 *
 *  @li An error occurs.
 *
 *  @param stream The stream from which the data is to be read.
 *  The type must support the @b SyncReadStream concept.
 *
 *  @param buffer A @b DynamicBuffer holding additional bytes
 *  read by the implementation from the stream. This is both
 *  an input and an output parameter; on entry, any data in the
 *  dynamic buffer's input sequence will be given to the parser
 *  first.
 *
 *  @param msg An object in which to store the message contents.
 *  This object should not have previous contents, otherwise
 *  the behavior is undefined.
 *
 *  @param ec Set to the error, if any occurred.
 *
 *  @return The number of bytes transferred to the parser.
 */
template<
    typename SyncReadStream,
    typename DynamicBuffer,
    typename Body,
    typename Sequence>
std::size_t
read(SyncReadStream & stream, DynamicBuffer & buffer, response<Body, Sequence> & msg, ec_t & ec)
{
    FN_ENTER();

    std::size_t bytes_read = boost::asio::read_until(stream, buffer, detail::match_condition, ec);

    buffer.commit(bytes_read);

    LOG_CHECK_EC(ec);

    if (not ec)
    {
        detail::populate_response_from_buffer(buffer, msg);
    }

    buffer.consume(bytes_read);

    FN_LEAVE();

    return bytes_read;
}


/**
 *  Read a complete message from a stream.
 *
 *  This function is used to read a complete message from a stream.
 *  The call will block until one of the following conditions is true:
 *
 *  @li The entire message is read.
 *
 *  @li An error occurs.
 *
 *  @param stream The stream from which the data is to be read.
 *  The type must support the @b SyncReadStream concept.
 *
 *  @param buffer A @b DynamicBuffer holding additional bytes
 *  read by the implementation from the stream. This is both
 *  an input and an output parameter; on entry, any data in the
 *  dynamic buffer's input sequence will be given to the parser
 *  first.
 *
 *  @param msg An object in which to store the message contents.
 *  This object should not have previous contents, otherwise
 *  the behavior is undefined.
 *
 *  @return The number of bytes transferred to the parser.
 *
 *  @throws system_error Thrown on failure.
 */
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


template<
    typename AsyncReadStream,
    typename DynamicBuffer,
    typename Body,
    typename Sequence,
    typename ReadHandler>
BOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
    void(boost::system::error_code, std::size_t))
async_read(
    AsyncReadStream & stream,
    DynamicBuffer & buffer,
    response<Body, Sequence> & msg,
    ReadHandler && handler)
{
    FN_ENTER();

    boost::asio::async_read_until(stream, buffer, detail::match_condition,
        [handler{std::move(handler)}, &buffer, &msg](boost::system::error_code const & ec, std::size_t rb)
        {
            FN_ENTER();

            LOG_CHECK_EC(ec);

            buffer.commit(rb);

            if (not ec)
            {
                detail::populate_response_from_buffer(buffer, msg);
            }

            // call user handler
            handler(ec, rb);

            buffer.consume(rb);

            FN_LEAVE();
        }
    );

    FN_LEAVE();
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_AT_READ_HPP_ */
