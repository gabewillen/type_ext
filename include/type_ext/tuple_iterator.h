//
// Created by gwillen on 5/14/19.
//

#pragma once
#ifndef TUPLE_EXT_ITERATOR_H
#define TUPLE_EXT_ITERATOR_H

#include "tuple_type_traits.h"

namespace ext {

    namespace detail {

        template <class...>
        struct iterator_begin;

        template <class...>
        struct iterator_end;

        template < class... ELEMENTS>
        struct iterator_begin<std::tuple<ELEMENTS...>> {
            using type = tuple_iterator<std::tuple<ELEMENTS...>>;
        };

        template <class TUPLE>
        using iterator_begin_t = typename iterator_begin<TUPLE>::type;

        template < class ...PREV>
            struct iterator_end<std::tuple<>, std::tuple<PREV...>> {
                using type = tuple_iterator<std::tuple<>, std::tuple<PREV...>>;
            };

        template<>
        struct iterator_end<std::tuple<>> : iterator_end<std::tuple<>, std::tuple<>> {};


        template< class ELEMENT, class ...NEXT, class ...PREV>
            struct iterator_end<std::tuple<ELEMENT, NEXT...>, std::tuple<PREV...>> : iterator_end<std::tuple<NEXT...>, std::tuple<ELEMENT, PREV...>> {
            };

        template < class ELEMENT, class... ELEMENTS>
        struct iterator_end<std::tuple<ELEMENT, ELEMENTS...>> : iterator_end<std::tuple<ELEMENTS...>, std::tuple<ELEMENT>> {};

//        template <template <class...> class TUPLE>
//        struct iterator_end<std::tuple<>> {
//            using type = tuple_iterator<std::tuple<>, std::tuple<>>;
//        };

        template <class TUPLE>
        using iterator_end_t = typename iterator_end<TUPLE>::type;

        template <class, auto>
        struct iterator_increment;

        template < class ELEMENT, class... ELEMENTS, class... PREV_ELEMENTS>
        struct iterator_increment<tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENTS...>>, 0> {
            using type = tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENTS...>>;
        };

        template < class... PREV_ELEMENTS>
            struct iterator_increment<tuple_iterator<std::tuple<>, std::tuple<PREV_ELEMENTS...>>, 0> {
                using type = tuple_iterator<std::tuple<>, std::tuple<PREV_ELEMENTS...>>;
            };

        template < class ELEMENT, class... ELEMENTS, class... PREV_ELEMENTS,
                  auto OFFSET>
        struct iterator_increment<tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENTS...>>, OFFSET>
            : iterator_increment<tuple_iterator<std::tuple<ELEMENTS...>,
                                                std::tuple<ELEMENT, PREV_ELEMENTS...>>,
                                 OFFSET - 1> {};

        template <class, auto>
        struct iterator_decrement;

        template < class ELEMENT, class... ELEMENTS, class PREV_ELEMENT,
                  class... PREV_ELEMENTS>
        struct iterator_decrement<tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENT, PREV_ELEMENTS...>>,
                                  0> {
            using type = tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENT, PREV_ELEMENTS...>>;
        };

        template < class ELEMENT, class... ELEMENTS>
            struct iterator_decrement<tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<>>,
                                      0> {
                using type = tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<>>;
            };

        template < class PREV_ELEMENT, class... PREV_ELEMENTS, auto OFFSET>
        struct iterator_decrement<tuple_iterator<std::tuple<>, std::tuple<PREV_ELEMENT, PREV_ELEMENTS...>>, OFFSET>
            : iterator_decrement<tuple_iterator<std::tuple<PREV_ELEMENT>, std::tuple<PREV_ELEMENTS...>>, OFFSET + 1> {};

        template < class ELEMENT, class... ELEMENTS, class PREV_ELEMENT,
                  class... PREV_ELEMENTS, auto OFFSET>
        struct iterator_decrement<tuple_iterator<std::tuple<ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENT, PREV_ELEMENTS...>>,
                                  OFFSET>
            : iterator_decrement<tuple_iterator<std::tuple<PREV_ELEMENT, ELEMENT, ELEMENTS...>, std::tuple<PREV_ELEMENTS...>>,
                                 OFFSET + 1> {};

        template <class, auto, bool>
        struct iterator_advance;

        template <class ...ELEMENTS, auto OFFSET>
        struct iterator_advance<tuple_iterator<ELEMENTS...>, OFFSET, true>
            : iterator_increment<tuple_iterator<ELEMENTS...>, OFFSET> {};

        template < class... ELEMENTS, auto OFFSET>
        struct iterator_advance<tuple_iterator<ELEMENTS...>, OFFSET, false>
            : iterator_decrement<tuple_iterator<ELEMENTS...>, OFFSET> {};

        template <class ITER, auto OFFSET>
        using iterator_advance_t = typename iterator_advance<ITER, OFFSET, (OFFSET > 0)>::type;

    } // namespace detail


    template < class... PREV_ELEMENTS>
    struct tuple_iterator<std::tuple<>, std::tuple<PREV_ELEMENTS...>> {
        using value_type = void;

    };

    template < class ELEMENT, class... PREV_ELEMENTS>
    struct tuple_iterator<std::tuple<ELEMENT>, std::tuple<PREV_ELEMENTS...>> {
        using value_type = ELEMENT;

    };

    template < class ELEMENT, class... NEXT_ELEMENTS, class... PREV_ELEMENTS>
    struct tuple_iterator<std::tuple<ELEMENT, NEXT_ELEMENTS...>, std::tuple<PREV_ELEMENTS...>>
        : tuple_iterator<std::tuple<ELEMENT>, std::tuple<PREV_ELEMENTS...>> {};


    template <class TUPLE>
    using tuple_begin = typename detail::iterator_begin_t<TUPLE>;

    template <class TUPLE>
    using tuple_end = typename detail::iterator_end_t<TUPLE>;

    template <class ITER, auto OFFSET>
    using tuple_advance = typename detail::iterator_advance_t<ITER, OFFSET>;

    template <class ITER, auto OFFSET = 1>
    using tuple_next = tuple_advance<ITER, OFFSET>;

    template <class ITER, auto OFFSET = -1>
    using tuple_prev = tuple_advance<ITER, (OFFSET > 0 ? -OFFSET : OFFSET)>;

    template <class...>
    struct tuple_distance;
    //
    template <class... ITER1_NEXT, class ...ITER1_PREV, class... ITER2_NEXT, class ...ITER2_PREV>
    struct tuple_distance<tuple_iterator<std::tuple<ITER1_NEXT...>, std::tuple<ITER1_PREV...>>, tuple_iterator<std::tuple<ITER2_NEXT...>, std::tuple<ITER2_PREV...>>> : std::integral_constant<std::intmax_t, static_cast<std::intmax_t>(sizeof...(ITER1_NEXT) - sizeof...(ITER2_NEXT))>{};

    template<class BEGIN, class END>
    static constexpr auto tuple_distance_v = tuple_distance<BEGIN, END>::value;
    //
    //    template <class BEGIN, class END>
    //    inline constexpr std::size_t tuple_distance_v = tuple_distance<BEGIN, END>::value;

} // namespace ext

#endif // TUPLE_EXT_ITERATOR_H
