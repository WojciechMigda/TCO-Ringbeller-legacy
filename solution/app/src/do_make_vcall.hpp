#pragma once

#ifndef APP_SRC_DO_MAKE_VCALL_HPP_
#define APP_SRC_DO_MAKE_VCALL_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run "Make voice call" example
 */
int run_make_vcall(
    std::string const & device,
    serial_port_param_set_t const & param_set,
    std::string const & da);


#endif /* APP_SRC_DO_MAKE_VCALL_HPP_ */
