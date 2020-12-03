#pragma once

#ifndef LIB_INCLUDE_LOGGING_HPP_
#define LIB_INCLUDE_LOGGING_HPP_

#include "spdlog/spdlog.h"

#include <boost/current_function.hpp>
#include <boost/system/error_code.hpp>


#define FN_ENTER() spdlog::trace("Fn Enter: {}", BOOST_CURRENT_FUNCTION)

#define FN_LEAVE() spdlog::trace("Fn Leave: {}", BOOST_CURRENT_FUNCTION)


namespace Ringbeller
{


/*
 * Utility for logging boost::system::error_code
 */
static inline void LOG_CHECK_EC(boost::system::error_code const & ec)
{
    if (ec)
    {
        spdlog::error("boost::system::error_code {}", ec.message());
    }
}


/*
 * Utility for logging exception to be thrown
 */
static inline void LOG_EXCEPTION(boost::system::error_code const & ec)
{
    spdlog::error("Throwing exception: {}", ec.message());
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_LOGGING_HPP_ */
