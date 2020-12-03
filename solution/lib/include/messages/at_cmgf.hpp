#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_CMGF_HPP_
#define LIB_INCLUDE_MESSAGES_AT_CMGF_HPP_

#include "message.hpp"

#include <string>


namespace Ringbeller
{


/*
 * Prepare AT+CMGF=<params> command
 */
template<typename Body = string_body>
write_request<Body> make_at_cmgf_write(std::string const & params)
{
    write_request<Body> rv;

    rv.command = "CMGF";
    rv.args = params;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_CMGF_HPP_ */
