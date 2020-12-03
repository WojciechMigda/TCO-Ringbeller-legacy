#include "parser.hpp"
#include "message.hpp"

#include "spdlog/spdlog.h"

#include "boost/spirit/home/x3.hpp"

// debug URL: http://coliru.stacked-crooked.com/a/9f5adbc76755f892


namespace Ringbeller
{


namespace detail
{


auto grammar_spec = [](auto & rc_setter, auto & rc_text_setter, auto & body_appender)
{
    namespace x3 = boost::spirit::x3;
    using x3::lit;
    using x3::char_;

    auto set_ok = [&](auto &){ rc_setter(Ringbeller::result_code::ok); };
    auto set_connect = [&](auto &){ rc_setter(Ringbeller::result_code::connect); };
    auto set_ring = [&](auto &){ rc_setter(Ringbeller::result_code::ring); };
    auto set_no_carrier = [&](auto &){ rc_setter(Ringbeller::result_code::no_carrier); };
    auto set_error = [&](auto &){ rc_setter(Ringbeller::result_code::error); };
    auto set_no_dialtone = [&](auto &){ rc_setter(Ringbeller::result_code::no_dialtone); };
    auto set_busy = [&](auto &){ rc_setter(Ringbeller::result_code::busy); };
    auto set_no_answer = [&](auto &){ rc_setter(Ringbeller::result_code::no_answer); };
    auto set_cme_error = [&](auto &){ rc_setter(Ringbeller::result_code::cme_error); };
    auto set_cms_error = [&](auto &){ rc_setter(Ringbeller::result_code::cms_error); };
    auto set_command_not_support = [&](auto &){ rc_setter(Ringbeller::result_code::command_not_support); };
    auto set_too_many_parameters = [&](auto &){ rc_setter(Ringbeller::result_code::too_may_parameters); };

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

    static auto const text = x3::raw[+(char_ - CR)];

    // these are also a matter of device configuration, smh
    auto const OK = lit("OK")[set_ok];
    auto const CONNECT = lit("CONNECT")[set_connect] >> -text[rc_text_setter];
    auto const RING = lit("RING")[set_ring];
    auto const NO_CARRIER = lit("NO CARRIER")[set_no_carrier];
    auto const ERROR = lit("ERROR")[set_error];
    auto const NO_DIALTONE = lit("NO DIALTONE")[set_no_dialtone];
    auto const BUSY = lit("BUSY")[set_busy];
    auto const NO_ANSWER = lit("NO ANSWER")[set_no_answer];

    auto const CME_ERROR = lit("+CME ERROR: ")[set_cme_error] >> text[rc_text_setter];
    auto const CMS_ERROR = lit("+CMS ERROR: ")[set_cms_error] >> text[rc_text_setter];
    auto const COMMAND_NOT_SUPPORT = lit("COMMAND NOT SUPPORT")[set_command_not_support];
    auto const TOO_MANY_PARAMETERS = lit("TOO MANY PARAMETERS")[set_too_many_parameters];


    auto const RESPONSE_LINE = text[body_appender] >> CRLF;

    auto FINAL_RESULT_CODE =
        CRLF >>
        (
            OK | CONNECT | RING | NO_CARRIER | ERROR | NO_DIALTONE |
            BUSY | NO_ANSWER |
            CME_ERROR | CMS_ERROR |
            COMMAND_NOT_SUPPORT | TOO_MANY_PARAMETERS
        ) >> CRLF;

    auto response =
        FINAL_RESULT_CODE
        | (CRLF >> +RESPONSE_LINE >> FINAL_RESULT_CODE);

    return response;
};



std::pair<asio_buf_iterator, bool>
match_condition(asio_buf_iterator begin, asio_buf_iterator end)
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    using x3::phrase_parse;
    using ascii::blank;

    auto nop1 = [](auto &&){};
    auto nop2 = [](auto &&){};

    bool ok = phrase_parse(
        begin,
        end,
        grammar_spec(nop1, nop2, nop2),
        blank
    );

    return std::make_pair(begin, ok);
}


std::pair<asio_buf_iterator, bool>
parse_response(
    asio_buf_iterator begin,
    asio_buf_iterator end,
    ::Ringbeller::result_code & result_code,
    std::string & result_text,
    std::vector<std::string> & body)
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    using x3::phrase_parse;
    using ascii::blank;

    auto rc_setter = [&](auto && rc){ result_code = rc; };
    auto rc_text_setter = [&](auto & ctx)
    {
        auto val = _attr(ctx);
        result_text = std::string(val.begin(), val.end());
    };
    auto body_appender = [&](auto & ctx)
    {
        auto val = _attr(ctx);
        body.emplace_back(val.begin(), val.end());
    };

    bool ok = phrase_parse(
        begin,
        end,
        grammar_spec(rc_setter, rc_text_setter, body_appender),
        blank
    );

    return std::make_pair(begin, ok);
}


}  // namespace detail


}  // namespace Ringbeller
