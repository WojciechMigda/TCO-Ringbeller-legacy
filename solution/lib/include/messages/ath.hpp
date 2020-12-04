#pragma once

#ifndef LIB_INCLUDE_MESSAGES_ATH_HPP_
#define LIB_INCLUDE_MESSAGES_ATH_HPP_

#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare ATH command
 */
template<typename Body = string_body>
basic_request<Body> make_ath()
{
    basic_request<Body> rv;

    rv.command = "H";

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_ATH_HPP_ */
