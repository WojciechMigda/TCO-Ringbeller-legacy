#pragma once

#ifndef APP_SRC_DO_SEND_SMS_HPP_
#define APP_SRC_DO_SEND_SMS_HPP_

#include "serial_port_setup.hpp"

#include <string>


/*
 * Run "Send SMS" example
 */
int run_send_sms(
    std::string const & device,
    serial_port_param_set_t const & param_set,
    std::string const & da,
    std::string const & sms_text);


#endif /* APP_SRC_DO_SEND_SMS_HPP_ */
