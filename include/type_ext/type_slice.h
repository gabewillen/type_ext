//
// Created by gwillen on 5/15/19.
//

#pragma once
#ifndef TUPLE_EXT_SLICE_H
#define TUPLE_EXT_SLICE_H

#include "type_algorithm.h"
#include "type_ext_traits.h"
#include "type_iterator.h"

namespace ext {

    namespace detail {

        template <class...>
        struct type_slicer;

        template <template <class...> class CONTAINER, class... REST>
        struct type_slicer<CONTAINER<>, REST...> {
            using type = CONTAINER<>;
        };

        template <template <class...> class CONTAINER, class ELEMENT, class... ELEMENTS, auto BEGIN,
                  auto END, auto CURRENT>
        struct type_slicer<CONTAINER<ELEMENT, ELEMENTS...>, ext::type_index<BEGIN>, ext::type_index<END>,
                           ext::type_index<CURRENT>> {

            using type = typename ext::type_flatten_t<
                typename std::conditional_t<(CURRENT > (BEGIN - 1)) && (CURRENT < END), CONTAINER<ELEMENT>,
                                            CONTAINER<>>,
                typename type_slicer<CONTAINER<ELEMENTS...>, ext::type_index<BEGIN>, ext::type_index<END>,
                                     ext::type_index<CURRENT + 1>>::type>;
        };

        template <class... REST>
        struct type_slicer<non_types<>, REST...> {
            using type = non_types<>;
        };

        template <auto ELEMENT, auto... ELEMENTS, auto BEGIN, auto END, auto CURRENT>
        struct type_slicer<non_types<ELEMENT, ELEMENTS...>, ext::type_index<BEGIN>, ext::type_index<END>,
                           ext::type_index<CURRENT>> {

            using type = typename ext::type_flatten_t<
                typename std::conditional_t<(CURRENT > (BEGIN - 1)) && (CURRENT < END), ext::non_types<ELEMENT>,
                                            ext::non_types<>>,
                typename type_slicer<non_types<ELEMENTS...>, ext::type_index<BEGIN>, ext::type_index<END>,
                                     ext::type_index<CURRENT + 1>>::type>;
        };

    } // namespace detail

    template <class TUPLE, long BEGIN, long END = ext::type_size_v<TUPLE>>
    using type_slice =
        typename detail::type_slicer<TUPLE, ext::type_index<BEGIN>, ext::type_index<END>, ext::type_index<0>>::type;

} // namespace ext

#endif // TUPLE_EXT_SLICE_H
