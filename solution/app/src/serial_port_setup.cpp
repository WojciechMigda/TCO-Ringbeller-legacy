#include "formatters.hpp"
#include "serial_port_setup.hpp"

#include "spdlog/spdlog.h"

#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>

#include <variant>


void setup_serial_port(
    boost::asio::serial_port & sp,
    serial_port_param_set_t const & param_set)
{
    for (auto const param : param_set)
    {
        auto visitor = [&sp](auto const & param)
        {
            if (param.has_value())
            {
                boost::system::error_code ec;

                auto const value = param.value();

                sp.set_option(value, ec);

                if (ec)
                {
                    spdlog::error("Failed to set serial port parameter {}", value);
                }
                else
                {
                    spdlog::debug("Set {} on serial port", value);
                }
            }
        };

        std::visit(visitor, param);
    }
}
