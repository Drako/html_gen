#include "string.hxx"

#include <iostream>

int main()
{
    std::cout <<
        ct::strcat_t<
            ct::strmul_t<ct::string<'Na'>, 16>,
            ct::string<' Bat', 'man!'>
        >::c_str()
        << std::endl;
    return 0;
}
