#ifndef ALGORITHM_HXX
#define ALGORITHM_HXX

namespace ct
{
    template <bool Condition, typename If, typename Else>
    struct _if
    {
        typedef Else type;
    };
    template <typename If, typename Else>
    struct _if<true, If, Else>
    {
        typedef If type;
    };
    template <bool Condition, typename If, typename Else>
    using if_t = typename _if<Condition, If, Else>::type;
}

#endif // ALGORITHM_HXX
