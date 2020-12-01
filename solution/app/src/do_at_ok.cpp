#include "messages/at.hpp"
#include "message.hpp"
#include "write.hpp"

#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>


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

    auto wb = Ringbeller::write(sp, command, ec);

    if (ec)
    {
        spdlog::error("Failed to write to the device {}, Reason: {}", device, ec.message());
    }

    spdlog::info("Sent {} bytes to the modem", wb);

    return 0;
}
