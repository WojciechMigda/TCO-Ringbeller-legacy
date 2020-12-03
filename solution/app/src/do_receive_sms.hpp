#pragma once

#ifndef APP_SRC_DO_RECEIVE_SMS_HPP_
#define APP_SRC_DO_RECEIVE_SMS_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run "Receive SMS" example
 */
int run_receive_sms(std::string const & device, serial_port_param_set_t const & param_set);


#endif /* APP_SRC_DO_RECEIVE_SMS_HPP_ */
