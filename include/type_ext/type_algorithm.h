//
// Created by gwillen on 4/27/19.
//

#pragma once
#ifndef TUPLE_EXT_ALGORITHM_H
#define TUPLE_EXT_ALGORITHM_H

#include "type_iterator.h"
#include "type_functional.h"

namespace ext {
    //
    //    template <class...>
    //    struct type_merge;

    template <class FIRST, class LAST, template <class...> class, bool = std::is_same_v<FIRST, LAST>>
    struct type_count_if;

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct type_count_if<type_iterator<FIRST...>, type_iterator<LAST...>, UNARY_PREDICATE, true>
        : std::integral_constant<std::size_t, 0> {};

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct type_count_if<type_iterator<FIRST...>, type_iterator<LAST...>, UNARY_PREDICATE, false>
        : std::integral_constant<std::size_t, UNARY_PREDICATE<typename type_iterator<FIRST...>::value_type>::value +
                                                  type_count_if<type_next<type_iterator<FIRST...>>,
                                                                 type_iterator<LAST...>, UNARY_PREDICATE>::value> {};

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto type_count_if_v = type_count_if<FIRST, LAST, PRED>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    using type_all_of = std::bool_constant<type_count_if_v<FIRST, LAST, PRED> == type_distance_v<FIRST, LAST>>;

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto type_all_of_v = type_all_of<FIRST, LAST, PRED>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    using type_any_of = std::bool_constant<(type_count_if_v<FIRST, LAST, PRED>> 0)>;

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto type_any_of_v = type_any_of<FIRST, LAST, PRED>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    using type_none_of = std::bool_constant<!type_any_of_v<FIRST, LAST, PRED>>;

    template <class FIRST, class LAST, class TYPE>
    struct type_count {

        template <class VALUE>
        using predicate = std::is_same<TYPE, VALUE>;

        static constexpr auto value = type_count_if_v<FIRST, LAST, predicate>;
    };

    template <class FIRST, class LAST, class TYPE>
    static constexpr auto type_count_v = type_count<FIRST, LAST, TYPE>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto type_none_of_v = type_none_of<FIRST, LAST, PRED>::value;

    template <class, class, template <class...> class, class...>
    struct type_find_if;

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct type_find_if<type_iterator<FIRST...>, type_iterator<LAST...>, UNARY_PREDICATE, std::true_type> {
        using type = type_iterator<LAST...>;
    };

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct type_find_if<type_iterator<FIRST...>, type_iterator<LAST...>, UNARY_PREDICATE, std::false_type>
        : std::conditional<UNARY_PREDICATE<typename type_iterator<FIRST...>::value_type>::value,
                           type_iterator<FIRST...>,
                           typename type_find_if<type_next<type_iterator<FIRST...>>, type_iterator<LAST...>,
                                                  UNARY_PREDICATE>::type> {};

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct type_find_if<type_iterator<FIRST...>, type_iterator<LAST...>, UNARY_PREDICATE>
        : type_find_if<type_iterator<FIRST...>, type_iterator<LAST...>, UNARY_PREDICATE,
                        typename std::is_same<type_iterator<FIRST...>, type_iterator<LAST...>>::type> {};

    template <class FIRST, class LAST, template <class...> class PRED>
    using type_find_if_t = typename type_find_if<FIRST, LAST, PRED>::type;

    template <class FIRST, class LAST, class VAL>
    struct type_find {

        template <class TYPE>
        using partial_is_same = std::is_same<VAL, TYPE>;

        using type = typename type_find_if<FIRST, LAST, partial_is_same>::type;
    };

    template <class FIRST, class LAST, class VAL>
    using type_find_t = typename type_find<FIRST, LAST, VAL>::type;

    template <class FIRST, class LAST, template <class...> class OP, class RESULTS = void,
              bool = std::is_same_v<FIRST, LAST>>
    struct type_transform;

    template <class FIRST, class LAST, template <class...> class OP, template <class ...> class CONTAINER, class... TYPES>
    struct type_transform<FIRST, LAST, OP, CONTAINER<TYPES...>, true> {
        using type = CONTAINER<TYPES...>;
    };

    template <class FIRST, class LAST, template <class...> class OP, template <class ...> class CONTAINER, class... RESULTS>
    struct type_transform<FIRST, LAST, OP, CONTAINER<RESULTS...>, false> {
        using type =
            typename type_transform<type_next<FIRST>, LAST, OP,
                                     CONTAINER<RESULTS..., typename OP<typename FIRST::value_type>::type>>::type;
    };

    template <class FIRST, class LAST, template <class...> class OP>
    struct type_transform<FIRST, LAST, OP, void, true> {
        using type = typename LAST::template container_type<>;
    };

