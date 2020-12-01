#pragma once

#ifndef LIB_INCLUDE_MESSAGE_HPP_
#define LIB_INCLUDE_MESSAGE_HPP_

#include <string>


namespace Ringbeller
{


struct string_body
{
    using value_type = std::string;
};


template<
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body
>
struct request
{
    typename Body::value_type command;
    typename Body::value_type args;
};


template<typename Body>
using basic_request = request<false, false, false, false, false, Body>;

template<typename Body>
using set_request = request<true, false, false, false, false, Body>;

template<typename Body>
using test_request = request<false, true, false, false, false, Body>;

template<typename Body>
using read_request = request<false, false, true, false, false, Body>;

template<typename Body>
using write_request = request<false, false, false, true, false, Body>;

template<typename Body>
using exec_request = request<false, false, false, false, true, Body>;


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGE_HPP_ */
