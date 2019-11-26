//
// Created by gwillen on 8/13/19.
//

#pragma once
#ifndef TYPE_EXT_TYPE_TREE_H
#define TYPE_EXT_TYPE_TREE_H

#include "tuple_algorithm.h"

namespace ext {

    template <class...>
    struct type_node {
        using children = std::tuple<>;
    };

    namespace detail {

        template <class, class = void>
        struct deduce_children {
            using type = std::tuple<>;
        };

        template <class TYPE, class ...PARENT>
        struct deduce_children<type_node<TYPE, PARENT...>, std::void_t<typename TYPE::children>> {

            template <class CHILD>
            struct to_tree {
                using type = ext::type_node<CHILD, type_node<TYPE, PARENT...>>;
            };

            using type = typename tuple_transform<tuple_begin<typename TYPE::children>,
                                                  tuple_end<typename TYPE::children>, to_tree>::type;
        };

    } // namespace detail

    template <class TYPE>
    struct type_node<TYPE> {
        using type = TYPE;
        using parent = void;
        using root = type_node;
        using children = typename detail::deduce_children<type_node>::type;
    };


    template <class TYPE, class... PARENT>
    struct type_node<TYPE, type_node<PARENT...>> : type_node<TYPE> {
        using parent = type_node<PARENT...>;
        using root = typename parent::root;
        using children = typename detail::deduce_children<type_node>::type;
    };


    template <class... ARGS>
    using type_tree = type_node<ARGS...>;

    template<class TREE>
    struct type_tree_preorder {
        using type = tuple_flatten_t<TREE, typename type_tree_preorder<typename TREE::children>::type>;
    };


    template<>
    struct type_tree_preorder<std::tuple<>> {
        using type = std::tuple<>;
    };

    template<class NODE, class ...CHILDREN>
        struct type_tree_preorder<std::tuple<NODE, CHILDREN...>> {
            using type = tuple_flatten_t<typename type_tree_preorder<NODE>::type, typename type_tree_preorder<std::tuple<CHILDREN...>>::type>;
        };



    template<class ...>
    struct type_tree_postorder;

    template<>
    struct type_tree_postorder<std::tuple<>> {
        using type = std::tuple<>;
    };

    template<class NODE, class ...CHILDREN>
        struct type_tree_postorder<std::tuple<NODE, CHILDREN...>> : tuple_flatten<typename type_tree_postorder<NODE>::type, typename type_tree_postorder<std::tuple<CHILDREN...>>::type> {};

    template<class TREE>
    struct type_tree_postorder<TREE> : tuple_flatten<typename type_tree_postorder<typename TREE::children>::type, TREE> {};

} // namespace ext

#endif // TYPE_EXT_TYPE_TREE_H
