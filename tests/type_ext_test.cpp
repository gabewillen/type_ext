//
// Created by gwillen on 5/19/19.
//

#include <iostream>
#include <type_ext/nt_tuple.h>
#include <type_ext/tuple_slice.h>
#include <type_ext/type_tree.h>

using tuple1 = std::tuple<int, short, float, long, float, bool>;
using tuple2 = std::tuple<void *, int *, short, long *, bool>;
using tuple3 = std::tuple<void *, int *, short *, long *, bool *>;
using begin1 = ext::tuple_begin<tuple1>;
using end1 = ext::tuple_end<tuple1>;

using begin2 = ext::tuple_begin<tuple2>;
using end2 = ext::tuple_end<tuple2>;

using begin3 = ext::tuple_begin<tuple3>;
using end3 = ext::tuple_end<tuple3>;

static_assert(std::is_same_v<typename ext::tuple_element_t<tuple1, 0>, int>, "tuple_element failed");
//static_assert(ext::tuple_distance_v<tuple1, long> == 2, "tuple_distance failed");


static_assert(std::is_same_v<typename begin1::value_type, int>, "tuple_begin failed");
static_assert(std::is_same_v<typename ext::tuple_next<begin1>::value_type, short>, "tuple_next failed");
static_assert(std::is_same_v<typename ext::tuple_advance<begin1, 2>::value_type, float>, "tuple_advance 2 failed");
static_assert(std::is_same_v<typename ext::tuple_prev<ext::tuple_end<tuple1>>::value_type, bool>, "tuple_next failed");
static_assert(std::is_same_v<typename ext::tuple_advance<ext::tuple_end<tuple1>, -2>::value_type, float>, "tuple_advance -2 failed");
static_assert(ext::tuple_count_v<begin1, end1, float> == 2, "tuple_count failed");
static_assert(std::is_same_v<typename ext::tuple_find_t<begin1, end1, float>, ext::tuple_advance<begin1, 2>>, "tuple_find failed");
static_assert(ext::tuple_any_of_v<begin1, end1, std::is_integral>, "tuple_any_of failed");
static_assert(!ext::tuple_all_of_v<begin1, end1, std::is_integral>, "tuple_all_of failed");
static_assert(ext::tuple_none_of_v<begin1, end1, std::is_void>, "tuple_none_of failed");
static_assert(std::is_same_v<typename ext::tuple_transform<begin1, end1, std::add_pointer>::type, std::tuple<int *, short *, float *, long *, float *, bool *>>, "tuple_transform failed");
static_assert(std::is_same_v<typename ext::tuple_copy_if<begin1, end1, std::is_integral>::type, std::tuple<int, short, long, bool>>, "tuple_copy_if failed");
static_assert(std::is_same_v<typename ext::tuple_reverse<begin1, end1>::type, std::tuple<bool, float, long, float, short, int>>, "tuple_reverse failed");
static_assert(std::is_same_v<typename ext::tuple_set_intersection< begin1, end1, begin2, end2>::type, std::tuple<short, bool>>, "tuple_set_intersection failed");
static_assert(std::is_same_v<typename ext::tuple_set_intersection< begin1, end1, begin3, end3>::type, std::tuple<>>, "tuple_set_intersection failed");
//static_assert(ext::tuple_distance<ext::tuple_next<it>, ext::tuple_end<tuple1>>)

template<class ...ARGS>
void print() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

struct A {

    struct B {
        struct E {};
        using children = std::tuple<E>;
    };
    struct C {};
    struct D {
        struct F {};
        struct G {};
        struct H {};
        struct J {};

        using children = std::tuple<F, G, H, J>;
    };

    using children = std::tuple<B, C, D>;

};

using tree = ext::type_tree<A>;

int a;
float b;
short c;
long d;

using tuple4 = ext::nt_tuple<&a, &b, &c, &d>;

int main() {
    using preorder = typename ext::type_tree_preorder<tree>::type;
    print<preorder>();
    print<typename ext::type_tree_postorder<tree>::type>();
    print<ext::type_bind<std::tuple, std::_Placeholder<0>, int>::template type<float>>();
    print<ext::tuple_slice_t<tuple1, 2>>();
}