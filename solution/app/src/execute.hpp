#pragma once

#ifndef APP_SRC_EXECUTE_HPP_
#define APP_SRC_EXECUTE_HPP_

#include "message.hpp"
#include "write.hpp"
#include "read.hpp"

#include "spdlog/spdlog.h"
#include "fmt/core.h"

#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>

#include <string>

/*
 * Utility to synchronously execute specified command
 */
template<
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body
>
int execute(
    std::string const & device,
    boost::asio::serial_port & sp,
    Ringbeller::request<isSet, isXTest, isXRead, isXWrite, isXExec, Body> const & command,
    Ringbeller::response<Ringbeller::string_body, Ringbeller::vector_sequence> & response)
{
    boost::system::error_code ec;

    // Write to the modem

    auto const wb = Ringbeller::write(sp, command, ec);

    if (ec)
    {
        spdlog::error("Failed to write to the device {}, Reason: {}", device, ec.message());
        return -1;
    }

    fmt::print("Sent {} bytes to the modem\n", wb);

    // Read from the modem

    boost::asio::streambuf buf;

    auto rb = Ringbeller::read(sp, buf, response, ec);

    if (ec)
    {
        spdlog::error("Failed to read from the device {}, Reason: {}", device, ec.message());
        return -1;
    }

    // Present the response
    fmt::print("Read {} bytes from the modem\n", rb);

    fmt::print("Response result code: {}\n", response.rc);
    if (not response.rc_text.empty())
    {
        fmt::print("Response result text: {}\n", response.rc_text);
    }
    else
    {
        fmt::print("Response result text is empty\n");
    }
    if (response.body.empty())
    {
        fmt::print("Response body is empty\n");
    }
    else for (auto const & line : response.body)
    {
        fmt::print("Response body: {}\n", line);
    }

    return 0;
}


#endif /* APP_SRC_EXECUTE_HPP_ */
