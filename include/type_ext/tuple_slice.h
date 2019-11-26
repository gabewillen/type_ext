//
// Created by gwillen on 5/15/19.
//

#pragma once
#ifndef TUPLE_EXT_SLICE_H
#define TUPLE_EXT_SLICE_H

#include "tuple_algorithm.h"
#include "tuple_iterator.h"
#include "type_ext_traits.h"

namespace ext {

    namespace detail {

        template <class ...>
        struct tuple_slicer;

        template<class ...REST>
            struct tuple_slicer<std::tuple<>, REST...> {
                using type = std::tuple<>;
            };


        template <class ELEMENT, class... ELEMENTS, std::size_t BEGIN, std::size_t END, std::size_t CURRENT>
        struct tuple_slicer<std::tuple<ELEMENT, ELEMENTS...>, ext::tuple_index<BEGIN>, ext::tuple_index<END>, ext::tuple_index<CURRENT>>  {


            using type = typename ext::tuple_flatten_t<typename std::conditional_t<(CURRENT > (BEGIN -1)) && (CURRENT < END), std::tuple<ELEMENT>, std::tuple<>>, typename tuple_slicer<std::tuple<ELEMENTS...>, ext::tuple_index<BEGIN>, ext::tuple_index<END>, ext::tuple_index<CURRENT + 1>>::type>;
        };

        template<class ...REST>
            struct tuple_slicer<nt_tuple<>, REST...> {
                using type = nt_tuple<>;
            };

        template<auto ELEMENT, auto ...ELEMENTS, std::size_t BEGIN, std::size_t END, std::size_t CURRENT>
        struct tuple_slicer<nt_tuple<ELEMENT, ELEMENTS...>, ext::tuple_index<BEGIN>, ext::tuple_index<END>, ext::tuple_index<CURRENT>> {

            using type = typename ext::tuple_flatten_t<typename std::conditional_t<(CURRENT > (BEGIN - 1) ) && (CURRENT < END), ext::nt_tuple<ELEMENT>, ext::nt_tuple<>>, typename tuple_slicer<nt_tuple<ELEMENTS...>, ext::tuple_index<BEGIN>, ext::tuple_index<END>, ext::tuple_index<CURRENT + 1>>::type>;
        };


    }

    template<class TUPLE, long BEGIN, long END=ext::tuple_size_v<TUPLE>>
    using tuple_slice_t = typename detail::tuple_slicer<TUPLE, ext::tuple_index<BEGIN>, ext::tuple_index<END>, ext::tuple_index<0>>::type;



}

#endif //TUPLE_EXT_SLICE_H
