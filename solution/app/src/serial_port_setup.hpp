#pragma once

#ifndef APP_SRC_SERIAL_PORT_SETUP_HPP_
#define APP_SRC_SERIAL_PORT_SETUP_HPP_

#include <boost/asio/serial_port.hpp>

#include <variant>
#include <optional>
#include <array>


using serial_port_param_t = std::variant<
    std::optional<boost::asio::serial_port::baud_rate>,
    std::optional<boost::asio::serial_port::flow_control>,
    std::optional<boost::asio::serial_port::parity>,
    std::optional<boost::asio::serial_port::stop_bits>,
    std::optional<boost::asio::serial_port::character_size>
>;


using serial_port_param_set_t = std::array<serial_port_param_t, std::variant_size_v<serial_port_param_t>>;


/*
 * Configure boost::asio::serial_port by setting passed parameters
 */
void setup_serial_port(
    boost::asio::serial_port & sp,
    serial_port_param_set_t const & param_set);


#endif /* APP_SRC_SERIAL_PORT_SETUP_HPP_ */
