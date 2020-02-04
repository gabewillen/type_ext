//
// Created by gwillen on 4/29/19.
//

#pragma once
#ifndef TUPLE_EXT_FUNCTIONAL_H
#define TUPLE_EXT_FUNCTIONAL_H

#include "type_logical.h"
#include <functional>

namespace ext {

    template <auto NUMBER>
    using placeholder = std::_Placeholder<NUMBER>;

    template <template <class...> class TEMPLATE, class...>
    struct type_apply;

    template <template <class...> class META, template <class...> class CONTAINER, class... ARGUMENTS>
    struct type_apply<META, CONTAINER<ARGUMENTS...>> {
        using type = META<ARGUMENTS...>;
    };

    namespace detail {

        template <class ARGUMENT, class UNBOUND_ITER>
        struct deduce_argument {
            using type = ARGUMENT;
        };

        template <auto NUMBER, class UNBOUND_ITER>
        struct deduce_argument<placeholder<NUMBER>, UNBOUND_ITER> {
            using type = typename ext::type_advance<UNBOUND_ITER, NUMBER - 1>::value_type;
        };

        template <class BOUND_BEGIN, class BOUND_END, class UNBOUND_BEGIN, class UNBOUND_END,
                  class ARGUMENTS = typename BOUND_BEGIN::template container_type<>,
                  bool = std::is_same_v<BOUND_BEGIN, BOUND_END>>
        struct deduce_arguments {
            using type = ARGUMENTS;
        };

        template <class BOUND_BEGIN, class BOUND_END, class UNBOUND_BEGIN, class UNBOUND_END,
                  template <class...> class CONTAINER, class... ARGUMENTS>
        struct deduce_arguments<BOUND_BEGIN, BOUND_END, UNBOUND_BEGIN, UNBOUND_END, CONTAINER<ARGUMENTS...>, false> {
            using type = typename deduce_arguments<
                ext::type_next<BOUND_BEGIN>, BOUND_END, UNBOUND_BEGIN, UNBOUND_END,
                CONTAINER<ARGUMENTS...,
                          typename deduce_argument<typename BOUND_BEGIN::value_type, UNBOUND_BEGIN>::type>>::type;
        };
    } // namespace detail

    template <template <class...> class META, class... BOUND_ARGS>
    class type_bind {
        using bound_args = types<BOUND_ARGS...>;

    public:
        template <class... UNBOUND_ARGS>
        using type = typename type_apply<
            META, typename detail::deduce_arguments<ext::type_begin<bound_args>, ext::type_end<bound_args>,
                                                    ext::type_begin<types<UNBOUND_ARGS...>>,
                                                    ext::type_end<types<UNBOUND_ARGS...>>>::type>::type;
    };

    template <template <class...> class META, class... BOUND_ARGS>
    using type_bind_t = typename type_bind<META, BOUND_ARGS...>::type;

    //    template<template <class ...> class META, class ...BOUND_ARGS>
    //        class type_bind<

} // namespace ext

#endif // TUPLE_EXT_FUNCTIONAL_H
