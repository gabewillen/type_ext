//
// Created by gwillen on 8/13/19.
//

#pragma once
#ifndef TYPE_EXT_TUPLE_H
#define TYPE_EXT_TUPLE_H

#include <tuple>

#include <tuple>

namespace ext {

    template <class, class = std::tuple<>>
    struct tuple_iterator;

    template <class...>
    struct tuple_distance;

    template <class... TUPLES>
    struct tuple_flatten;

    namespace detail {

        template <class, auto, bool = true>
        struct iterator_advance;

        template <class...>
        struct iterator_begin;

        template <class...>
        struct iterator_end;

        template <class... ELEMENTS>
        struct to_tuple {
            using type = std::tuple<ELEMENTS...>;
        };

        template <class... BEGIN, class... END>
        struct to_tuple<tuple_iterator<BEGIN...>, tuple_iterator<END...>, std::true_type> {
            using type = std::tuple<>;
        };

        template <class... BEGIN, class... END>
        struct to_tuple<tuple_iterator<BEGIN...>, tuple_iterator<END...>, std::false_type>
            : tuple_flatten<std::tuple<typename tuple_iterator<BEGIN...>::value_type>,
                            typename to_tuple<typename iterator_advance<tuple_iterator<BEGIN...>, 1>::type,
                                              tuple_iterator<END...>>::type> {};

        template <class... BEGIN, class... END>
        struct to_tuple<tuple_iterator<BEGIN...>, tuple_iterator<END...>>
            : to_tuple<tuple_iterator<BEGIN...>, tuple_iterator<END...>,
                       typename std::is_same<tuple_iterator<BEGIN...>, tuple_iterator<END...>>::type> {};

    } // namespace detail

    template <class... ELEMENTS>
    struct tuple_flatten {
        using type = std::tuple<ELEMENTS...>;
    };

    template <class... ELEMENTS>
    struct tuple_flatten<std::tuple<ELEMENTS...>> {
        using type = typename tuple_flatten<ELEMENTS...>::type;
    };

    template <class... ELEMENTS, class... TUPLES>
    struct tuple_flatten<std::tuple<ELEMENTS...>, std::tuple<>, TUPLES...>
        : tuple_flatten<std::tuple<ELEMENTS...>, TUPLES...> {};

    template <class ELEMENT, class... ELEMENTS, class... TUPLES>
    struct tuple_flatten<ELEMENT, std::tuple<ELEMENTS...>, TUPLES...>
        : tuple_flatten<std::tuple<ELEMENT, ELEMENTS...>, TUPLES...> {};

    template <class... ELEMENTS, class ELEMENT, class... TUPLES>
    struct tuple_flatten<std::tuple<ELEMENTS...>, ELEMENT, TUPLES...>
        : tuple_flatten<std::tuple<ELEMENTS..., ELEMENT>, TUPLES...> {};

    template <class... TUPLE1_ELEMENTS, class... TUPLE2_ELEMENTS, class... TUPLES>
    struct tuple_flatten<std::tuple<TUPLE1_ELEMENTS...>, std::tuple<TUPLE2_ELEMENTS...>, TUPLES...>
        : tuple_flatten<std::tuple<TUPLE1_ELEMENTS..., TUPLE2_ELEMENTS...>, TUPLES...> {};

    template <class... TUPLES>
    using tuple_flatten_t = typename tuple_flatten<TUPLES...>::type;

    template <class... ELEMENTS>
    using tuple = typename detail::to_tuple<ELEMENTS...>::type;

    template <class...>
    struct tuple_size;

    template <class... ELEMENTS>
    struct tuple_size<std::tuple<ELEMENTS...>> : std::tuple_size<std::tuple<ELEMENTS...>> {};

    template <class TUPLE>
    constexpr auto tuple_size_v = ext::tuple_size<TUPLE>::value;

} // namespace ext

#endif // TYPE_EXT_TUPLE_H
