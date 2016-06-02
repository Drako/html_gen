#include "string.hxx"

#include <iostream>

int main()
{
    std::cout <<
        decltype(
            ct::string<'Na'>{} * ct::_int<16>{} + ct::string<' Bat', 'man!'>{}
        )::c_str()
        << std::endl;
    return 0;
}
