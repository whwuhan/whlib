#pragma once
#include <vector>
#include <string>
#include <iostream>
//分割字符串
namespace wh
{
    namespace utils
    {
        std::vector<std::string> split(const std::string &s, const std::string &seperator);
    } // namespace utils
} // namespace wh