#pragma once

#ifndef APP_SRC_DO_AT_OK_HPP_
#define APP_SRC_DO_AT_OK_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run AT/OK example
 */
int run_at_ok(std::string const & device, serial_port_param_set_t const & param_set);


#endif /* APP_SRC_DO_AT_OK_HPP_ */
