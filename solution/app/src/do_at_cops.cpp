#include "messages/at_cops.hpp"
#include "message.hpp"
#include "write.hpp"
#include "read.hpp"
#include "serial_port_setup.hpp"
#include "logging.hpp"

#include "spdlog/spdlog.h"
#include "fmt/core.h"

#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>

#include <string>


int run_at_cops(std::string const & device, serial_port_param_set_t const & param_set)
{
    boost::system::error_code ec;

    boost::asio::io_service ios;
    boost::asio::serial_port sp(ios);

    sp.open(device, ec);

    if (ec)
    {
        spdlog::error("Failed to open device {}. Reason: {}", device, ec.message());

        return -1;
    }

    setup_serial_port(sp, param_set);

    auto command = Ringbeller::make_at_cops_test();
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body, Ringbeller::vector_sequence> response;

    // Write to the modem

    auto read_handler = [&response](boost::system::error_code const & ec, std::size_t rb)
    {
        FN_ENTER();

        // Present the response
        fmt::print("Read {} bytes from the modem\n", rb);

        fmt::print("Response result code: {}\n", response.result_code);
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

        FN_LEAVE();
    };

    auto write_handler = [&](boost::system::error_code const & ec, std::size_t wb)
    {
        FN_ENTER();

        if (ec)
        {
            spdlog::error("Asynchronous write to device {} completed with an error: {}",
                device, ec.message());
        }
        else
        {
            fmt::print("Sent {} bytes to the modem\n", wb);
            fmt::print("Awaiting response, it may take a while..\n", wb);

            Ringbeller::async_read(sp, buf, response, read_handler);
        }

        FN_LEAVE();
    };

    Ringbeller::async_write(sp, command, write_handler);

    // run the event loop until completion
    ios.run();

    return 0;
}
