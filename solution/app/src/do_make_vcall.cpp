#include "result_code_formatter.hpp"
#include "messages/at_qcfg.hpp"
#include "messages/at_qpcmv.hpp"
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

    return 0;
}
