#ifndef STRING_HXX
#define STRING_HXX

#include "list.hxx"

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
        };

        static char const * c_str()
        {
            return expander<type>::c_str();
        }
    };

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
    constexpr auto operator + (StringA const &, StringB const &) -> strcat_t<StringA, StringB> { return {}; };

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
}

#endif // STRING_HXX
