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
#include <boost/asio/async_result.hpp>
#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>

#include <cstddef>
#include <string>


namespace Ringbeller
{


namespace detail
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


} // detail


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

    std::string s = detail::to_string(msg);

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


/**
 * This function is used to asynchronously write a complete message
 * to a stream. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li All of the data in the supplied request has been written.
 *
 * @li An error occurred.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the AsyncWriteStream concept.
 *
 * @param b A request object from which data will be written. Ownership
 * of the request is retained by the caller, which must guarantee that it
 * remains valid until the handler is called.
 *
 * @param handler The handler to be called when the write operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   const boost::system::error_code& error, // Result of operation.
 *
 *   std::size_t bytes_transferred           // Number of bytes written from the
 *                                           // buffers. If an error occurred,
 *                                           // this will be less than the sum
 *                                           // of the buffer sizes.
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * boost::asio::io_service::post().
 */
template<
    typename AsyncWriteStream,
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body,
    typename WriteHandler
>
BOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
    void (boost::system::error_code, std::size_t))
async_write(
    AsyncWriteStream & stream,
    request<isSet, isXTest, isXRead, isXWrite, isXExec, Body> const & msg,
    WriteHandler && handler)
{
    FN_ENTER();

    std::string s = detail::to_string(msg);

    boost::asio::async_write(
        stream, boost::asio::buffer(s, s.size()), std::move(handler));

    FN_LEAVE();
}


/**
 * This function is used to asynchronously write a certain number of bytes of
 * data to a stream. The function call always returns immediately. The
 * asynchronous operation will continue until one of the following conditions
 * is true:
 *
 * @li All of the data in the supplied request has been written.
 *
 * @li An error occurred.
 *
 * @param s The stream to which the data is to be written. The type must support
 * the AsyncWriteStream concept.
 *
 * @param b A string object from which data will be written. Ownership
 * of the object is retained by the caller, which must guarantee that it
 * remains valid until the handler is called.
 *
 * @param handler The handler to be called when the write operation completes.
 * Copies will be made of the handler as required. The function signature of the
 * handler must be:
 * @code void handler(
 *   const boost::system::error_code& error, // Result of operation.
 *
 *   std::size_t bytes_transferred           // Number of bytes written from the
 *                                           // buffers. If an error occurred,
 *                                           // this will be less than the sum
 *                                           // of the buffer sizes.
 * ); @endcode
 * Regardless of whether the asynchronous operation completes immediately or
 * not, the handler will not be invoked from within this function. Invocation of
 * the handler will be performed in a manner equivalent to using
 * boost::asio::io_service::post().
 */
template<
    typename AsyncWriteStream,
    typename WriteHandler
>
BOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
    void (boost::system::error_code, std::size_t))
async_write_text(
    AsyncWriteStream & stream,
    std::string const & text,
    WriteHandler && handler)
{
    FN_ENTER();

    std::string s = fmt::format("{}\032", text);

    boost::asio::async_write(
        stream, boost::asio::buffer(s, s.size()), std::move(handler));

    FN_LEAVE();
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_WRITE_HPP_ */
