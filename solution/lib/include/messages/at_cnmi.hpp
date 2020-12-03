#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_CNMI_HPP_
#define LIB_INCLUDE_MESSAGES_AT_CNMI_HPP_

#include "message.hpp"

#include <string>


namespace Ringbeller
{


/*
 * Prepare AT+CNMI=<params> command
 */
template<typename Body = string_body>
write_request<Body> make_at_cnmi_write(std::string const & params)
{
    write_request<Body> rv;

    rv.command = "CNMI";
    rv.args = params;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_CNMI_HPP_ */
