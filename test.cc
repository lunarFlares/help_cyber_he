#include <iostream>
#include "ceval.hh"

using u8 = unsigned char;


u8 forceCon[68];


void powerCon(u8 whichKey, u8 force) {
    if (whichKey) {
        forceCon[whichKey - 1] = force;
    } else {
        for (u8 i = 0; i < 68; i++) {
            forceCon[i] = force;
        }
    }
}

#define powerCon(nums_list, force)                                  \
if (1) {                                                            \
constexpr const char* num_list_str = STR(nums_list);                \
constexpr unsigned split_count = count_split(num_list_str, '|');    \
constexpr split_str_to_u<split_count> sp(num_list_str, '|');        \
for (unsigned i = 0; i < sp.length; i++) {                          \
    powerCon(sp.nums[i], force);                                    \
}                                                                   \
}

void littleFingerForce() {
    powerCon(1 | 2 | 6 | 7 | 11 | 52 | 57 | 58 | 65, 10);
}



int main() {
    constexpr const char* _tmp = STR(1 | 2 | 6 | 7 | 11 | 52 | 57 | 58 | 65);
    static_assert(stou("114514") == 114514, "should be 114514");
    static_assert(count_split(_tmp, '|') == 9, "should be 9");
    constexpr unsigned split_count = count_split(_tmp, '|');

    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[0] == 1, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[1] == 2, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[2] == 6, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[3] == 7, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[4] == 11, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[5] == 52, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[6] == 57, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[7] == 58, "error");
    static_assert(split_str_to_u<split_count>(_tmp, '|').nums[8] == 65, "error");

    littleFingerForce();
    for (auto i = 0u; i < 68; i++) {
        std::cout << "force:" << i << " = " << unsigned(forceCon[i]) << std::endl;
    }
}