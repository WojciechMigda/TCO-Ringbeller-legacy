#include "at/response.hpp"
#include "messages/at.hpp"
#include "message.hpp"
#include "write.hpp"
#include "read.hpp"

#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>


int run_at_ok(std::string const & device)
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

    auto command = Ringbeller::make_at();

    // Write to the modem

    auto const wb = Ringbeller::write(sp, command, ec);

    if (ec)
    {
        spdlog::error("Failed to write to the device {}, Reason: {}", device, ec.message());
        return -1;
    }

    spdlog::info("Sent {} bytes to the modem", wb);

    // Read from the modem

    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body, Ringbeller::vector_sequence> response;

    auto rb = Ringbeller::read(sp, buf, response, ec);

    if (ec)
    {
        spdlog::error("Failed to read from the device {}, Reason: {}", device, ec.message());
        return -1;
    }

    // Present the response
    spdlog::info("Read {} bytes from the modem", rb);

    spdlog::info("Response result code: {}", response.result_code);
    if (not response.rc_text.empty())
    {
        spdlog::info("Response result text: {}", response.rc_text);
    }
    else
    {
        spdlog::info("Response result text is empty");
    }
    if (response.body.empty())
    {
        spdlog::info("Response body is empty");
    }
    else for (auto const & line : response.body)
    {
        spdlog::info("Response body: {}", line);
    }

    return 0;
}
