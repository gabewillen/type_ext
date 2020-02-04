//
// Created by gwillen on 5/14/19.
//

#pragma once
#ifndef TYPES_EXT_ITERATOR_H
#define TYPES_EXT_ITERATOR_H

#include "type_ext_traits.h"

namespace ext {

    namespace detail {

        template <class...>
        struct iterator_begin;

        template <class...>
        struct iterator_end;

        template <template <class...> class TYPE, class... TYPES>
        struct iterator_begin<TYPE<TYPES...>> {
            using type = type_iterator<TYPE<TYPES...>>;
        };

        template <class TYPES>
        using iterator_begin_t = typename iterator_begin<TYPES>::type;

        template <template <class...> class CONTAINER, class... PREV>
        struct iterator_end<CONTAINER<>, CONTAINER<PREV...>> {
            using type = type_iterator<CONTAINER<>, CONTAINER<PREV...>>;
        };

        template <template <class...> class CONTAINER>
        struct iterator_end<CONTAINER<>> : iterator_end<CONTAINER<>, CONTAINER<>> {};

        template <template <class...> class CONTAINER, class TYPE, class... NEXT, class... PREV>
        struct iterator_end<CONTAINER<TYPE, NEXT...>, CONTAINER<PREV...>>
            : iterator_end<CONTAINER<NEXT...>, CONTAINER<TYPE, PREV...>> {};

        template <template <class...> class CONTAINER, class TYPE, class... TYPES>
        struct iterator_end<CONTAINER<TYPE, TYPES...>> : iterator_end<CONTAINER<TYPES...>, CONTAINER<TYPE>> {};

        template <class TYPES>
        using iterator_end_t = typename iterator_end<TYPES>::type;

        template <class, auto>
        struct iterator_increment;

        template <template <class...> class CONTAINER, class TYPE, class... TYPES, class... PREV_TYPES>
        struct iterator_increment<type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<PREV_TYPES...>>, 0> {
            using type = type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<PREV_TYPES...>>;
        };

        template <template <class...> class CONTAINER, class... PREV_TYPES>
        struct iterator_increment<type_iterator<CONTAINER<>, CONTAINER<PREV_TYPES...>>, 0> {
            using type = type_iterator<CONTAINER<>, CONTAINER<PREV_TYPES...>>;
        };

        template <template <class...> class CONTAINER, class TYPE, class... TYPES, class... PREV_TYPES, auto OFFSET>
        struct iterator_increment<type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<PREV_TYPES...>>, OFFSET>
            : iterator_increment<type_iterator<CONTAINER<TYPES...>, CONTAINER<TYPE, PREV_TYPES...>>, OFFSET - 1> {};

        template <class, auto>
        struct iterator_decrement;

        template <template <class...> class CONTAINER, class TYPE, class... TYPES, class PREV_TYPE, class... PREV_TYPES>
        struct iterator_decrement<type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<PREV_TYPE, PREV_TYPES...>>, 0> {
            using type = type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<PREV_TYPE, PREV_TYPES...>>;
        };

        template <template <class...> class CONTAINER, class TYPE, class... TYPES>
        struct iterator_decrement<type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<>>, 0> {
            using type = type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<>>;
        };

        template <template <class...> class CONTAINER, class PREV_TYPE, class... PREV_TYPES, auto OFFSET>
        struct iterator_decrement<type_iterator<CONTAINER<>, CONTAINER<PREV_TYPE, PREV_TYPES...>>, OFFSET>
            : iterator_decrement<type_iterator<CONTAINER<PREV_TYPE>, CONTAINER<PREV_TYPES...>>, OFFSET + 1> {};

        template <template <class...> class CONTAINER, class TYPE, class... TYPES, class PREV_TYPE, class... PREV_TYPES,
                  auto OFFSET>
        struct iterator_decrement<type_iterator<CONTAINER<TYPE, TYPES...>, CONTAINER<PREV_TYPE, PREV_TYPES...>>, OFFSET>
            : iterator_decrement<type_iterator<CONTAINER<PREV_TYPE, TYPE, TYPES...>, CONTAINER<PREV_TYPES...>>,
                                 OFFSET + 1> {};

        template <class, auto, bool>
        struct iterator_advance;

        template <class... ELEMENTS, auto OFFSET>
        struct iterator_advance<type_iterator<ELEMENTS...>, OFFSET, true>
            : iterator_increment<type_iterator<ELEMENTS...>, OFFSET> {};

        template <class... ELEMENTS, auto OFFSET>
        struct iterator_advance<type_iterator<ELEMENTS...>, OFFSET, false>
            : iterator_decrement<type_iterator<ELEMENTS...>, OFFSET> {};

        template <class ITER, auto OFFSET>
        using iterator_advance_t = typename iterator_advance<ITER, OFFSET, (OFFSET > 0)>::type;

    } // namespace detail

    template <template <class...> class CONTAINER, class... PREV_TYPES>
    struct type_iterator<CONTAINER<>, CONTAINER<PREV_TYPES...>> {
        using value_type = void;

        template <class... TYPES>
        using container_type = CONTAINER<TYPES...>;
    };

    template <template <class...> class CONTAINER, class TYPE, class... PREV_TYPES>
    struct type_iterator<CONTAINER<TYPE>, CONTAINER<PREV_TYPES...>> {
        using value_type = TYPE;
        template <class... TYPES>
        using container_type = CONTAINER<TYPES...>;
    };

    template <template <class...> class CONTAINER, class TYPE, class... NEXT_TYPES, class... PREV_TYPES>
    struct type_iterator<CONTAINER<TYPE, NEXT_TYPES...>, CONTAINER<PREV_TYPES...>>
        : type_iterator<CONTAINER<TYPE>, CONTAINER<PREV_TYPES...>> {};

    template <class TYPES>
    using type_begin = typename detail::iterator_begin_t<TYPES>;

    template <class TYPES>
    using type_end = typename detail::iterator_end_t<TYPES>;

    template <class ITER, auto OFFSET>
    using type_advance = typename detail::iterator_advance_t<ITER, OFFSET>;

    template <class ITER, auto OFFSET = 1>
    using type_next = type_advance<ITER, OFFSET>;

    template <class ITER, auto OFFSET = -1>
    using type_prev = type_advance<ITER, (OFFSET > 0 ? -OFFSET : OFFSET)>;

    template <class...>
    struct type_distance;

    template <template <class...> class CONTAINER, class... ITER1_NEXT, class... ITER1_PREV, class... ITER2_NEXT,
              class... ITER2_PREV>
    struct type_distance<type_iterator<CONTAINER<ITER1_NEXT...>, CONTAINER<ITER1_PREV...>>,
                         type_iterator<CONTAINER<ITER2_NEXT...>, CONTAINER<ITER2_PREV...>>>
        : std::integral_constant<std::intmax_t,
                                 static_cast<std::intmax_t>(sizeof...(ITER1_NEXT) - sizeof...(ITER2_NEXT))> {};

    template <class BEGIN, class END>
    static constexpr auto type_distance_v = type_distance<BEGIN, END>::value;

} // namespace ext

#endif // TYPES_EXT_ITERATOR_H
