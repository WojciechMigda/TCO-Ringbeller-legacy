#pragma once

#ifndef LIB_INCLUDE_MESSAGE_HPP_
#define LIB_INCLUDE_MESSAGE_HPP_

#include <string>
#include <vector>


namespace Ringbeller
{


struct string_body
{
    using value_type = std::string;
};

struct vector_sequence
{
    template<typename T>
    using value_type = std::vector<T>;
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


enum result_code
{
    none,
    ok,
    connect,
    ring,
    no_carrier,
    error,
    no_dialtone,
    busy,
    no_answer,
    cme_error,
    cms_error,
    command_not_support,
    too_may_parameters
};

template<typename Text, typename Sequence>
struct response
{
    using text_type = typename Text::value_type;

    enum result_code result_code;
    text_type rc_text;
    typename Sequence::template value_type<text_type> body;
};


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGE_HPP_ */
