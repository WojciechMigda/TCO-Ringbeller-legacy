#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_QPCMV_HPP_
#define LIB_INCLUDE_MESSAGES_AT_QPCMV_HPP_

#include "message.hpp"

#include <string>


namespace Ringbeller
{


/*
 * Prepare AT+QPCMV=<params> command
 */
template<typename Body = string_body>
write_request<Body> make_at_qpcmv_write(std::string const & params)
{
    write_request<Body> rv;

    rv.command = "QPCMV";
    rv.args = params;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_QPCMV_HPP_ */
