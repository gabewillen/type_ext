//
// Created by gwillen on 4/29/19.
//

#pragma once
#ifndef TUPLE_EXT_FUNCTIONAL_H
#define TUPLE_EXT_FUNCTIONAL_H

#include "type_logical.h"
#include <functional>

namespace ext {

    template <template <class...> class TEMPLATE, class...>
    struct type_bind;

    template <template <class...> class META>
    struct type_bind<META> {

        template <class... ARGS>
        using type = META<ARGS...>;
    };

    namespace detail {

        template<class ...>
        struct apply;

        template<class BOUND_ITER, class UNBOUND_ITER>

        template<class BOUND_ITER, class UNBOUND_ITER>
        struct apply<BOUND_ITER, UNBOUND_ITER> {

        };
    }

    template <template <class...> class META, int NUMBER, class... ARGS>
    struct type_bind<META, std::_Placeholder<NUMBER>, ARGS...> {
        template <class... _ARGS>
        using type = typename type_bind<META, ARGS...>::template type<
            typename std::tuple_element_t<NUMBER, std::tuple<_ARGS...>>, _ARGS...>;
    };

    template<template <class ...> class META, class ...BOUND, class ...UNBOUND>
        struct type_bind<META, tuple_iterator<BOUND...>, tuple_iterator<UNBOUND...>> {

        };

    template <template <class...> class META, class... BOUND_ARGS>
    struct type_bind<META, BOUND_ARGS...> {//: type_bind<META, tuple_begin<std::tuple<ARGS...>>, tuple_end<std::tuple<ARGS...>>> {
        template <class... UNBOUND_ARGS>
        using type = META<typename detail::deduce_arguments<std::tuple_begin<std::tuple<BOUND_ARGS...>>, ext::tuple_end<std::tuple<UNBOUND_ARGS...>>
    };

    template <template <class...> class META, class... ARGS>
    using type_bind_t = typename type_bind<META, ARGS...>::type;

} // namespace ext

#endif // TUPLE_EXT_FUNCTIONAL_H
