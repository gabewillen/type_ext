//
// Created by gwillen on 4/27/19.
//

#pragma once
#ifndef TUPLE_NT_TUPLE_H
#define TUPLE_NT_TUPLE_H

#include "tuple_algorithm.h"

namespace ext {

    template<auto ELEMENT>
    struct nt_element {};

    template <auto...>
    struct nt_tuple : std::tuple<> {
        using types = std::tuple<>;
    };

    namespace detail {

        template <class...>
        struct deduce_types;

        template <>
        struct deduce_types<nt_tuple<>> {
            using type = std::tuple<>;
            using types = type;
        };

        template <class TYPE, TYPE* ELEMENT, auto... ELEMENTS>
        struct deduce_types<nt_tuple<ELEMENT, ELEMENTS...>> {
            using type = std::tuple<TYPE*>;
            using types = typename ext::tuple_flatten_t<type, typename deduce_types<nt_tuple<ELEMENTS...>>::types>;
        };

        template <class OWNER, class TYPE, TYPE OWNER::*ELEMENT, auto... ELEMENTS>
        struct deduce_types<nt_tuple<ELEMENT, ELEMENTS...>> {
            using type = std::tuple<TYPE*>;
            using types = typename ext::tuple_flatten_t<type, typename deduce_types<nt_tuple<ELEMENTS...>>::types>;
        };

        template <class TUPLE>
        using deduce_types_t = typename deduce_types<TUPLE>::types;

    } // namespace detail

    template <auto ITEM, auto... ITEMS>
    struct nt_tuple<ITEM, ITEMS...> : detail::deduce_types_t<nt_tuple<ITEM, ITEMS...>> {
        using types = typename detail::deduce_types_t<nt_tuple>;

        using types::types;
    };

    template <template <auto...> class T, auto... ITEMS, std::size_t INDEX>
    struct tuple_element<T<ITEMS...>, INDEX> : ext::tuple_element<typename nt_tuple<ITEMS...>::types, INDEX> {};

    template <class...>
    struct tuple_size;

    template <auto... ITEMS>
    struct tuple_size<nt_tuple<ITEMS...>> : std::tuple_size<typename nt_tuple<ITEMS...>::types> {};

    template <std::size_t INDEX, auto... ELEMENTS>
    constexpr auto get(nt_tuple<ELEMENTS...>& tuple)
        -> decltype(std::get<INDEX>(static_cast<typename nt_tuple<ELEMENTS...>::types&>(tuple))) {
        return std::get<INDEX>(static_cast<typename nt_tuple<ELEMENTS...>::types&>(tuple));
    }

    template <auto... ITEMS>
    struct tuple_common_type<nt_tuple<ITEMS...>> : tuple_common_type<typename nt_tuple<ITEMS...>::types> {};

    template <auto ELEMENT, auto... NEXT_ELEMENTS, auto... PREV_ELEMENTS>
    struct tuple_iterator<nt_tuple<ELEMENT, NEXT_ELEMENTS...>, nt_tuple<PREV_ELEMENTS...>> {
        using value_type = nt_element<ELEMENT>;
    };

    namespace detail {

        template <auto... ELEMENTS>
        struct iterator_begin<nt_tuple<ELEMENTS...>> {
            using type = tuple_iterator<nt_tuple<ELEMENTS...>, nt_tuple<>>;
        };

        template <auto... PREV>
        struct iterator_end<nt_tuple<>, nt_tuple<PREV...>> {
            using type = tuple_iterator<nt_tuple<>, nt_tuple<PREV...>>;
        };

        template<>
        struct iterator_end<nt_tuple<>> : iterator_end<nt_tuple<>, nt_tuple<>> {};

        template <auto ELEMENT, auto... NEXT, auto... PREV>
        struct iterator_end<nt_tuple<ELEMENT, NEXT...>, nt_tuple<PREV...>>
            : iterator_end<nt_tuple<NEXT...>, nt_tuple<ELEMENT, PREV...>> {};

        template <auto ELEMENT, auto... ELEMENTS>
        struct iterator_end<nt_tuple<ELEMENT, ELEMENTS...>>
            : iterator_end<nt_tuple<ELEMENTS...>, nt_tuple<ELEMENT>> {};



        template < auto ELEMENT, auto... ELEMENTS, auto... PREV_ELEMENTS>
            struct iterator_increment<tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENTS...>>, 0> {
                using type = tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENTS...>>;
            };

