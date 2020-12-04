#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>

#include <string>

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <istream>
//#include <boost/thread.hpp>

using std::streambuf;


struct Ringbeller :
    public std::enable_shared_from_this<Ringbeller>
    //public boost::enable_shared_from_this<Ringbeller>
{
    using ec_t = boost::system::error_code;

    boost::asio::serial_port m_sp;
    boost::asio::streambuf m_buffer;

    Ringbeller(boost::asio::io_service & ios, std::string const & device)
    : m_sp(ios, device)
    {
        printf("is open %d\n", m_sp.is_open());
        //m_sp.set_option(boost::asio::serial_port::baud_rate(19200));
    }

    ~Ringbeller()
    {
        ec_t ec;

        m_sp.close(ec);
    }


    void write_handler(ec_t const & ec, std::size_t sz)
    {
        if (ec)
        {
            printf("Error!\n");
        }
        else
        {
            printf("Wrote %zu bytes\n", sz);

            std::size_t bytes_read = boost::asio::read_until(this->m_sp, this->m_buffer, "\r\n");
            printf("read %zu\n", bytes_read);
        }
    }

    template<typename ATCommandT>
    void send(ATCommandT && cmd)
    {
        ec_t ec;
//#if 0

        boost::asio::async_write(m_sp, boost::asio::buffer("AT\r\n", 4),
            [this](ec_t const & ec, std::size_t sz)
            {
                if (ec)
                {
                    printf("Error!\n");
                }
                else
                {
                    printf("Wrote %zu bytes\n", sz);

                    std::size_t bytes_read = boost::asio::read_until(this->m_sp, this->m_buffer, "\r\n");
                    //printf("read %zu\n", bytes_read);
                    {
                        m_buffer.commit(bytes_read);
                        std::istream is(&m_buffer);
                        std::string s;
                        is >> s;
                        printf("read %zu %s\n", bytes_read, s.c_str());
                        m_buffer.consume(bytes_read);

                    }

                    {
                        // read more?
//                        std::size_t bytes_read = boost::asio::read_until(this->m_sp, this->m_buffer, "\r\n");
//                        printf("read %zu\n", bytes_read);
                    }
                }
            }
        );
//        boost::asio::async_write(m_sp, boost::asio::buffer("AT\r\n"),
//            boost::bind(&Ringbeller::write_handler, shared_from_this(),
//                boost::asio::placeholders::error,
//                boost::asio::placeholders::bytes_transferred
//                //std::placeholders::_1,
//                //std::placeholders::_2
//                )
//        );
//        boost::asio::async_write(m_sp, boost::asio::buffer("AT\r\n"),
//            boost::bind(&Ringbeller::write_handler, this,
//                boost::asio::placeholders::error,
//                boost::asio::placeholders::bytes_transferred
//                //std::placeholders::_1,
//                //std::placeholders::_2
//                )
//        );

//#endif

#if 0
        auto wb = boost::asio::write(m_sp, boost::asio::buffer("AT\r\n"), ec);
        printf("written %zu\n", wb);
        auto rb = boost::asio::read_until(m_sp, m_buffer, "\r\n");
        {
//            m_buffer.commit(rb);
//
//            auto d = m_buffer.data();
//
//            std::string s(boost::asio::buffers_begin(d), boost::asio::buffers_end(d));
//            printf("read %zu %s\n", rb, s.c_str());

//            m_buffer.consume(rb);


            // to dziala
            m_buffer.commit(rb);
            std::istream is(&m_buffer);
            std::string s;
            is >> s;
            printf("read %zu %s\n", rb, s.c_str());
            m_buffer.consume(rb);
        }

        // na tym sie zablokuje
        rb = boost::asio::read_until(m_sp, m_buffer, "\r\n");
        {
//            m_buffer.commit(rb);
//
//            auto d = m_buffer.data();
//
//            std::string s(boost::asio::buffers_begin(d), boost::asio::buffers_end(d));
//            printf("read %zu %s\n", rb, s.c_str());
//
//            m_buffer.consume(rb);

            m_buffer.commit(rb);
            std::istream is(&m_buffer);
            std::string s;
            is >> s;
            printf("read %zu %s\n", rb, s.c_str());
            m_buffer.consume(rb);

        }
#endif

//        boost::asio::deadline_timer timer(m_ios, boost::posix_time::seconds(5));
//        timer.wait();

    }
};

#if 0
copy (istream_iterator < char > (istream ( &buf ) >>noskipws ),
        istream_iterator < char > ( ),
        ostream_iterator < char > ( cout ) ) ;;
#endif

void runme()
{
    boost::asio::io_service ios;

    Ringbeller rb(ios, "/dev/ttyUSB0");

//    rb.send(AT::AT());

//    ios.run();
//    ios.run_one();
    ios.poll();

    printf("\n after ios.run()\n");
    boost::asio::deadline_timer timer2(ios, boost::posix_time::seconds(1));
    timer2.wait();
}
