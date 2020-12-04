#pragma once

#ifndef APP_SRC_DO_RECEIVE_VCALL_HPP_
#define APP_SRC_DO_RECEIVE_VCALL_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run "Receive voice call" example
 */
int run_receive_vcall(
    std::string const & device,
    serial_port_param_set_t const & param_set);


#endif /* APP_SRC_DO_RECEIVE_VCALL_HPP_ */
