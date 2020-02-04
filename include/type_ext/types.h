//
// Created by gwillen on 8/13/19.
//

#pragma once
#ifndef TYPE_EXT_TYPES_H
#define TYPE_EXT_TYPES_H

#include <tuple>

#include <tuple>

namespace ext {

    namespace detail {

        template <class...>
        struct deduce_container_type;

        template <template <class...> class CONTAINER, class... TYPES>
        struct deduce_container_type<CONTAINER<TYPES...>> {
            using type = CONTAINER<>;
        };

        template <class CONTAINER>
        using deduce_container_type_t = typename deduce_container_type<CONTAINER>::type;
    } // namespace detail

    template <class... TYPES>
    struct type_container;

    template <class CONTAINER, class = detail::deduce_container_type_t<CONTAINER>>
    struct type_iterator;

    template <class...>
    struct type_distance;

    template <class... CONTAINERS>
    struct type_flatten;

    namespace detail {

        template <class ITER, auto, bool = true>
        struct iterator_advance;

        template <class...>
        struct iterator_begin;

        template <class...>
        struct iterator_end;

        template <class... ELEMENTS>
        struct to_types {
            using type = type_container<ELEMENTS...>;
        };

        template <class... BEGIN, class... END>
        struct to_types<type_iterator<BEGIN...>, type_iterator<END...>, std::true_type> {
            using type = type_container<>;
        };

        template <class... BEGIN, class... END>
        struct to_types<type_iterator<BEGIN...>, type_iterator<END...>, std::false_type>
            : type_flatten<std::tuple<typename type_iterator<BEGIN...>::value_type>,
                           typename to_types<typename iterator_advance<type_iterator<BEGIN...>, 1>::type,
                                             type_iterator<END...>>::type> {};

        template <class... BEGIN, class... END>
        struct to_types<type_iterator<BEGIN...>, type_iterator<END...>>
            : to_types<type_iterator<BEGIN...>, type_iterator<END...>,
                       typename std::is_same<type_iterator<BEGIN...>, type_iterator<END...>>::type> {};

        template <class TYPE, class BEGIN2, class END2, bool = std::is_same_v<BEGIN2, END2>>
        struct type_inserter;

        template <template <class...> class TYPE, class... ELEMENTS, class BEGIN2, class END2>
        struct type_inserter<TYPE<ELEMENTS...>, BEGIN2, END2, true> {
            using type = TYPE<ELEMENTS...>;
        };

        template <template <class...> class TYPE, class... ELEMENTS, class BEGIN2, class END2>
        struct type_inserter<TYPE<ELEMENTS...>, BEGIN2, END2, false>
            : type_inserter<TYPE<ELEMENTS..., typename BEGIN2::value_type>, typename iterator_advance<BEGIN2, 1>::type,
                            END2> {};

    } // namespace detail

    template <class... TYPES>
    struct type_container : std::tuple<TYPES...> {
        using begin = typename detail::iterator_begin<type_container>::type;
        using end = typename detail::iterator_end<type_container>::type;
    };

    template <class...>
    struct type_flatten;

    template <template <class...> class CONTAINER, class... ELEMENTS>
    struct type_flatten<CONTAINER<ELEMENTS...>> {
        using type = CONTAINER<ELEMENTS...>;
    };

//    template <template <class...> class CONTAINER, class... ELEMENTS>
//    struct type_flatten<CONTAINER<ELEMENTS...>> {
//        using type = typename type_flatten<CONTAINER<ELEMENTS...>, ELEMENTS.>::type;
//    };

    template <template <class...> class CONTAINER, class ELEMENT, class... ELEMENTS, class... CONTAINERS>
    struct type_flatten<CONTAINER<ELEMENT, ELEMENTS...>, CONTAINER<>, CONTAINERS...>
        : type_flatten<CONTAINER<ELEMENT, ELEMENTS...>, CONTAINERS...> {};

//    template <template <class...> class CONTAINER, class ELEMENT, class... ELEMENTS, class... CONTAINERS>
//    struct type_flatten<ELEMENT, CONTAINER<ELEMENTS...>, CONTAINERS...>
//        : type_flatten<CONTAINER<ELEMENT, ELEMENTS...>, CONTAINERS...> {};

    template <template <class...> class CONTAINER, class... ELEMENTS, class ELEMENT, class... CONTAINERS>
    struct type_flatten<CONTAINER<ELEMENTS...>, ELEMENT, CONTAINERS...>
        : type_flatten<CONTAINER<ELEMENTS..., ELEMENT>, CONTAINERS...> {};

    template <template <class...> class CONTAINER, class... TUPLE1_ELEMENTS, class... TUPLE2_ELEMENTS, class... CONTAINERS>
    struct type_flatten<CONTAINER<TUPLE1_ELEMENTS...>, CONTAINER<TUPLE2_ELEMENTS...>, CONTAINERS...> {

        using type = std::conditional_t<std::is_base_of_v<std::tuple<>, CONTAINER<>>, typename type_flatten<CONTAINER<TUPLE1_ELEMENTS..., TUPLE2_ELEMENTS...>, CONTAINERS...>::type, typename type_flatten<std::tuple<CONTAINER<TUPLE1_ELEMENTS...>, CONTAINER<TUPLE2_ELEMENTS...>>, CONTAINERS...>::type>;
    };

    template <class... CONTAINERS>
    using type_flatten_t = typename type_flatten<CONTAINERS...>::type;

    template <class... ELEMENTS>
    using types = typename detail::to_types<ELEMENTS...>::type;
    //
    template <class...>
    struct type_size;

    template <template <class ...>class CONTAINER, class... ELEMENTS>
    struct type_size<CONTAINER<ELEMENTS...>> : std::tuple_size<std::tuple<ELEMENTS...>> {};

    template <class TUPLE>
    constexpr auto type_size_v = ext::type_size<TUPLE>::value;

    template <class TYPE, class BEGIN2, class END2>
    using type_insert = typename detail::type_inserter<TYPE, BEGIN2, END2>::type;

} // namespace ext

#endif // TYPE_EXT_TYPES_H
