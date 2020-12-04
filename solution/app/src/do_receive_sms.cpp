#include "result_code_formatter.hpp"
#include "messages/at_cmgf.hpp"
#include "messages/at_cnmi.hpp"
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

#include <string>
#include <functional>


int run_receive_sms(std::string const & device, serial_port_param_set_t const & param_set)
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

    fmt::print("Setting up SMS format as \"text\"\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_cmgf_write("1")))
    {
        return rv;
    }

    fmt::print("Setting up GSM encoding\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_cscs_write("\"GSM\"")))
    {
        return rv;
    }

    fmt::print("Setting up reception of unsolicited SMS notifications (+CMT)\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_cnmi_write("1,2,0,0,0")))
    {
        return rv;
    }

    fmt::print("Waiting for incoming SMS notification (press Ctrl-C to terminate)\n");

    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body, Ringbeller::vector_sequence> response;


    std::function<
        std::function<void(boost::system::error_code const & , std::size_t )>(void)
    >
    make_read_handler = [&make_read_handler, &sp, &buf, &response, &device]()
    {
        return [&make_read_handler, &sp, &buf, &response, &device](boost::system::error_code const & ec, std::size_t rb)
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

                    bool sms_received = response.body.size() and response.body.front().rfind("+CMT:") == 0;

                    if (not sms_received)
                    {
                        fmt::print(
                            "Received message is not an unsolicited +CMT notification."
                            " Waiting for another unsolicited message\n");

                        Ringbeller::async_read(sp, buf, response, make_read_handler());
                    }
                    else
                    {
                        fmt::print("SMS received.");
                    }
                }

                FN_LEAVE();
            };
    };

    Ringbeller::async_read(sp, buf, response, make_read_handler());

    ios.run();

    return 0;
}
