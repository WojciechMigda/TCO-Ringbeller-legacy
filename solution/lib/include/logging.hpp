#pragma once

#ifndef LIB_INCLUDE_LOGGING_HPP_
#define LIB_INCLUDE_LOGGING_HPP_

#include "spdlog/spdlog.h"

#include <boost/current_function.hpp>
#include <boost/system/error_code.hpp>


#define FN_ENTER() spdlog::trace("Fn Enter: {}", BOOST_CURRENT_FUNCTION)

#define FN_LEAVE() spdlog::trace("Fn Leave: {}", BOOST_CURRENT_FUNCTION)

static inline void LOG_CHECK_EC(boost::system::error_code & ec)
{
    if (ec)
    {
        spdlog::error("boost::system::error_code {}", ec.message());
    }
}


static inline void LOG_EXCEPTION(boost::system::error_code & ec)
{
    spdlog::error("Throwing exception: {}", ec.message());
}


#endif /* LIB_INCLUDE_LOGGING_HPP_ */
