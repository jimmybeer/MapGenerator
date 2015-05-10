#pragma once
#include <string>   // std::string
#include <sstream>  // std::ostringstream

typedef int int32;

namespace std
{
    template <typename T> std::string to_string(const T& val)
    {
        std::ostringstream os;
        os << val;
        return os.str();
    }
}

template<typename Container>
void deleteContainerContents(Container &c)
{
    while(!c.empty())
    {
        delete c.back();
        c.pop_back();
    }
}