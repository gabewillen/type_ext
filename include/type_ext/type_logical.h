//
// Created by gwillen on 4/29/19.
//

#pragma once
#ifndef TUPLE_TYPE_LOGICAL_H
#define TUPLE_TYPE_LOGICAL_H

namespace ext {

    template <template <class...> class PREDICATE>
    struct type_not {
        template <class... ARGS>
        using type = std::integral_constant<bool, !PREDICATE<ARGS...>::value>;
    };

    template <template <class...> class PREDICATE>
    using type_not_t = typename type_not<PREDICATE>::type;


    template<class ...>
        struct type_or;

    template<class FIRST, class ...REST>
    struct type_or<FIRST, REST...> {
        using type = FIRST;
        static constexpr auto value = 1;
    };

    template<>
    struct type_or<> {
        using type = void;
        static constexpr auto value = 0;
    };

    template<class ...NEXT>
    struct type_or<void, NEXT...>  {
        using next = type_or<NEXT...>;
        using type = typename next::type;
        static constexpr auto value = next::value ? 1 + next::value : next::value;
    };

    template<class LEFT, class RIGHT>
    using type_or_t = typename type_or<LEFT, RIGHT>::type;
//
//
//    template <template <class...> class PREDICATE, class TRUE, class FALSE>
//    struct type_or {
//        template <class... ARGS>
//        using type = std::conditional_t<PREDICATE<ARGS...>::value, TRUE, FALSE>;
//    };
//
//    template<template <class ...> class PREDICATE, class TRUE, class FALSE>
//        using type_or_t = typename type_of<PREDICATE, TRUE, FALSE>::type;
} // namespace ext

#endif // TUPLE_TYPE_LOGICAL_H
