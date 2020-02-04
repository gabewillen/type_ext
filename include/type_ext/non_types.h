//
// Created by gwillen on 4/27/19.
//

#pragma once
#ifndef TUPLE_NT_TUPLE_H
#define TUPLE_NT_TUPLE_H

#include "type_algorithm.h"

namespace ext {

    template <auto ELEMENT>
    struct non_type {};

    template <auto...>
    struct non_types : types<> {
        using types = ext::types<>;
    };

    namespace detail {

        template <class...>
        struct deduce_types;

        template <>
        struct deduce_types<non_types<>> {
            using type = std::tuple<>;
            using types = type;
        };

        template <class TYPE, TYPE* ELEMENT, auto... ELEMENTS>
        struct deduce_types<non_types<ELEMENT, ELEMENTS...>> {
            using type = std::tuple<TYPE*>;
            using types = typename ext::type_flatten_t<type, typename deduce_types<non_types<ELEMENTS...>>::types>;
        };

        template <class OWNER, class TYPE, TYPE OWNER::*ELEMENT, auto... ELEMENTS>
        struct deduce_types<non_types<ELEMENT, ELEMENTS...>> {
            using type = std::tuple<TYPE*>;
            using types = typename ext::type_flatten_t<type, typename deduce_types<non_types<ELEMENTS...>>::types>;
        };

        template <class TUPLE>
        using deduce_types_t = typename deduce_types<TUPLE>::types;

    } // namespace detail

    template <auto ITEM, auto... ITEMS>
    struct non_types<ITEM, ITEMS...> : detail::deduce_types_t<non_types<ITEM, ITEMS...>> {
        using types = typename detail::deduce_types_t<non_types>;

        using types::types;
    };

    template <template <auto...> class T, auto... ITEMS, std::size_t INDEX>
    struct type_element<T<ITEMS...>, INDEX> : ext::type_element<typename non_types<ITEMS...>::types, INDEX> {};

    template <class...>
    struct type_size;

    template <template <auto...> class CONTAINER, auto... ITEMS>
    struct type_size<CONTAINER<ITEMS...>> : std::tuple_size<typename non_types<ITEMS...>::types> {};

    template <std::size_t INDEX, template <auto...> class CONTAINER, auto... ELEMENTS>
    constexpr auto get(CONTAINER<ELEMENTS...>& tuple)
        -> decltype(std::get<INDEX>(static_cast<typename non_types<ELEMENTS...>::types&>(tuple))) {
        return std::get<INDEX>(static_cast<typename non_types<ELEMENTS...>::types&>(tuple));
    }

    template <template <auto...> class CONTAINER, auto... ITEMS>
    struct type_common_type<CONTAINER<ITEMS...>> : type_common_type<typename non_types<ITEMS...>::types> {};

    template <template <auto...> class CONTAINER, auto ELEMENT, auto... NEXT_ELEMENTS, auto... PREV_ELEMENTS>
    struct type_iterator<CONTAINER<ELEMENT, NEXT_ELEMENTS...>, CONTAINER<PREV_ELEMENTS...>> {
        using value_type = decltype(ELEMENT);

        template <auto... ELEMENTS>
        using container_type = CONTAINER<ELEMENTS...>;
    };

    template <template <auto...> class CONTAINER, auto... PREV_ELEMENTS>
    struct type_iterator<CONTAINER<>, CONTAINER<PREV_ELEMENTS...>> {
        using value_type = decltype(nullptr);

        template <auto... ELEMENTS>
        using container_type = CONTAINER<ELEMENTS...>;
    };

    namespace detail {

        template <auto... ELEMENTS>
        struct iterator_begin<non_types<ELEMENTS...>> {
            using type = type_iterator<non_types<ELEMENTS...>, non_types<>>;
        };

        template <auto... PREV>
        struct iterator_end<non_types<>, non_types<PREV...>> {
            using type = type_iterator<non_types<>, non_types<PREV...>>;
        };

        template <>
        struct iterator_end<non_types<>> : iterator_end<non_types<>, non_types<>> {};

        template <auto ELEMENT, auto... NEXT, auto... PREV>
        struct iterator_end<non_types<ELEMENT, NEXT...>, non_types<PREV...>>
            : iterator_end<non_types<NEXT...>, non_types<ELEMENT, PREV...>> {};

        template <auto ELEMENT, auto... ELEMENTS>
        struct iterator_end<non_types<ELEMENT, ELEMENTS...>>
            : iterator_end<non_types<ELEMENTS...>, non_types<ELEMENT>> {};

