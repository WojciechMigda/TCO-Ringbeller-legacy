#pragma once

#ifndef APP_SRC_FORMATTERS_HPP_
#define APP_SRC_FORMATTERS_HPP_

#include "fmt/format.h"

#include <boost/asio/serial_port.hpp>

#include <string>


// https://fmt.dev/latest/api.html#formatting-user-defined-types


/*
 * Custom boost::asio::serial_port::baud_rate formatter for fmt::format
 */
template <>
struct fmt::formatter<boost::asio::serial_port::baud_rate> : public fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(boost::asio::serial_port::baud_rate const & v, FormatContext & ctx)
    {
        return fmt::formatter<std::string>::format(fmt::format("baud_rate::{}", v.value()), ctx);
    }
};


/*
 * Custom boost::asio::serial_port::flow_control formatter for fmt::format
 */
template <>
struct fmt::formatter<boost::asio::serial_port::flow_control> : public fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(boost::asio::serial_port::flow_control const & v, FormatContext & ctx)
    {
        std::string name = "unknown";

        switch (v.value())
        {
            case boost::asio::serial_port::flow_control::none:      name = "none";  break;
            case boost::asio::serial_port::flow_control::software:  name = "sw";    break;
            case boost::asio::serial_port::flow_control::hardware:  name = "hw";    break;
        }

        return fmt::formatter<std::string>::format(fmt::format("flow_control::{}", name), ctx);
    }
};


/*
 * Custom boost::asio::serial_port::parity formatter for fmt::format
 */
template <>
struct fmt::formatter<boost::asio::serial_port::parity> : public fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(boost::asio::serial_port::parity const & v, FormatContext & ctx)
    {
        std::string name = "unknown";

        switch (v.value())
        {
            case boost::asio::serial_port::parity::none:    name = "none";  break;
            case boost::asio::serial_port::parity::odd:     name = "odd";   break;
            case boost::asio::serial_port::parity::even:    name = "even";  break;
        }

        return fmt::formatter<std::string>::format(fmt::format("parity::{}", name), ctx);
    }
};


/*
 * Custom boost::asio::serial_port::stop_bits formatter for fmt::format
 */
template <>
struct fmt::formatter<boost::asio::serial_port::stop_bits> : public fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(boost::asio::serial_port::stop_bits const & v, FormatContext & ctx)
    {
        std::string name = "unknown";

        switch (v.value())
        {
            case boost::asio::serial_port::stop_bits::one:          name = "one";           break;
            case boost::asio::serial_port::stop_bits::onepointfive: name = "onepointfive";  break;
            case boost::asio::serial_port::stop_bits::two:          name = "two";           break;
        }

        return fmt::formatter<std::string>::format(fmt::format("stop_bits::{}", name), ctx);
    }
};


/*
 * Custom boost::asio::serial_port::character_size formatter for fmt::format
 */
template <>
struct fmt::formatter<boost::asio::serial_port::character_size> : public fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(boost::asio::serial_port::character_size const & v, FormatContext & ctx)
    {
        return fmt::formatter<std::string>::format(fmt::format("character_size::{}", v.value()), ctx);
    }
};


#endif /* APP_SRC_FORMATTERS_HPP_ */
