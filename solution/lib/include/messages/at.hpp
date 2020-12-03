#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_HPP_
#define LIB_INCLUDE_MESSAGES_AT_HPP_

#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare AT command
 */
template<typename Body = string_body>
basic_request<Body> make_at()
{
    basic_request<Body> rv;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_HPP_ */