    template <class FIRST, class LAST, template <class...> class OP>
    struct type_transform<FIRST, LAST, OP, void, false> {
        using type = typename type_transform<type_next<FIRST>, LAST, OP,
                                              typename LAST::template container_type<typename OP<typename FIRST::value_type>::type>>::type;
    };

    template <class FIRST, class LAST, template <class...> class OP>
    using type_transform_t = typename ext::type_transform<FIRST, LAST, OP>::type;

    template <class FIRST, class LAST, class RESULTS = void, bool = std::is_same_v<FIRST, LAST>>
    struct type_copy;

    template <class... FIRST, class... LAST, template <class ...> class CONTAINER, class... TYPES>
    struct type_copy<type_iterator<FIRST...>, type_iterator<LAST...>, CONTAINER<TYPES...>, true> {
        using type = CONTAINER<TYPES...>;
    };

    template <class... FIRST, class... LAST, template <class ...> class CONTAINER, class... TYPES>
    struct type_copy<type_iterator<FIRST...>, type_iterator<LAST...>, CONTAINER<TYPES...>, false> {
        using type = typename type_copy<type_next<type_iterator<FIRST...>>, type_iterator<LAST...>,
                                         CONTAINER<TYPES..., typename type_iterator<FIRST...>::value_type>>::type;
    };

    template <class FIRST, class LAST, template <class...> class PRED, class RESULTS = void,
              bool = std::is_same_v<FIRST, LAST>>
    struct type_copy_if;

    template <class FIRST, class LAST, template <class...> class PRED>
    struct type_copy_if<FIRST, LAST, PRED, void, true> {
        using type = typename LAST::template container_type<>;
    };

    template <class FIRST, class LAST, template <class...> class PRED, template <class ...> class CONTAINER, class... TYPES>
    struct type_copy_if<FIRST, LAST, PRED, CONTAINER<TYPES...>, true> {
        using type = CONTAINER<TYPES...>;
    };

    template <class FIRST, class LAST, template <class...> class PRED, template <class ...> class CONTAINER, class... RESULTS>
    struct type_copy_if<FIRST, LAST, PRED, CONTAINER<RESULTS...>, false> {
        using type =
            typename type_copy_if<type_next<FIRST>, LAST, PRED,
                                   typename std::conditional_t<PRED<typename FIRST::value_type>::value,
                                                               CONTAINER<RESULTS..., typename FIRST::value_type>,
                                                               CONTAINER<RESULTS...>>>::type;
    };

    template <class FIRST, class LAST, template <class...> class PRED>
    struct type_copy_if<FIRST, LAST, PRED, void, false> : type_copy_if<FIRST, LAST, PRED, typename FIRST::template container_type<>> {};

    template <class FIRST, class LAST, template <class...> class PRED>
    using type_copy_if_t = typename type_copy_if<FIRST, LAST, PRED>::type;

    template <class FIRST, class LAST, class CONTAINER = typename FIRST::template container_type<>, bool = std::is_same_v<FIRST, LAST>>
    struct type_reverse;

    template <class... FIRST, class... LAST, template <class ...> class CONTAINER,  class... RESULTS>
    struct type_reverse<type_iterator<FIRST...>, type_iterator<LAST...>, CONTAINER<RESULTS...>, true> {
        using type = CONTAINER<RESULTS...>;
    };

    template <class... FIRST, class... LAST, template <class ...> class CONTAINER, class... RESULTS>
    struct type_reverse<type_iterator<FIRST...>, type_iterator<LAST...>, CONTAINER<RESULTS...>, false> {
        using type = typename type_reverse<
            type_iterator<FIRST...>, type_prev<type_iterator<LAST...>>,
            CONTAINER<RESULTS..., typename type_prev<type_iterator<LAST...>>::value_type>>::type;
    };

    template <class BEGIN1, class END1, class BEGIN2, class END2, class = typename BEGIN1::template container_type<>,
              bool = std::is_same_v<BEGIN1, END1>>
    struct type_set_intersection;

    template <class BEGIN1, class END1, class BEGIN2, class END2, template <class ...> class CONTAINER, class... ELEMENTS>
    struct type_set_intersection<BEGIN1, END1, BEGIN2, END2, CONTAINER<ELEMENTS...>, true> {
        //        using type = std::conditional_t<std::
        using type = CONTAINER<ELEMENTS...>;
    };

    template <class BEGIN1, class END1, class BEGIN2, class END2, template <class ...> class CONTAINER, class... ELEMENTS>
    struct type_set_intersection<BEGIN1, END1, BEGIN2, END2, CONTAINER<ELEMENTS...>, false> {
        //        using type = std::conditional_t<std::

