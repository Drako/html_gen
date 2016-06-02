#ifndef LIST_HXX
#define LIST_HXX

#include "algorithm.hxx"

namespace ct
{
    // list holder
    template <typename...>
    struct list {};

    // append list to list
    template <typename ListA, typename ListB>
    struct append;
    template <typename... TypesA, typename... TypesB>
    struct append<list<TypesA...>, list<TypesB...>>
    {
        typedef list<TypesA..., TypesB...> type;
    };
    template <typename ListA, typename ListB>
    using append_t = typename append<ListA, ListB>::type;
    template <typename ListA, typename ListB>
    auto operator + (ListA const &, ListB const &) -> append_t<ListA, ListB> { return {}; };

    // map
    template <typename List, template <typename> class Predicate>
    struct map;
    template <typename... Types, template <typename> class Predicate>
    struct map<list<Types...>, Predicate>
    {
        typedef list<typename Predicate<Types>::type...> type;
    };
    template <typename List, template <typename> class Predicate>
    using map_t = typename map<List, Predicate>::type;

    // grep
    template <typename List, template <typename> class Predicate>
    struct grep;
    template <template <typename> class Predicate>
    struct grep<list<>, Predicate>
    {
        typedef list<> type;
    };
    template <typename Head, typename... Tail, template <typename> class Predicate>
    struct grep<list<Head, Tail...>, Predicate>
    {
        typedef if_t<
            Predicate<Head>::value,
            append_t<list<Head>, typename grep<list<Tail...>, Predicate>::type>,
            typename grep<list<Tail...>, Predicate>::type
        > type;
    };
    template <typename List, template <typename> class Predicate>
    using grep_t = typename grep<List, Predicate>::type;

    // flatten a list of lists to a list
    template <typename ListOfLists>
    struct flatten;
    template <>
    struct flatten<list<>>
    {
        typedef list<> type;
    };
    template <typename Head, typename... Tail>
    struct flatten<list<Head, Tail...>>
    {
        typedef append_t<Head, typename flatten<list<Tail...>>::type> type;
    };
    template <typename ListOfLists>
    using flatten_t = typename flatten<ListOfLists>::type;
}

#endif // LIST_HXX
