//
// Created by gwillen on 4/27/19.
//

#pragma once
#ifndef TUPLE_EXT_TRAITS_H
#define TUPLE_EXT_TRAITS_H

#include <type_traits>
#include "tuple.h"

namespace ext {

    template <class...>
    struct tuple_common_type;

    template <template  <class ...> class TUPLE, class ITEM, class... ITEMS>
    struct tuple_common_type<TUPLE<ITEM, ITEMS...>> : std::common_type<ITEM, ITEMS...> {};

    template <class TUPLE>
    using tuple_common_type_t = typename tuple_common_type<TUPLE>::type;

    template <std::size_t VALUE>
    using tuple_index = std::integral_constant<std::size_t, VALUE>;


    template <class, std::size_t>
    struct tuple_element;

    template <template <class...> class TUPLE, class ELEMENT, class... ELEMENTS, std::size_t INDEX>
    struct tuple_element<TUPLE<ELEMENT, ELEMENTS...>, INDEX> : std::tuple_element<INDEX, std::tuple<ELEMENT, ELEMENTS...>> {
        using index = tuple_index<INDEX>;
    };

    template<class TUPLE, std::size_t INDEX>
    using tuple_element_t = typename tuple_element<TUPLE, INDEX>::type;


} // namespace ext

#endif // TUPLE_EXT_TRAITS_H
