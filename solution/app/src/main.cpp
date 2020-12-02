#include "do_at_ok.hpp"
#include "do_ati.hpp"

#include "clipp.hpp"
#include "spdlog/spdlog.h"

#include <iostream>


/*
 * Convert bool into string representation.
 */
static std::string to_string(bool v)
{
    return v ? "true" : "false";
}


/*
 * Set `spdlog` log level according to `verbose` and `debug` flags.
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

    bool verbose = false;
    bool debug = false;
    bool trace = false;

    std::string device;

    // TODO modem params
    // TODO timeouts?

    auto at_ok = (
        clipp::command("at_ok").set(do_at_ok, true).doc("Execute AT/OK scenario (synchronous API)")
    );

    auto ati = (
        clipp::command("ati").set(do_ati, true).doc("Execute ATI scenario (synchronous API)")
    );

    auto common = (
        clipp::required("--device").doc("Modem device path") & clipp::value("Path to the modem device", device),
        clipp::option("--verbose", "-v").set(verbose, true).doc("Enable verbose output, default=" + to_string(verbose)),
        clipp::option("--debug").set(debug, true).doc("Enable debug output, default=" + to_string(debug)),
        clipp::option("--trace").set(trace, true).doc("Enable trace output, default=" + to_string(trace))
    );

    auto cli = (at_ok | ati) & common;

    if (not clipp::parse(argc, argv, cli))
    {
        std::cout << clipp::make_man_page(cli, argv[0]);

        return -1;
    }
    else
    {
        set_log_level(verbose, debug, trace);

        if (do_at_ok)
        {
            spdlog::info("Executing AT/OK scenario");

            auto rv = run_at_ok(device);

            return rv;
        }
        else if (do_ati)
        {
            spdlog::info("Executing ATI scenario");

            auto rv = run_ati(device);

            return rv;
        }
    }

    return 0;
}