        template <auto ELEMENT, auto... ELEMENTS, auto... PREV_ELEMENTS>
        struct iterator_increment<type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENTS...>>, 0> {
            using type = type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENTS...>>;
        };

        template <auto... PREV_ELEMENTS>
        struct iterator_increment<type_iterator<non_types<>, non_types<PREV_ELEMENTS...>>, 0> {
            using type = type_iterator<non_types<>, non_types<PREV_ELEMENTS...>>;
        };

        template <auto ELEMENT, auto... ELEMENTS, auto... PREV_ELEMENTS, auto OFFSET>
        struct iterator_increment<type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENTS...>>, OFFSET>
            : iterator_increment<type_iterator<non_types<ELEMENTS...>, non_types<ELEMENT, PREV_ELEMENTS...>>,
                                 OFFSET - 1> {};

        template <auto ELEMENT, auto... ELEMENTS, auto PREV_ELEMENT, auto... PREV_ELEMENTS>
        struct iterator_decrement<
            type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENT, PREV_ELEMENTS...>>, 0> {
            using type = type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENT, PREV_ELEMENTS...>>;
        };

        template <auto ELEMENT, auto... ELEMENTS>
        struct iterator_decrement<type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<>>, 0> {
            using type = type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<>>;
        };

        template <auto PREV_ELEMENT, auto... PREV_ELEMENTS, auto OFFSET>
        struct iterator_decrement<type_iterator<non_types<>, non_types<PREV_ELEMENT, PREV_ELEMENTS...>>, OFFSET>
            : iterator_decrement<type_iterator<non_types<PREV_ELEMENT>, non_types<PREV_ELEMENTS...>>, OFFSET + 1> {};

        template <auto ELEMENT, auto... ELEMENTS, auto PREV_ELEMENT, auto... PREV_ELEMENTS, auto OFFSET>
        struct iterator_decrement<
            type_iterator<non_types<ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENT, PREV_ELEMENTS...>>, OFFSET>
            : iterator_decrement<
                  type_iterator<non_types<PREV_ELEMENT, ELEMENT, ELEMENTS...>, non_types<PREV_ELEMENTS...>>,
                  OFFSET + 1> {};

    }; // namespace detail

    //    template <class FIRST, class LAST, template <auto...> class OP, class RESULTS = void,
    //        bool = std::is_same_v<FIRST, LAST>>
    //        struct nt_type_transform;
    //
    //    template <class FIRST, class LAST, template <auto...> class OP, auto ...RESULTS>
    //        struct nt_type_transform<FIRST, LAST, OP, non_types<RESULTS...>, true> {
    //            using type = non_types<RESULTS...>;
    //        };
    //
    //    template <class FIRST, class LAST, template <auto...> class OP, auto ...RESULTS>
    //        struct nt_type_transform<FIRST, LAST, OP, non_types<RESULTS...>, false> {
    //            using type =
    //            typename nt_type_transform<type_next<FIRST>, LAST, OP,
    //                                     non_types<RESULTS...,  OP<FIRST::value>::value>>::type;
    //        };
    //
    //    template <class FIRST, class LAST, template <auto...> class OP>
    //        struct nt_type_transform<FIRST, LAST, OP, void, false> {
    //            using type = typename nt_type_transform<type_next<FIRST>, LAST, OP,
    //                                                  non_types<OP<FIRST::value>::value>>::type;
    //        };
    //
    //    template <class FIRST, class LAST, template <auto...> class OP>
    //        using nt_type_transform_t = typename ext::nt_type_transform<FIRST, LAST, OP>::type;


    template <template <auto...> class TUPLE, auto... ELEMENTS>
    struct type_flatten<TUPLE<ELEMENTS...>> {
        using type = TUPLE<ELEMENTS...>;
    };

    template <template <auto...> class TUPLE, auto... TUPLE1_ELEMENTS, auto... TUPLE2_ELEMENTS, class... TUPLES>
    struct type_flatten<TUPLE<TUPLE1_ELEMENTS...>, TUPLE<TUPLE2_ELEMENTS...>, TUPLES...>
        : type_flatten<TUPLE<TUPLE1_ELEMENTS..., TUPLE2_ELEMENTS...>, TUPLES...> {};

//    template<class BEGIN, class END, template<class ...> class PRED, template <auto ...> class CONTAINER, auto ...ELEMENTS>
//        struct type_unique<BEGIN, END, PRED, CONTAINER<ELEMENTS...>, true> {
//            using type = CONTAINER<ELEMENTS...>;
//        };
} // namespace ext

#endif // TUPLE_NT_TUPLE_H
