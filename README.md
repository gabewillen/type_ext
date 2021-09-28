# type_ext
This is meant to be a template library that implements algorithms for compile time from the standard library. 
**WARNING** ***devlopment of this library was absorbed closed source at my current employer. Use this version at your own risk.***

# Example

```C++
//
// Created by gwillen on 5/19/19.
//

#include <iostream>
#include <type_ext/non_types.h>
#include <type_ext/type_slice.h>
#include <type_ext/type_tree.h>

using tuple1 = std::tuple<int, short, float, long, float, bool, int, short, float, long, float, bool>;
using tuple2 = std::tuple<void *, int *, short, long *, bool>;
using tuple3 = std::tuple<void *, int *, short *, long *, bool *>;
using begin1 = ext::type_begin<tuple1>;
using end1 = ext::type_end<tuple1>;

using begin2 = ext::type_begin<tuple2>;
using end2 = ext::type_end<tuple2>;

using begin3 = ext::type_begin<tuple3>;
using end3 = ext::type_end<tuple3>;

static_assert(std::is_same_v<typename ext::type_element_t<tuple1, 0>, int>, "type_element failed");
static_assert(std::is_same_v<typename begin1::value_type, int>, "type_begin failed");
static_assert(std::is_same_v<typename ext::type_next<begin1>::value_type, short>, "type_next failed");
static_assert(std::is_same_v<typename ext::type_advance<begin1, 2>::value_type, float>, "type_advance 2 failed");
static_assert(std::is_same_v<typename ext::type_prev<ext::type_end<tuple1>>::value_type, bool>, "type_next failed");
static_assert(std::is_same_v<typename ext::type_advance<ext::type_end<tuple1>, -2>::value_type, float>, "type_advance -2 failed");
static_assert(ext::type_count_v<begin1, end1, float> == 4, "type_count failed");
static_assert(std::is_same_v<typename ext::type_find_t<begin1, end1, float>, ext::type_advance<begin1, 2>>, "type_find failed");
static_assert(ext::type_any_of_v<begin1, end1, std::is_integral>, "type_any_of failed");
static_assert(!ext::type_all_of_v<begin1, end1, std::is_integral>, "type_all_of failed");
static_assert(ext::type_none_of_v<begin1, end1, std::is_void>, "type_none_of failed");
static_assert(std::is_same_v<typename ext::type_transform<begin1, end1, std::add_pointer>::type, std::tuple<int *, short *, float *, long *, float *, bool *, int *, short *, float *, long *, float *, bool *>>, "type_transform failed");
static_assert(std::is_same_v<typename ext::type_copy_if<begin1, end1, std::is_integral>::type, std::tuple<int, short, long, bool, int, short, long, bool>>, "type_copy_if failed");
static_assert(std::is_same_v<typename ext::type_reverse<begin1, end1>::type, std::tuple<bool, float, long, float, short, int, bool, float, long, float, short, int>>, "type_reverse failed");
static_assert(std::is_same_v<typename ext::type_set_intersection< begin1, end1, begin2, end2>::type, std::tuple<short, bool>>, "type_set_intersection failed");
static_assert(std::is_same_v<typename ext::type_set_intersection< begin1, end1, begin3, end3>::type, std::tuple<>>, "type_set_intersection failed");
static_assert(std::is_same_v<typename ext::type_set_difference<begin1, end1, begin2, end2>::type, std::tuple<int, float, long>>, "type_set_difference failed");
```
