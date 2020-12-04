#include "result_code_formatter.hpp"
#include "messages/at_cmgf.hpp"
#include "messages/at_cmgs.hpp"
#include "messages/at_cscs.hpp"
#include "message.hpp"
#include "write.hpp"
#include "read.hpp"
#include "serial_port_setup.hpp"
#include "logging.hpp"
#include "execute.hpp"

#include "spdlog/spdlog.h"
#include "fmt/core.h"

#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>


int run_send_sms(
    std::string const & device,
    serial_port_param_set_t const & param_set,
    std::string const & da,
    std::string const & sms_text)
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

    Ringbeller::response<Ringbeller::string_body, Ringbeller::vector_sequence> response;

    fmt::print("Setting up SMS format as \"text\"\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_cmgf_write("1"), response))
    {
        return rv;
    }

    fmt::print("Setting up GSM encoding\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_cscs_write("\"GSM\""), response))
    {
        return rv;
    }

    fmt::print("Sending SMS\n");

    boost::asio::streambuf buf;

    auto text_write_handler = [&](boost::system::error_code const & ec, std::size_t wb)
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
        }

        FN_LEAVE();
    };

    auto prompt_read_handler = [&](boost::system::error_code const & ec, std::size_t rb)
    {
        FN_ENTER();

        if (ec)
        {
            spdlog::error("Asynchronous read from device {} completed with an error: {}",
                device, ec.message());
        }
        else
        {
            buf.consume(rb);

            bool prompt_received =
                response.rc == Ringbeller::result_code::none and
                response.body.size() and
                response.body.front().rfind("> ") == 0;

            if (prompt_received)
            {
                fmt::print("Prompt received, sending SMS body\n");
                Ringbeller::async_write_text(sp, sms_text, text_write_handler);
            }
            else
            {
                spdlog::error("Prompt '> ' not received");
            }
        }

        FN_LEAVE();
    };

    auto cmgs_write_handler = [&](boost::system::error_code const & ec, std::size_t wb)
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
            fmt::print("Waiting for prompt..\n", wb);

            Ringbeller::async_read(sp, buf, response, prompt_read_handler);
        }

        FN_LEAVE();
    };

    auto command = Ringbeller::make_at_cmgs_write(fmt::format("\"{}\"", da));
    Ringbeller::async_write(sp, command, cmgs_write_handler);

    ios.run();

    return 0;
}