        template < auto... PREV_ELEMENTS>
            struct iterator_increment<tuple_iterator<nt_tuple<>, nt_tuple<PREV_ELEMENTS...>>, 0> {
                using type = tuple_iterator<nt_tuple<>, nt_tuple<PREV_ELEMENTS...>>;
            };

        template < auto ELEMENT, auto... ELEMENTS, auto... PREV_ELEMENTS,
            auto OFFSET>
            struct iterator_increment<tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENTS...>>, OFFSET>
                : iterator_increment<tuple_iterator<nt_tuple<ELEMENTS...>,
                                                    nt_tuple<ELEMENT, PREV_ELEMENTS...>>,
                                     OFFSET - 1> {};


        template < auto ELEMENT, auto... ELEMENTS, auto PREV_ELEMENT,
                   auto... PREV_ELEMENTS>
            struct iterator_decrement<tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENT, PREV_ELEMENTS...>>,
                                      0> {
                using type = tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENT, PREV_ELEMENTS...>>;
            };

        template < auto ELEMENT, auto... ELEMENTS>
            struct iterator_decrement<tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<>>,
                                      0> {
                using type = tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<>>;
            };

        template < auto PREV_ELEMENT, auto... PREV_ELEMENTS, auto OFFSET>
            struct iterator_decrement<tuple_iterator<nt_tuple<>, nt_tuple<PREV_ELEMENT, PREV_ELEMENTS...>>, OFFSET>
                : iterator_decrement<tuple_iterator<nt_tuple<PREV_ELEMENT>, nt_tuple<PREV_ELEMENTS...>>, OFFSET + 1> {};

        template < auto ELEMENT, auto... ELEMENTS, auto PREV_ELEMENT,
                   auto... PREV_ELEMENTS, auto OFFSET>
            struct iterator_decrement<tuple_iterator<nt_tuple<ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENT, PREV_ELEMENTS...>>,
                                      OFFSET>
                : iterator_decrement<tuple_iterator<nt_tuple<PREV_ELEMENT, ELEMENT, ELEMENTS...>, nt_tuple<PREV_ELEMENTS...>>,
                                     OFFSET + 1> {};

    }; // namespace detail

//    template <class FIRST, class LAST, template <auto...> class OP, class RESULTS = void,
//        bool = std::is_same_v<FIRST, LAST>>
//        struct nt_tuple_transform;
//
//    template <class FIRST, class LAST, template <auto...> class OP, auto ...RESULTS>
//        struct nt_tuple_transform<FIRST, LAST, OP, nt_tuple<RESULTS...>, true> {
//            using type = nt_tuple<RESULTS...>;
//        };
//
//    template <class FIRST, class LAST, template <auto...> class OP, auto ...RESULTS>
//        struct nt_tuple_transform<FIRST, LAST, OP, nt_tuple<RESULTS...>, false> {
//            using type =
//            typename nt_tuple_transform<tuple_next<FIRST>, LAST, OP,
//                                     nt_tuple<RESULTS...,  OP<FIRST::value>::value>>::type;
//        };
//
//    template <class FIRST, class LAST, template <auto...> class OP>
//        struct nt_tuple_transform<FIRST, LAST, OP, void, false> {
//            using type = typename nt_tuple_transform<tuple_next<FIRST>, LAST, OP,
//                                                  nt_tuple<OP<FIRST::value>::value>>::type;
//        };
//
//    template <class FIRST, class LAST, template <auto...> class OP>
//        using nt_tuple_transform_t = typename ext::nt_tuple_transform<FIRST, LAST, OP>::type;

    template <template <auto...> class TUPLE, auto... ELEMENTS>
        struct tuple_flatten<TUPLE<ELEMENTS...>> {
            using type = TUPLE<ELEMENTS...>;
        };

    template <template <auto...> class TUPLE, auto... TUPLE1_ELEMENTS, auto... TUPLE2_ELEMENTS, class... TUPLES>
        struct tuple_flatten<TUPLE<TUPLE1_ELEMENTS...>, TUPLE<TUPLE2_ELEMENTS...>, TUPLES...>
            : tuple_flatten<TUPLE<TUPLE1_ELEMENTS..., TUPLE2_ELEMENTS...>, TUPLES...> {};
} // namespace ext

#endif // TUPLE_NT_TUPLE_H
