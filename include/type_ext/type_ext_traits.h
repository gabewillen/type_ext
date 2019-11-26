//
// Created by gwillen on 5/21/19.
//

#pragma once
#ifndef TYPE_EXT_TYPE_EXT_TRAITS_H
#define TYPE_EXT_TYPE_EXT_TRAITS_H

#include <type_traits>

namespace ext {

    template<bool VALUE, class TRUE=std::true_type, class FALSE=std::false_type>
    using conditional = std::conditional<VALUE, TRUE, FALSE>;

    template<bool VALUE, class TRUE=std::true_type, class FALSE=std::false_type>
    using conditional_t = typename ext::conditional<VALUE, TRUE, FALSE>::type;

}

#endif // TYPE_EXT_TYPE_EXT_TRAITS_H
