//
// Created by gwillen on 5/21/19.
//

#pragma once
#ifndef TYPE_EXT_TYPE_EXT_TRAITS_H
#define TYPE_EXT_TYPE_EXT_TRAITS_H

#include "types.h"
#include <type_traits>

namespace ext {

    template <class...>
        struct type_common_type;

    template <template  <class ...> class TYPE, class ITEM, class... ITEMS>
        struct type_common_type<TYPE<ITEM, ITEMS...>> : std::common_type<ITEM, ITEMS...> {};

    template <class TYPE>
        using type_common_type_t = typename type_common_type<TYPE>::type;

    template <long VALUE>
        using type_index = std::integral_constant<long, VALUE>;


    template <class, std::size_t>
        struct type_element;

    template <template <class...> class TYPE, class ELEMENT, class... ELEMENTS, std::size_t INDEX>
        struct type_element<TYPE<ELEMENT, ELEMENTS...>, INDEX> : std::tuple_element<INDEX, std::tuple<ELEMENT, ELEMENTS...>> {
            using index = type_index<INDEX>;
        };

    template<class TYPE, std::size_t INDEX>
        using type_element_t = typename type_element<TYPE, INDEX>::type;



    template<bool VALUE, class TRUE=std::true_type, class FALSE=std::false_type>
    using conditional = std::conditional<VALUE, TRUE, FALSE>;

    template<bool VALUE, class TRUE=std::true_type, class FALSE=std::false_type>
    using conditional_t = typename ext::conditional<VALUE, TRUE, FALSE>::type;

//    template<class TYPE1, class TYPE2>
//    struct not_same : conditional<!std::is_same_v<TYPE1, TYPE2> {
//        static constexpr auto value = !std::is_same_v<TYPE1, TYPE2>;
//    };

}

#endif // TYPE_EXT_TYPE_EXT_TRAITS_H
