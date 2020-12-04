#include "result_code_formatter.hpp"
#include "messages/at_qcfg.hpp"
#include "messages/at_qpcmv.hpp"
#include "messages/at_cscs.hpp"
#include "messages/atd.hpp"
#include "messages/ath.hpp"
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


int run_make_vcall(
    std::string const & device,
    serial_port_param_set_t const & param_set,
    std::string const & da)
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

    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body, Ringbeller::vector_sequence> response;

    fmt::print("Setting up Extended USB configuration\n");

    // query for existing USBCFG
    std::string usb_cfg;

    fmt::print("Query existing Extended USB configuration\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_qcfg_write("\"USBCFG\""), response))
    {
        return rv;
    }

    // wait for +QCFG URC
    std::function<
        std::function<void(boost::system::error_code const & , std::size_t )>(void)
    >
    make_qcfg_read_handler = [&]()
    {
        return [&](boost::system::error_code const & ec, std::size_t rb)
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


                    bool urc_received = response.body.size() and response.body.front().rfind("+QCFG:") == 0;

                    if (not urc_received)
                    {
                        fmt::print(
                            "Received message is not an unsolicited +QCFG notification."
                            " Waiting for another URC\n");

                        Ringbeller::async_read(sp, buf, response, make_qcfg_read_handler());
                    }
                    else
                    {
                        fmt::print("+QCFG received.\n");

                        usb_cfg = response.body.front().substr(7);
                        // toggle flag to 'on'
                        usb_cfg.back() = '1';
                    }
                }

                FN_LEAVE();
            };
    };

    fmt::print("Waiting for +QCFG URC\n");
    Ringbeller::async_read(sp, buf, response, make_qcfg_read_handler());
    ios.run();

    fmt::print("Write new Extended USB configuration\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_qcfg_write(usb_cfg), response))
    {
        return rv;
    }

    fmt::print("Enabling UAC PCM\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_qpcmv_write("1,2"), response))
    {
        return rv;
    }

    fmt::print("Setting up GSM encoding\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_cscs_write("\"GSM\""), response))
    {
        return rv;
    }

    // make the call
    fmt::print("Calling {} for 30 seconds\n", da);
    if (int rv = execute(device, sp, Ringbeller::make_atd("\"{}\";"), response))
    {
        return rv;
    }

    fmt::print("You can send PCM audio to the device now\n", da);

    sleep(30);

    fmt::print("Terminating the call\n", da);
    if (int rv = execute(device, sp, Ringbeller::make_ath(), response))
    {
        return rv;
    }

    fmt::print("Disabling UAC PCM\n");
    if (int rv = execute(device, sp, Ringbeller::make_at_qpcmv_write("0"), response))
    {
        return rv;
    }

    return 0;
}
