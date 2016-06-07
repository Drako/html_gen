#include "string.hxx"

#include <iostream>

int main()
{
    std::cout << decltype(
        ct::string<'Na'>{} * ct::_int<16>{} + ct::string<' Bat', 'man!'>{}
    )::c_str() << std::endl;

    std::cout << decltype(
        ct::to_upper_t<ct::string<'foo'>>{} == ct::string<'FOO'>{}
    )::value << std::endl;

    typedef ct::string<'Feli', 'x'> felix;
    std::cout << decltype(
        felix{} * felix{}.length()
    )::c_str() << std::endl;

    return 0;
}
