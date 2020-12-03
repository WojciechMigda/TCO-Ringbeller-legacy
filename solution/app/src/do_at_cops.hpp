#pragma once

#ifndef APP_SRC_DO_AT_COPS_HPP_
#define APP_SRC_DO_AT_COPS_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run AT+COPS=? example
 */
int run_at_cops(std::string const & device, serial_port_param_set_t const & param_set);


#endif /* APP_SRC_DO_AT_COPS_HPP_ */
