#pragma once

#ifndef LIB_INCLUDE_RESULT_CODE_FORMATTER_HPP_
#define LIB_INCLUDE_RESULT_CODE_FORMATTER_HPP_

#include "message.hpp"

#include "fmt/format.h"

#include <string>


// https://fmt.dev/latest/api.html#formatting-user-defined-types


/*
 * Custom Ringbeller::result_code formatter for fmt::format
 */
template <>
struct fmt::formatter<Ringbeller::result_code> : public fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(Ringbeller::result_code const & v, FormatContext & ctx)
    {
        std::string name = "unknown";

        switch (v)
        {
            case Ringbeller::result_code::busy:                 name = "BUSY";  break;
            case Ringbeller::result_code::cme_error:            name = "CME ERROR";  break;
            case Ringbeller::result_code::cms_error:            name = "CMS ERROR";  break;
            case Ringbeller::result_code::command_not_support:  name = "COMMAND NOT SUPPORT";  break;
            case Ringbeller::result_code::connect:              name = "CONNECT";  break;
            case Ringbeller::result_code::error:                name = "ERROR";  break;
            case Ringbeller::result_code::no_answer:            name = "NO ANSWER";  break;
            case Ringbeller::result_code::no_carrier:           name = "NO CARRIER";  break;
            case Ringbeller::result_code::no_dialtone:          name = "NO DIALTONE";  break;
            case Ringbeller::result_code::none:                 name = "(none)";  break;
            case Ringbeller::result_code::ok:                   name = "OK";  break;
            case Ringbeller::result_code::ring:                 name = "RING";  break;
            case Ringbeller::result_code::too_may_parameters:   name = "TOO MANY PARAMETERS";  break;
        }

        return fmt::formatter<std::string>::format(name, ctx);
    }
};



#endif /* LIB_INCLUDE_RESULT_CODE_FORMATTER_HPP_ */
