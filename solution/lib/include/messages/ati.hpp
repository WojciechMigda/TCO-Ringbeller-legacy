#pragma once

#ifndef LIB_INCLUDE_MESSAGES_ATI_HPP_
#define LIB_INCLUDE_MESSAGES_ATI_HPP_


#include "message.hpp"


namespace Ringbeller
{


template<typename Body = string_body>
basic_request<Body> make_ati()
{
    basic_request<Body> rv;

    rv.command = "I";

    return rv;
}


}  // namespace Ringbeller



#endif /* LIB_INCLUDE_MESSAGES_ATI_HPP_ */
