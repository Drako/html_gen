#ifndef STRING_HXX
#define STRING_HXX

#include "list.hxx"

#include <cstddef>

namespace ct
{
    // single character
    template <char C>
    struct _char
    {
        constexpr static char const value = C;
    };

    // single int
    template <int I>
    struct _int
    {
        constexpr static char const value = I;
    };

    // a bool
    template <bool B>
    struct _bool
    {
        constexpr static bool const value = B;
    };

    // splits an int into up to 4 chars
    namespace detail
    {
        template <typename Char>
        struct is_nonzero;
        template <char C>
        struct is_nonzero<_char<C>>
        {
            constexpr static bool const value = C != '\0';
        };

        template <int N, char A = N & 0xFF, char B = (N >> 8) & 0xFF, char C = (N >> 16) & 0xFF, char D = (N >> 24) & 0xFF>
        struct to_chars
        {
            typedef grep_t<list<_char<D>, _char<C>, _char<B>, _char<A>>, is_nonzero> type;
        };
    }
    template <typename>
    struct to_chars;
    template <int N>
    struct to_chars<_int<N>>
        : detail::to_chars<N>
    {};

    // string holder
    template <int... Chars>
    struct string
    {
        typedef flatten_t<map_t<list<_int<Chars>...>, to_chars>> type;

        template <typename ListOfChars>
        struct expander;
        template <char... RealChars>
        struct expander<list<_char<RealChars>...>>
        {
            static char const * c_str()
            {
                static char const data[] = { RealChars..., '\0' };
                return data;
            }

            static std::size_t const length = sizeof...(RealChars);
        };

        static char const * c_str()
        {
            return expander<type>::c_str();
        }

        constexpr auto length() const
            -> _int<static_cast<int>(expander<type>::length)>
        {
            return {};
        }
    };

    // to upper
    namespace detail
    {
        template <typename Normalized>
        struct to_upper_impl;
        template <char... Chars>
        struct to_upper_impl<list<_char<Chars>...>>
        {
            typedef string<
                ((Chars >= 'a' && Chars <= 'z') ? Chars - 'a' + 'A' : Chars)...
            > type;
        };
    }

    template <typename String>
    struct to_upper
    {
        typedef typename detail::to_upper_impl<typename String::type>::type type;
    };
    template <typename String>
    using to_upper_t = typename to_upper<String>::type;

    // to lower
    namespace detail
    {
        template <typename Normalized>
        struct to_lower_impl;
        template <char... Chars>
        struct to_lower_impl<list<_char<Chars>...>>
        {
            typedef string<
                ((Chars >= 'A' && Chars <= 'Z') ? Chars - 'A' + 'a' : Chars)...
            > type;
        };
    }

    template <typename String>
    struct to_lower
    {
        typedef typename detail::to_lower_impl<typename String::type>::type type;
    };
    template <typename String>
    using to_lower_t = typename to_lower<String>::type;

    // string comparison (strcmp: case sensitive, stricmp: case insensitive)
    namespace detail
    {
        template <typename NormalizedA, typename NormalizedB>
        struct strcmp_impl;
        template <char HeadA, char... TailA, char HeadB, char... TailB>
        struct strcmp_impl<list<_char<HeadA>, _char<TailA>...>, list<_char<HeadB>, _char<TailB>...>>
        {
            constexpr static int const value = HeadA < HeadB ? -1 :
                                               HeadA > HeadB ? 1 :
                                               strcmp_impl<list<_char<TailA>...>, list<_char<TailB>...>>::value;
        };
        template <char HeadA, char... TailA>
        struct strcmp_impl<list<_char<HeadA>, _char<TailA>...>, list<>>
        {
            constexpr static int const value = 1;
        };
        template <char HeadB, char... TailB>
        struct strcmp_impl<list<>, list<_char<HeadB>, _char<TailB>...>>
        {
            constexpr static int const value = -1;
        };
        template <>
        struct strcmp_impl<list<>, list<>>
        {
            constexpr static int const value = 0;
        };
    }
    template <typename StringA, typename StringB>
    struct strcmp
    {
        constexpr static int const value = detail::strcmp_impl<typename StringA::type, typename StringB::type>::value;
    };
    template <typename StringA, typename StringB>
    struct stricmp
    {
        constexpr static int const value = strcmp<to_lower_t<StringA>, to_lower_t<StringB>>::value;
    };
    template <typename StringA, typename StringB>
    constexpr auto operator == (StringA const &, StringB const &) -> _bool<strcmp<StringA, StringB>::value == 0> { return {}; }
    template <typename StringA, typename StringB>
    constexpr auto operator != (StringA const &, StringB const &) -> _bool<strcmp<StringA, StringB>::value != 0> { return {}; }
    template <typename StringA, typename StringB>
    constexpr auto operator < (StringA const &, StringB const &) -> _bool<strcmp<StringA, StringB>::value == -1> { return {}; }
    template <typename StringA, typename StringB>
    constexpr auto operator > (StringA const &, StringB const &) -> _bool<strcmp<StringA, StringB>::value == 1> { return {}; }

    // string concatenating
    template <typename StringA, typename StringB>
    struct strcat;
    template <int... CharsA, int... CharsB>
    struct strcat<string<CharsA...>, string<CharsB...>>
    {
        typedef string<CharsA..., CharsB...> type;
    };
    template <typename StringA, typename StringB>
    using strcat_t = typename strcat<StringA, StringB>::type;
    template <typename StringA, typename StringB>
    constexpr auto operator + (StringA const &, StringB const &) -> strcat_t<StringA, StringB> { return {}; }

    // multiply
    template <typename String, unsigned Times>
    struct strmul
    {
        typedef strcat_t<
            String,
            typename strmul<String, Times - 1>::type
        > type;
    };
    template <typename String>
    struct strmul<String, 0u>
    {
        typedef string<> type;
    };
    template <typename String, unsigned Times>
    using strmul_t = typename strmul<String, Times>::type;
    template <typename String, typename Times>
    constexpr auto operator * (String const &, Times const &) -> strmul_t<String, Times::value> { return {}; };

    // string length
    namespace detail
    {
        template <typename Normalized>
        struct strlen_impl;
        template <typename... Chars>
        struct strlen_impl<list<Chars...>>
        {
            constexpr static std::size_t const value = sizeof...(Chars);
        };
    }
    template <typename String>
    struct strlen
    {
        constexpr static std::size_t const value = detail::strlen_impl<typename String::type>::value;
    };
}

#endif // STRING_HXX
