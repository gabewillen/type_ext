//
// Created by gwillen on 4/27/19.
//

#pragma once
#ifndef TUPLE_EXT_ALGORITHM_H
#define TUPLE_EXT_ALGORITHM_H

#include "tuple_iterator.h"
#include "type_functional.h"

namespace ext {

    template <class...>
    struct tuple_merge;

    template <class FIRST, class LAST, template <class...> class, bool = std::is_same_v<FIRST, LAST>>
    struct tuple_count_if;

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct tuple_count_if<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, UNARY_PREDICATE, true>
        : std::integral_constant<std::size_t, 0> {};

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct tuple_count_if<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, UNARY_PREDICATE, false>
        : std::integral_constant<std::size_t, UNARY_PREDICATE<typename tuple_iterator<FIRST...>::value_type>::value +
                                                  tuple_count_if<tuple_next<tuple_iterator<FIRST...>>,
                                                                 tuple_iterator<LAST...>, UNARY_PREDICATE>::value> {};

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto tuple_count_if_v = tuple_count_if<FIRST, LAST, PRED>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    using tuple_all_of = std::bool_constant<tuple_count_if_v<FIRST, LAST, PRED> == tuple_distance_v<FIRST, LAST>>;

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto tuple_all_of_v = tuple_all_of<FIRST, LAST, PRED>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    using tuple_any_of = std::bool_constant<(tuple_count_if_v<FIRST, LAST, PRED>> 0)>;

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto tuple_any_of_v = tuple_any_of<FIRST, LAST, PRED>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    using tuple_none_of = std::bool_constant<!tuple_any_of_v<FIRST, LAST, PRED>>;

    template <class FIRST, class LAST, class TYPE>
    struct tuple_count {

        template <class VALUE>
        using predicate = std::is_same<TYPE, VALUE>;

        static constexpr auto value = tuple_count_if_v<FIRST, LAST, predicate>;
    };

    template<class FIRST, class LAST, class TYPE>
        static constexpr auto tuple_count_v = tuple_count<FIRST, LAST, TYPE>::value;

    template <class FIRST, class LAST, template <class...> class PRED>
    static constexpr auto tuple_none_of_v = tuple_none_of<FIRST, LAST, PRED>::value;

    template <class, class, template <class...> class, class...>
    struct tuple_find_if;

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct tuple_find_if<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, UNARY_PREDICATE, std::true_type> {
        using type = tuple_iterator<LAST...>;
    };

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct tuple_find_if<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, UNARY_PREDICATE, std::false_type>
        : std::conditional<UNARY_PREDICATE<typename tuple_iterator<FIRST...>::value_type>::value,
                           tuple_iterator<FIRST...>,
                           typename tuple_find_if<tuple_next<tuple_iterator<FIRST...>>, tuple_iterator<LAST...>,
                                                  UNARY_PREDICATE>::type> {};

    template <class... FIRST, class... LAST, template <class...> class UNARY_PREDICATE>
    struct tuple_find_if<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, UNARY_PREDICATE>
        : tuple_find_if<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, UNARY_PREDICATE,
                        typename std::is_same<tuple_iterator<FIRST...>, tuple_iterator<LAST...>>::type> {};

    template <class FIRST, class LAST, template <class...> class PRED>
    using tuple_find_if_t = typename tuple_find_if<FIRST, LAST, PRED>::type;

    template <class FIRST, class LAST, class VAL>
    using tuple_find = tuple_find_if<FIRST, LAST, type_bind<std::is_same, VAL>::template type>;

    template <class FIRST, class LAST, class VAL>
    using tuple_find_t = typename tuple_find<FIRST, LAST, VAL>::type;

    template <class FIRST, class LAST, template <class...> class OP, class RESULTS = void,
              bool = std::is_same_v<FIRST, LAST>>
    struct tuple_transform;

    template <class FIRST, class LAST, template <class...> class OP, class... RESULTS>
    struct tuple_transform<FIRST, LAST, OP, std::tuple<RESULTS...>, true> {
        using type = std::tuple<RESULTS...>;
    };

    template <class FIRST, class LAST, template <class...> class OP, class... RESULTS>
    struct tuple_transform<FIRST, LAST, OP, std::tuple<RESULTS...>, false> {
        using type =
            typename tuple_transform<tuple_next<FIRST>, LAST, OP,
                                     std::tuple<RESULTS..., typename OP<typename FIRST::value_type>::type>>::type;
    };

    template <class FIRST, class LAST, template <class...> class OP>
    struct tuple_transform<FIRST, LAST, OP, void, true> {
        using type = std::tuple<>;
    };

