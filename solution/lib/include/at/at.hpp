#pragma once

#ifndef LIB_INCLUDE_AT_AT_HPP_
#define LIB_INCLUDE_AT_AT_HPP_

#include <string>


namespace AT
{


struct AT
{
    AT();

    std::string to_string() const;
};


AT::AT()
{

}


std::string AT::to_string() const
{
    return "AT";
}


} // namespace AT


#endif /* LIB_INCLUDE_AT_AT_HPP_ */
