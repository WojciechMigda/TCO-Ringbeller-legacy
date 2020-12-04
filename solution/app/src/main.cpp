#include "do_at_ok.hpp"
#include "do_ati.hpp"
#include "do_at_cops.hpp"
#include "do_receive_sms.hpp"
#include "do_send_sms.hpp"
#include "do_make_vcall.hpp"
#include "do_receive_vcall.hpp"

#include "formatters.hpp"

#include "clipp.hpp"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "fmt/core.h"
#include "serial_port_setup.hpp"

#include <boost/asio/serial_port.hpp>

#include <iostream>
#include <optional>
#include <string>


/*
 * Convert option type into string representation.
 */
template<typename T>
std::string to_string(std::optional<T> const & v, std::string const & none = "(empty)")
{
    return v.has_value() ? fmt::format("{}", v.value()) : none;
}


/*
 * Set `spdlog` log level according to `verbose`, `debug`, and `trace` flags.
 */
static void set_log_level(bool verbose, bool debug, bool trace)
{
    if (trace)
    {
        spdlog::set_level(spdlog::level::trace);
    }
    else if (debug)
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else if (verbose)
    {
        spdlog::set_level(spdlog::level::info);
    }
    else
    {
        spdlog::set_level(spdlog::level::warn);
    }
}


