#pragma once

#ifndef LIB_INCLUDE_RESPONSE_HPP_
#define LIB_INCLUDE_RESPONSE_HPP_

#include "at/ok.hpp"
#include <variant>


namespace AT
{


using response_t = std::variant<
    AT::OK
>;


}  // namespace AT


#endif /* LIB_INCLUDE_RESPONSE_HPP_ */
