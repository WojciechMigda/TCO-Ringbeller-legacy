#pragma once

#ifndef LIB_INCLUDE_MESSAGES_ATD_HPP_
#define LIB_INCLUDE_MESSAGES_ATD_HPP_

#include "message.hpp"

#include <string>


namespace Ringbeller
{


/*
 * Prepare ATD command
 */
template<typename Body = string_body>
basic_request<Body> make_atd(std::string const & params)
{
    basic_request<Body> rv;

    rv.command = "D";
    rv.args = params;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_ATD_HPP_ */