    template <class FIRST, class LAST, template <class...> class OP>
    struct tuple_transform<FIRST, LAST, OP, void, false> {
        using type = typename tuple_transform<tuple_next<FIRST>, LAST, OP,
                                              std::tuple<typename OP<typename FIRST::value_type>::type>>::type;
    };

    template <class FIRST, class LAST, template <class...> class OP>
    using tuple_transform_t = typename ext::tuple_transform<FIRST, LAST, OP>::type;

    template <class FIRST, class LAST, class RESULTS = void, bool = std::is_same_v<FIRST, LAST>>
    struct tuple_copy;

    template <class... FIRST, class... LAST, class... RESULTS>
    struct tuple_copy<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, std::tuple<RESULTS...>, true> {
        using type = std::tuple<RESULTS...>;
    };

    template <class... FIRST, class... LAST, class... RESULTS>
    struct tuple_copy<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, std::tuple<RESULTS...>, false> {
        using type = typename tuple_copy<tuple_next<tuple_iterator<FIRST...>>, tuple_iterator<LAST...>,
                                         std::tuple<RESULTS..., typename tuple_iterator<FIRST...>::value_type>>::type;
    };

    template <class FIRST, class LAST, template <class...> class PRED, class RESULTS = void,
              bool = std::is_same_v<FIRST, LAST>>
    struct tuple_copy_if;

    template <class FIRST, class LAST, template <class...> class PRED>
    struct tuple_copy_if<FIRST, LAST, PRED, void, true> {
        using type = std::tuple<>;
    };

    template <class FIRST, class LAST, template <class...> class PRED, class... RESULTS>
    struct tuple_copy_if<FIRST, LAST, PRED, std::tuple<RESULTS...>, true> {
        using type = std::tuple<RESULTS...>;
    };

    template <class FIRST, class LAST, template <class...> class PRED, class... RESULTS>
    struct tuple_copy_if<FIRST, LAST, PRED, std::tuple<RESULTS...>, false> {
        using type =
            typename tuple_copy_if<tuple_next<FIRST>, LAST, PRED,
                                   typename std::conditional_t<PRED<typename FIRST::value_type>::value,
                                                               std::tuple<RESULTS..., typename FIRST::value_type>,
                                                               std::tuple<RESULTS...>>>::type;
    };

    template <class FIRST, class LAST, template <class...> class PRED>
    struct tuple_copy_if<FIRST, LAST, PRED, void, false> : tuple_copy_if<FIRST, LAST, PRED, std::tuple<>> {};

    template <class FIRST, class LAST, class RESULTS = void, bool = std::is_same_v<FIRST, LAST>>
    struct tuple_reverse;

    template <class... FIRST, class... LAST, class... RESULTS>
    struct tuple_reverse<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, std::tuple<RESULTS...>, true> {
        using type = std::tuple<RESULTS...>;
    };

    template <class... FIRST, class... LAST, class... RESULTS>
    struct tuple_reverse<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, std::tuple<RESULTS...>, false> {
        using type = typename tuple_reverse<
            tuple_iterator<FIRST...>, tuple_prev<tuple_iterator<LAST...>>,
            std::tuple<RESULTS..., typename tuple_prev<tuple_iterator<LAST...>>::value_type>>::type;
    };

    template <class... FIRST, class... LAST>
    struct tuple_reverse<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, void, false>
        : tuple_reverse<tuple_iterator<FIRST...>, tuple_iterator<LAST...>, std::tuple<>> {};

    template <class FIRST1, class LAST1, class FIRST2, class LAST2, class = void, bool = std::is_same_v<FIRST1, LAST1>>
    struct tuple_set_intersection;

    template <class... FIRST1, class... LAST1, class... FIRST2, class... LAST2>
    struct tuple_set_intersection<tuple_iterator<FIRST1...>, tuple_iterator<LAST1...>, tuple_iterator<FIRST2...>,
                                  tuple_iterator<LAST2...>, void, false> {
        //        using type = std::conditional_t<std::

        template <class ELEMENT>
        struct intersects : tuple_any_of<tuple_iterator<FIRST2...>, tuple_iterator<LAST2...>,
                                         type_bind<std::is_same, ELEMENT>::template type> {};

        using type = typename tuple_copy_if<tuple_iterator<FIRST1...>, tuple_iterator<LAST1...>, intersects>::type;
    };

    template<class FIRST, class LAST,

} // namespace ext

#endif // TUPLE_EXT_ALGORITHM_H
