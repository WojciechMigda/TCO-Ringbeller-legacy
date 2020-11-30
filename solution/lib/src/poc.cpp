#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>


struct Ringbeller
{
    boost::asio::io_service m_ios;
};


void runme()
{
    Ringbeller rb;
}