        using type = typename type_set_intersection<
            type_next<BEGIN1>, END1, BEGIN2, END2,
            std::conditional_t<
                type_any_of_v<BEGIN2, END2,
                               type_bind<std::is_same, typename BEGIN1::value_type, placeholder<1>>::template type> &&
                    type_none_of_v<
                        type_begin<CONTAINER<ELEMENTS...>>, type_end<CONTAINER<ELEMENTS...>>,
                        type_bind<std::is_same, typename BEGIN1::value_type, placeholder<1>>::template type>,
                CONTAINER<ELEMENTS..., typename BEGIN1::value_type>, CONTAINER<ELEMENTS...>>>::type;
    };

    template <class BEGIN1, class END1, class BEGIN2, class END2, class = typename BEGIN1::template container_type<>,
              bool = std::is_same_v<BEGIN1, END1>>
    struct type_set_difference;

    template <class BEGIN1, class END1, class BEGIN2, class END2, template <class ...> class CONTAINER, class... ELEMENTS>
    struct type_set_difference<BEGIN1, END1, BEGIN2, END2, CONTAINER<ELEMENTS...>, true> {
        //        using type = std::conditional_t<std::
        using type = CONTAINER<ELEMENTS...>;
    };

    template <class BEGIN1, class END1, class BEGIN2, class END2, template <class ...> class CONTAINER, class ...ELEMENTS>
    struct type_set_difference<BEGIN1, END1, BEGIN2, END2, CONTAINER<ELEMENTS...>, false>
        : type_set_difference<
              type_next<BEGIN1>, END1, BEGIN2, END2,
              std::conditional_t<
                  type_none_of_v<
                      BEGIN2, END2,
                      type_bind<std::is_same, typename BEGIN1::value_type, placeholder<1>>::template type> &&
                      type_none_of_v<
                          type_begin<CONTAINER<ELEMENTS...>>, type_end<CONTAINER<ELEMENTS...>>,
                          type_bind<std::is_same, typename BEGIN1::value_type, placeholder<1>>::template type>,
                  CONTAINER<ELEMENTS..., typename BEGIN1::value_type>, CONTAINER<ELEMENTS...>>> {};

    template <class BEGIN, class END, template <class...> class COMP = std::is_same, bool = std::is_same_v<BEGIN, END>>
    struct type_sort;

    //    template<class BEGIN, class END, template <class ...> class COMP, class ...SORTED>
    //        struct type_sort<BEGIN, END, COMP, std::tuple<SORTED...>, false> {
    //
    //            using current = typename BEGIN::value_type;
    //            using prev = typename type_prev<type_end<std::tuple<SORTED...>>>::value_type;
    //            using sorted = std::tuple<SORTED...>;
    //
    //            using type = typename type_sort<type_next<BEGIN>, END, COMP,
    //            std::conditional_t<COMP<current, prev>::value, std::tuple<current, SORTED...>, std::tuple<SORTED...,
    //            current>>>::type;
    //        };

    template <class BEGIN, class END, template <class...> class COMP>
    struct type_sort<BEGIN, END, COMP, true> {

        using type = typename BEGIN::template container_type<>;
    };

    template <class BEGIN, class END, template <class...> class COMP>
    struct type_sort<BEGIN, END, COMP, false> {

        using sorted =
            type_copy_if_t<BEGIN, END, type_bind<COMP, typename BEGIN::value_type, placeholder<1>>::template type>;

        template <class ELEMENT>
        struct differentiate : type_none_of<type_begin<sorted>, type_end<sorted>,
                                             type_bind<std::is_same, ELEMENT, placeholder<1>>::template type> {};
        using unsorted = typename type_copy_if<BEGIN, END, differentiate>::type;

        using type =
            typename type_flatten<sorted,
                                   typename type_sort<type_begin<unsorted>, type_end<unsorted>, COMP>::type>::type;
    };

    template <class BEGIN, class END, template <class...> class PRED = std::is_same, class = typename BEGIN::template container_type<>,
              bool = std::is_same_v<BEGIN, END>>
    struct type_unique;

    template <class BEGIN, class END, template <class...> class PRED,template <class ...> class CONTAINER, class... UNIQUE>
    struct type_unique<BEGIN, END, PRED, CONTAINER<UNIQUE...>, true> {
        using type = CONTAINER<UNIQUE...>;
    };

    template <class BEGIN, class END, template <class...> class PRED,template <class ...> class CONTAINER, class... UNIQUE>
    struct type_unique<BEGIN, END, PRED, CONTAINER<UNIQUE...>, false> {
        using unique = CONTAINER<UNIQUE...>;
        using type = typename type_unique<
            type_next<BEGIN>, END, PRED,
            std::conditional_t<
                type_none_of_v<type_begin<unique>, type_end<unique>,
                                type_bind<PRED, typename BEGIN::value_type, placeholder<1>>::template type>,
                CONTAINER<UNIQUE..., typename BEGIN::value_type>, CONTAINER<UNIQUE...>>>::type;
    };
} // namespace ext

#endif // TUPLE_EXT_ALGORITHM_H
