#pragma once

#ifndef LIB_INCLUDE_MESSAGES_ATA_HPP_
#define LIB_INCLUDE_MESSAGES_ATA_HPP_

#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare ATA command
 */
template<typename Body = string_body>
basic_request<Body> make_ata()
{
    basic_request<Body> rv;

    rv.command = "A";

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_ATA_HPP_ */
