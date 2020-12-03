#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_CSCS_HPP_
#define LIB_INCLUDE_MESSAGES_AT_CSCS_HPP_

#include "message.hpp"

#include <string>


namespace Ringbeller
{


/*
 * Prepare AT+CSCS=<params> command
 */
template<typename Body = string_body>
write_request<Body> make_at_cscs_write(std::string const & params)
{
    write_request<Body> rv;

    rv.command = "CSCS";
    rv.args = params;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_CSCS_HPP_ */
