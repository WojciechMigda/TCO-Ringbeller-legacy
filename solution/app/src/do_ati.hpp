#pragma once

#ifndef APP_SRC_DO_ATI_HPP_
#define APP_SRC_DO_ATI_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run ATI example
 */
int run_ati(std::string const & device, serial_port_param_set_t const & param_set);


#endif /* APP_SRC_DO_ATI_HPP_ */
