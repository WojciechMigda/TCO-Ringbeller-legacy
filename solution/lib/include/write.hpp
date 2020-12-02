#pragma once

#ifndef LIB_INCLUDE_WRITE_HPP_
#define LIB_INCLUDE_WRITE_HPP_

#include "message.hpp"

#include "asio_types.hpp"
#include "logging.hpp"

#include "spdlog/spdlog.h"
#include "fmt/format.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>

#include <cstddef>
#include <string>


namespace Ringbeller
{


template<
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body
>
std::string to_string(request<isSet, isXTest, isXRead, isXWrite, isXExec, Body> const & msg)
{
    std::string rv("AT");

    if (isSet)
    {
        rv += fmt::format("S{}={}", msg.command, msg.args);
    }
    else if (isXTest)
    {
        rv += fmt::format("+{}=?", msg.command);
    }
    else if (isXRead)
    {
        rv += fmt::format("+{}?", msg.command);
    }
    else if (isXWrite)
    {
        rv += fmt::format("+{}={}", msg.command, msg.args);
    }
    else if (isXExec)
    {
        rv += fmt::format("+{}", msg.command);
    }
    else
    {
        rv += fmt::format("{}{}", msg.command, msg.args);
    }

    spdlog::trace("to_string serialized message to {}", rv);

    rv += "\r\n";

    return rv;
}


/**
 * Write a complete message to a stream.
 *
 * This function is used to write a complete message to a stream using
 * AT Commands. The call will block until one of the following conditions is true:
 *
 *  @li The entire message is written.
 *
 *  @li An error occurs.
 *
 *  @param stream The stream to which the data is to be written.
 *
 *  @param msg The message to write.
 *
 *  @param ec Set to the error, if any occurred.
 *
 *  @return The number of bytes written to the stream.
 *
 */
template<
    typename SyncWriteStream,
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body
>
std::size_t
write(SyncWriteStream & stream, request<isSet, isXTest, isXRead, isXWrite, isXExec, Body> const & msg, ec_t & ec)
{
    FN_ENTER();

    std::string s = to_string(msg);

    auto rv = boost::asio::write(stream, boost::asio::buffer(s, s.size()), ec);

    LOG_CHECK_EC(ec);

    FN_LEAVE();

    return rv;
}


/**
 * Write a complete message to a stream.
 *
 * This function is used to write a complete message to a stream using
 * AT Commands. The call will block until one of the following conditions is true:
 *
 *  @li The entire message is written.
 *
 *  @li An error occurs.
 *
 *  @param stream The stream to which the data is to be written.
 *
 *  @param msg The message to write.
 *
 *  @return The number of bytes written to the stream.
 *
 *  @throws system_error Thrown on failure.
 *
 */
template<
    typename SyncWriteStream,
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body
>
std::size_t
write(SyncWriteStream & stream, request<isSet, isXTest, isXRead, isXWrite, isXExec, Body> const & msg)
{
    FN_ENTER();

    ec_t ec;

    auto const bytes_transferred = write(stream, msg, ec);

    if (ec)
    {
        LOG_EXCEPTION(ec);
        BOOST_THROW_EXCEPTION(boost::system::system_error{ec});
    }

    FN_LEAVE();

    return bytes_transferred;
}


}  // namespace Ringbeller



#endif /* LIB_INCLUDE_WRITE_HPP_ */
