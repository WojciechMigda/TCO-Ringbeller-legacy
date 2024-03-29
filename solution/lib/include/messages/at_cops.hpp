#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_COPS_HPP_
#define LIB_INCLUDE_MESSAGES_AT_COPS_HPP_

#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare AT+COPS? command
 */
template<typename Body = string_body>
test_request<Body> make_at_cops_test()
{
    test_request<Body> rv;

    rv.command = "COPS";

    return rv;
}


/*
 * Prepare AT+COPS=? command
 */
template<typename Body = string_body>
read_request<Body> make_at_cops_read()
{
    read_request<Body> rv;

    rv.command = "COPS";

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_COPS_HPP_ */