int main(int argc, char **argv)
{
    bool do_at_ok = false;
    bool do_ati = false;
    bool do_at_cops = false;
    bool do_rcv_sms = false;
    bool do_send_sms = false;
    bool do_make_call = false;
    bool do_rcv_call = false;

    std::string da;
    std::string sms_text("Feel the bern!");

    bool verbose = false;
    bool debug = false;
    bool trace = false;

    std::string device;

    std::optional<boost::asio::serial_port::baud_rate> maybe_baud_rate;
    auto baud_rate_setter = [&maybe_baud_rate](std::string const & s){ maybe_baud_rate = boost::asio::serial_port::baud_rate(std::stoi(s)); };

    std::optional<boost::asio::serial_port::flow_control> maybe_flow_control;
    auto flow_control_setter = [&maybe_flow_control](std::string const & s)
    {
        using flow_control = boost::asio::serial_port::flow_control;

        if (s == "none")
        {
            maybe_flow_control = flow_control(flow_control::none);
        }
        else if (s == "hw")
        {
            maybe_flow_control = flow_control(flow_control::hardware);
        }
        else if (s == "sw")
        {
            maybe_flow_control = flow_control(flow_control::software);
        }
    };

    std::optional<boost::asio::serial_port::parity> maybe_parity;
    auto parity_setter = [&maybe_parity](std::string const & s)
    {
        using parity = boost::asio::serial_port::parity;

        if (s == "none")
        {
            maybe_parity = parity(parity::none);
        }
        else if (s == "odd")
        {
            maybe_parity = parity(parity::odd);
        }
        else if (s == "even")
        {
            maybe_parity = parity(parity::even);
        }
    };

    std::optional<boost::asio::serial_port::stop_bits> maybe_stop_bits;
    auto stop_bits_setter = [&maybe_stop_bits](std::string const & s)
    {
        using stop_bits = boost::asio::serial_port::stop_bits;

        if (s == "one")
        {
            maybe_stop_bits = stop_bits(stop_bits::one);
        }
        else if (s == "onepointfive")
        {
            maybe_stop_bits = stop_bits(stop_bits::onepointfive);
        }
        else if (s == "two")
        {
            maybe_stop_bits = stop_bits(stop_bits::two);
        }
    };

    std::optional<boost::asio::serial_port::character_size> maybe_character_size;
    auto character_size_setter = [&maybe_character_size](std::string const & s){ maybe_character_size = boost::asio::serial_port::character_size(std::stoi(s)); };


    auto at_ok = (
        clipp::command("at_ok").set(do_at_ok, true).doc("Execute AT/OK scenario (synchronous API)")
    );

    auto ati = (
        clipp::command("ati").set(do_ati, true).doc("Execute ATI scenario (synchronous API)")
    );

    auto at_cops = (
        clipp::command("at+cops").set(do_at_cops, true).doc("Execute AT+COPS=? scenario (asynchronous API)")
    );

    auto receive_sms = (
        clipp::command("rcv-sms").set(do_rcv_sms, true).doc("Wait and receive SMS")
    );

    auto send_sms = (
        clipp::command("send-sms").set(do_send_sms, true).doc("Send SMS") & clipp::value("Destination address", da),
        clipp::option("--text").doc(fmt::format("Set SMS text, default=\"{}\"", sms_text)) & clipp::value("SMS text to send", sms_text)
    );

    auto make_call = (
        clipp::command("make-call").set(do_make_call, true).doc("Make voice call") & clipp::value("Destination address", da)
    );

    auto receive_call = (
        clipp::command("rcv-call").set(do_rcv_call, true).doc("Wait and receive voice call")
    );

    auto common = (
        clipp::required("--device").doc("Modem device path") & clipp::value("Path to the modem device", device),

        // modem params
        clipp::option("--baud-rate").doc("Set new baud rate on device, default=" + to_string(maybe_baud_rate))
            & clipp::integer("New baud rate to set").call(baud_rate_setter),
        clipp::option("--flow-control").doc("Set new flow control on device, default=" + to_string(maybe_flow_control))
            & (clipp::required("none").call(flow_control_setter)
                | clipp::required("sw").call(flow_control_setter)
                | clipp::required("hw").call(flow_control_setter)),
        clipp::option("--parity").doc("Set new parity on device, default=" + to_string(maybe_parity))
            & (clipp::required("none").call(parity_setter)
                | clipp::required("odd").call(parity_setter)
                | clipp::required("even").call(parity_setter)),
        clipp::option("--stop-bits").doc("Set new stop bits on device, default=" + to_string(maybe_stop_bits))
            & (clipp::required("one").call(stop_bits_setter)
                | clipp::required("onepointfive").call(stop_bits_setter)
                | clipp::required("two").call(stop_bits_setter)),
        clipp::option("--character-size").doc("Set new character size on device, default=" + to_string(maybe_character_size))
            & clipp::integer("New character size to set").call(character_size_setter),


        clipp::option("--verbose", "-v").set(verbose, true).doc("Enable verbose output, default=" + fmt::format("{}", verbose)),
        clipp::option("--debug").set(debug, true).doc("Enable debug output, default=" + fmt::format("{}", debug)),
        clipp::option("--trace").set(trace, true).doc("Enable trace output, default=" + fmt::format("{}", trace))
    );

    auto cli =
        (at_ok & common) |
        (ati & common) |
        (at_cops & common) |
        (receive_sms & common) |
        (send_sms & common) |
        (make_call & common) |
        (receive_call & common);

    int rv = 0;

    if (not clipp::parse(argc, argv, cli))
    {
        std::cout << clipp::make_man_page(cli, argv[0]);

        return -1;
    }
    else
    {
        set_log_level(verbose, debug, trace);

        auto const serial_port_param_set = serial_port_param_set_t{maybe_baud_rate, maybe_parity, maybe_stop_bits, maybe_flow_control, maybe_character_size};

        if (do_at_ok)
        {
            fmt::print("Executing AT/OK scenario\n");

            rv = run_at_ok(device, serial_port_param_set);
        }
        else if (do_ati)
        {
            fmt::print("Executing ATI scenario\n");

            rv = run_ati(device, serial_port_param_set);
        }
        else if (do_at_cops)
        {
            fmt::print("Executing AT+COPS scenario\n");

            rv = run_at_cops(device, serial_port_param_set);
        }
        else if (do_rcv_sms)
        {
            fmt::print("Executing \"Receive SMS\" scenario\n");

            rv = run_receive_sms(device, serial_port_param_set);
        }
        else if (do_send_sms)
        {
            fmt::print("Executing \"Send SMS\" scenario\n");
            fmt::print("SMS target: {}\n", da);
            fmt::print("SMS text: {}\n", sms_text);

            rv = run_send_sms(device, serial_port_param_set, da, sms_text);
        }
        else if (do_rcv_call)
        {
            fmt::print("Executing \"Receive voice call\" scenario\n");

            rv = run_receive_vcall(device, serial_port_param_set);
        }
        else if (do_make_call)
        {
            fmt::print("Executing \"Make voice call\" scenario\n");
            fmt::print("Voice call target: {}\n", da);

            rv = run_make_vcall(device, serial_port_param_set, da);
        }
    }

    fmt::print("DONE with status={}\n", rv);

    return rv;
}
