#include "parser.hpp"

#include "spdlog/spdlog.h"

#include "boost/spirit/home/x3.hpp"


auto grammar_spec = []()
{
    namespace x3 = boost::spirit::x3;
    using x3::lit;
    using x3::char_;

    /*
     * cheat sheet:
     * + : >=1 times
     * - : 0 or 1 times
     * * : 0 or n times
     * More: https://www.boost.org/doc/libs/1_74_0/libs/spirit/doc/html/spirit/qi/reference/operator.html
     */

    static auto const CR = lit('\r');
    static auto const CRLF = lit("\r\n"); // This is actually a configurable
                                          // parameter of the device.
                                          // It gets funky if it's changed.
                                          // Hardcoded for now.

    static auto const text = +(!CR);

    // these are also a matter of device configuration, smh
    static auto const OK = lit("OK");
    static auto const CONNECT = lit("CONNECT") >> -text;
    static auto const RING = lit("RING");
    static auto const NO_CARRIER = lit("NO CARRIER");
    static auto const ERROR = lit("ERROR");
    static auto const NO_DIALTONE = lit("NO DIALTONE");
    static auto const BUSY = lit("BUSY");
    static auto const NO_ANSWER = lit("NO ANSWER");

    static auto const CME_ERROR = lit("+CME ERROR: ") >> text;
    static auto const CMS_ERROR = lit("+CMS ERROR: ") >> text;
    static auto const COMMAND_NOT_SUPPORT = lit("COMMAND NOT SUPPORT");
    static auto const TOO_MANY_PARAMETERS = lit("TOO MANY PARAMETERS");

    static auto const RESPONSE_LINE = CRLF >> text >> CRLF;

    static auto FINAL_RESULT_CODE =
        CRLF >>
        (
            OK | CONNECT | RING | NO_CARRIER | ERROR | NO_DIALTONE |
            BUSY | NO_ANSWER |
            CME_ERROR | CMS_ERROR |
            COMMAND_NOT_SUPPORT | TOO_MANY_PARAMETERS
        ) >> CRLF;

    static auto response =
        FINAL_RESULT_CODE
        | (+RESPONSE_LINE >> FINAL_RESULT_CODE);

    return response;
};


std::pair<asio_buf_iterator, bool>
match_condition(asio_buf_iterator begin, asio_buf_iterator end)
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    using x3::phrase_parse;
    using ascii::space;

    bool ok = phrase_parse(
        begin,
        end,
        grammar_spec(),
        x3::blank
    );

    return std::make_pair(begin, ok);
}
