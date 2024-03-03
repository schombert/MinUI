#include "minui_interfaces.hpp"
#include "minui_text_impl.hpp"
#include "unordered_dense.h"

#include <stdint.h>
#include <ctype.h>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <variant>
#include <optional>
#include <chrono>
#include <array>
#include <icu.h>
#include <assert.h>

#pragma comment(lib, "icu.lib")

namespace minui {
namespace text {

/*
	n 	the absolute value of N.*
	i 	the integer digits of N.*
	v 	the number of visible fraction digits in N, with trailing zeros.*
	w 	the number of visible fraction digits in N, without trailing zeros.*
	f 	the visible fraction digits in N, with trailing zeros, expressed as an integer.*
	t 	the visible fraction digits in N, without trailing zeros, expressed as an integer.*
	c 	compact decimal exponent value: exponent of the power of 10 used in compact decimal formatting.
	e 	a deprecated synonym for ‘c’. Note: it may be redefined in the future.
	*/

int64_t remove_zeros(int64_t f) {
	while(f % 10 == 0) {
		f = f / 10;
	}
	return f;
}

attribute_type card_2am(int64_t i, int64_t f, int32_t) {
	if(i == 0 || (std::abs(i) == 1 && f == 0))
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2ff(int64_t i, int64_t, int32_t) {
	if(i == 0 || i == 1)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2ast(int64_t i, int64_t, int32_t v) {
	if(i == 1 && v == 0)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2si(int64_t i, int64_t f, int32_t) {
	if((std::abs(i) == 1 || i == 0) && f == 0)
		return attribute_type::one;
	else if(i == 0 && f == 1)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2ak(int64_t i, int64_t f, int32_t) {
	if((std::abs(i) == 1 || i == 0) && f == 0)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2tzm(int64_t i, int64_t f, int32_t) {
	if((std::abs(i) == 1 || i == 0 || (std::abs(i) >= 11 && std::abs(i) <= 99)) && f == 0)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2af(int64_t i, int64_t f, int32_t) {
	if((std::abs(i) == 1) && f == 0)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2da(int64_t i, int64_t f, int32_t) {
	if((std::abs(i) == 1) && f == 0)
		return attribute_type::one;
	else if((i == 0 || i == 1) && f != 0)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2is(int64_t i, int64_t f, int32_t) {
	if((i % 10 == 1) && (i % 100 != 11) && f == 0)
		return attribute_type::one;
	else if(remove_zeros(f) % 10 == 1 && remove_zeros(f) % 100 != 11)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2mk(int64_t i, int64_t f, int32_t v) {
	if(v == 0 && i % 10 == 1 && i % 100 != 11)
		return attribute_type::one;
	else if(f % 10 == 1 && f % 100 != 11)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_2ceb(int64_t i, int64_t f, int32_t v) {
	if(v == 0 && (i == 1 || i == 2 || i == 3))
		return attribute_type::one;
	else if(v == 0 && i % 10 != 4 && i % 10 != 6 && i % 10 != 9)
		return attribute_type::one;
	else if(v != 0 && f % 10 != 4 && f % 10 != 6 && f % 10 != 9)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_3lv(int64_t i, int64_t f, int32_t v) {
	if(f == 0 && (i % 10 == 0 || (i % 100 >= 11 && i % 100 <= 19)))
		return attribute_type::zero;
	else if(v == 2 && f % 100 >= 11 && f % 100 <= 19)
		return attribute_type::zero;
	else if(f == 0 && std::abs(i) % 10 == 1 && std::abs(i) % 100 != 11)
		return attribute_type::one;
	else if(v == 2 && f % 10 == 1 && f % 100 != 11)
		return attribute_type::one;
	else if(v != 2 && f % 10 == 1)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_3lag(int64_t i, int64_t f, int32_t) {
	if(f == 0 && i == 0)
		return attribute_type::zero;
	else if(i == 0 || i == 1)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_3ksh(int64_t i, int64_t f, int32_t) {
	if(f == 0 && i == 0)
		return attribute_type::zero;
	else if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else
		return attribute_type::other;
}

attribute_type card_3he(int64_t i, int64_t, int32_t v) {
	if((i == 1 && v == 0) || (i == 0 && v != 0))
		return attribute_type::one;
	else if(i == 2 && v == 0)
		return attribute_type::two;
	else
		return attribute_type::other;
}

attribute_type card_3iu(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) == 2)
		return attribute_type::two;
	else
		return attribute_type::other;
}

attribute_type card_3shi(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(i == 0)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) >= 2 && std::abs(i) <= 10)
		return attribute_type::few;
	else
		return attribute_type::other;
}

attribute_type card_3mo(int64_t i, int64_t f, int32_t v) {
	if(v == 0 && i == 1)
		return attribute_type::one;
	else if(v != 0 || (f == 0 && i == 0) || (f == 0 && abs(i) != 1 && abs(i) % 100 >= 1 && abs(i) % 100 <= 19))
		return attribute_type::few;
	else
		return attribute_type::other;
}

attribute_type card_3bs(int64_t i, int64_t f, int32_t v) {
	if(v == 0 and i % 10 == 1 && i % 100 != 11)
		return attribute_type::one;
	else if(f % 10 == 1 && f % 100 != 11)
		return attribute_type::one;
	else if(v == 0 && i % 10 >= 2 && i % 10 <= 4 && i % 100 != 12 && i % 100 != 13 && i % 100 != 14)
		return attribute_type::few;
	else if(f % 10 >= 2 && f % 10 <= 4 && f % 100 != 12 && f % 100 != 13 && f % 100 != 14)
		return attribute_type::few;
	else
		return attribute_type::other;
}

attribute_type card_3fr(int64_t i, int64_t, int32_t v) {
	if(i == 0 || i == 1)
		return attribute_type::one;
	else if(i != 0 && i % 1000000 == 0 && v == 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_3pt(int64_t i, int64_t, int32_t v) {
	if(i == 0 || i == 1)
		return attribute_type::one;
	else if(i != 0 && i % 1000000 == 0 && v == 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_3ca(int64_t i, int64_t, int32_t v) {
	if(i == 1 && v == 0)
		return attribute_type::one;
	else if(i != 0 && i % 1000000 == 0 and v == 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_3es(int64_t i, int64_t f, int32_t v) {
	if(std::abs(i) == 1 && f == 0)
		return attribute_type::one;
	else if(i != 0 && i % 1000000 == 0 && v == 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_4gd(int64_t i, int64_t f, int32_t) {
	if((std::abs(i) == 1 || std::abs(i) == 11) && f == 0)
		return attribute_type::one;
	else if((std::abs(i) == 2 || std::abs(i) == 12) && f == 0)
		return attribute_type::two;
	else if(std::abs(i) >= 3 && std::abs(i) <= 19 && f == 0)
		return attribute_type::few;
	else
		return attribute_type::many;
}

attribute_type card_4sl(int64_t i, int64_t, int32_t v) {
	if(v == 0 && i % 100 == 1)
		return attribute_type::one;
	else if(v == 0 && i % 100 == 2)
		return attribute_type::two;
	else if(v == 0 && (i % 100 == 3 || i % 100 == 4))
		return attribute_type::few;
	else if(v != 0)
		return attribute_type::few;
	else
		return attribute_type::other;
}

attribute_type card_4dsb(int64_t i, int64_t f, int32_t v) {
	if(v == 0 && i % 100 == 1)
		return attribute_type::one;
	else if(f % 100 == 1)
		return attribute_type::one;
	else if(v == 0 && i % 100 == 2)
		return attribute_type::two;
	else if(f % 100 == 2)
		return attribute_type::two;
	else if(v == 0 && (i % 100 == 3 || i % 100 == 4))
		return attribute_type::few;
	else if((f % 100 == 3 || f % 100 == 4))
		return attribute_type::few;
	else
		return attribute_type::other;
}

attribute_type card_4cs(int64_t i, int64_t, int32_t v) {
	if(i == 1 && v == 0)
		return attribute_type::one;
	else if(v == 0 && (i == 2 || i == 3 || i == 4))
		return attribute_type::few;
	else if(v != 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_4pl(int64_t i, int64_t, int32_t v) {
	if(i == 1 && v == 0)
		return attribute_type::one;
	else if(v == 0 && (i % 10 == 2 || i % 10 == 3 || i % 10 == 4) && i % 100 != 12 && i % 100 != 13 && i % 100 != 14)
		return attribute_type::few;
	else if(v == 0 && i != 1 && i % 10 != 2 && i % 10 != 3 && i % 10 != 4)
		return attribute_type::many;
	else if(v == 0 && (i % 100 == 12 || i % 100 == 13 || i % 100 == 14))
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_4be(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) % 10 == 1 && std::abs(i) % 100 != 11)
		return attribute_type::one;
	else if(f == 0 && (std::abs(i) % 10 == 2 || std::abs(i) % 10 == 3 || std::abs(i) % 10 == 4) && std::abs(i) % 100 != 12 && std::abs(i) % 100 != 13 && std::abs(i) % 100 != 14)
		return attribute_type::few;
	else if(f == 0 && std::abs(i) % 10 == 0)
		return attribute_type::many;
	else if(f == 0 && std::abs(i) % 10 >= 5 && std::abs(i) % 10 <= 9)
		return attribute_type::many;
	else if(f == 0 && std::abs(i) % 100 >= 11 && std::abs(i) % 100 <= 14)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_4lt(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) % 10 == 1 && (std::abs(i) % 100 < 11 || std::abs(i) % 100 > 19))
		return attribute_type::one;
	else if(f == 0 && std::abs(i) % 10 >= 2 && std::abs(i) % 10 <= 9 && (std::abs(i) % 100 < 11 || std::abs(i) % 100 > 19))
		return attribute_type::few;
	else if(f != 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_4ru(int64_t i, int64_t, int32_t v) {
	if(v == 0 && i % 10 == 1 && i % 100 != 11)
		return attribute_type::one;
	else if(v == 0 && (std::abs(i) % 10 == 2 || std::abs(i) % 10 == 3 || std::abs(i) % 10 == 4) && std::abs(i) % 100 != 12 && std::abs(i) % 100 != 13 && std::abs(i) % 100 != 14)
		return attribute_type::few;
	else if(v == 0 && i % 10 == 0)
		return attribute_type::many;
	else if(v == 0 && i % 10 >= 5 && i % 10 <= 9)
		return attribute_type::many;
	else if(v == 0 && (i % 100 == 11 || i % 100 == 12 || i % 100 == 13 || i % 100 == 14))
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_5br(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) % 10 == 1 && std::abs(i) % 100 != 11 && std::abs(i) % 100 != 71 && std::abs(i) % 100 != 91)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) % 10 == 2 && std::abs(i) % 100 != 12 && std::abs(i) % 100 != 72 && std::abs(i) % 100 != 92)
		return attribute_type::two;
	else if(f == 0 && (std::abs(i) % 10 == 3 || std::abs(i) % 10 == 4 || std::abs(i) % 10 == 9) && (std::abs(i) % 100 < 10 || (std::abs(i) % 100 > 19 && std::abs(i) % 100 < 70) || (std::abs(i) % 100 > 79 && std::abs(i) % 100 < 90)))
		return attribute_type::few;
	else if(f == 0 && i != 0 && i % 1000000 == 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_5mt(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) == 2)
		return attribute_type::two;
	else if(f == 0 && i == 0)
		return attribute_type::few;
	else if(f == 0 && std::abs(i) % 100 >= 3 && std::abs(i) % 100 <= 10)
		return attribute_type::few;
	else if(f == 0 && std::abs(i) % 100 >= 11 && std::abs(i) % 100 <= 19)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_5ga(int64_t i, int64_t f, int32_t) {
	if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) == 2)
		return attribute_type::two;
	else if(f == 0 && std::abs(i) >= 3 && std::abs(i) <= 6)
		return attribute_type::few;
	else if(f == 0 && std::abs(i) >= 7 && std::abs(i) <= 10)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_5gv(int64_t i, int64_t, int32_t v) {
	if(v == 0 && i % 10 == 1)
		return attribute_type::one;
	else if(v == 0 && i % 10 == 2)
		return attribute_type::two;
	else if(v == 0 && (i % 100 == 0 || i % 100 == 20 || i % 100 == 40 || i % 100 == 60 || i % 100 == 80))
		return attribute_type::few;
	else if(v != 0)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_6kw(int64_t i, int64_t f, int32_t) {
	if(f == 0 && i == 0)
		return attribute_type::zero;
	else if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(f == 0 && (std::abs(i) % 100 == 2 || std::abs(i) % 100 == 22 || std::abs(i) % 100 == 42 || std::abs(i) % 100 == 62 || std::abs(i) % 100 == 82))
		return attribute_type::two;
	else if(f == 0 && std::abs(i) % 1000 == 0
		&& ((std::abs(i) % 100000 >= 1000 && std::abs(i) % 100000 <= 20000) || std::abs(i) % 100000 == 40000 || std::abs(i) % 100000 == 60000 || std::abs(i) % 100000 == 80000))
		return attribute_type::two;
	else if(f == 0 && std::abs(i) % 1000000 == 100000)
		return attribute_type::two;
	else if(f == 0 && (std::abs(i) % 100 == 3 || std::abs(i) % 100 == 23 || std::abs(i) % 100 == 43 || std::abs(i) % 100 == 63 || std::abs(i) % 100 == 83))
		return attribute_type::few;
	else if(f == 0 && std::abs(i) != 1 && (std::abs(i) % 100 == 1 || std::abs(i) % 100 == 21 || std::abs(i) % 100 == 41 || std::abs(i) % 100 == 61 || std::abs(i) % 100 == 81))
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_6ar(int64_t i, int64_t f, int32_t) {
	if(f == 0 && i == 0)
		return attribute_type::zero;
	else if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) == 2)
		return attribute_type::two;
	else if(f == 0 && std::abs(i) % 100 >= 3 && std::abs(i) % 100 <= 10)
		return attribute_type::few;
	else if(f == 0 && std::abs(i) % 100 >= 11 && std::abs(i) % 100 <= 99)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type card_6cy(int64_t i, int64_t f, int32_t) {
	if(f == 0 && i == 0)
		return attribute_type::zero;
	else if(f == 0 && std::abs(i) == 1)
		return attribute_type::one;
	else if(f == 0 && std::abs(i) == 2)
		return attribute_type::two;
	else if(f == 0 && std::abs(i) == 3)
		return attribute_type::few;
	else if(f == 0 && std::abs(i) == 6)
		return attribute_type::many;
	else
		return attribute_type::other;
}

attribute_type ord_2sv(int64_t i) {
	if((std::abs(i) % 10 == 1 || std::abs(i) % 10 == 2) && std::abs(i) % 100 != 11 && std::abs(i) % 100 != 12)
		return attribute_type::ord_one;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2bal(int64_t i) {
	if(std::abs(i) == 1)
		return attribute_type::ord_one;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2hu(int64_t i) {
	if(std::abs(i) == 1 || std::abs(i) == 5)
		return attribute_type::ord_one;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2ne(int64_t i) {
	if(std::abs(i) >= 1 && std::abs(i) <= 4)
		return attribute_type::ord_one;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2be(int64_t i) {
	if((std::abs(i) % 10 == 2 || std::abs(i) % 10 == 3) && std::abs(i) % 100 != 12 && std::abs(i) % 100 != 13)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2uk(int64_t i) {
	if(std::abs(i) % 10 == 3 && std::abs(i) % 100 != 13)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2tk(int64_t i) {
	if(std::abs(i) % 10 == 9 || std::abs(i) % 10 == 6 || std::abs(i) == 10)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2kk(int64_t i) {
	if(std::abs(i) % 10 == 9 || std::abs(i) % 10 == 6 || (std::abs(i) % 10 == 0 && i != 0))
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2it(int64_t i) {
	if(std::abs(i) == 11 || std::abs(i) == 8 || std::abs(i) == 80 || std::abs(i) == 800)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_2lij(int64_t i) {
	if(std::abs(i) == 11 || std::abs(i) == 8 || (std::abs(i) >= 80 && std::abs(i) <= 89) || (std::abs(i) >= 800 && std::abs(i) <= 899))
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_3ka(int64_t i) {
	if(i == 1)
		return attribute_type::ord_one;
	else if(i == 0 || i % 100 == 40 || i % 100 == 60 || i % 100 == 80 || (i % 100 >= 2 && i % 100 <= 20))
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_3sq(int64_t i) {
	if(std::abs(i) == 1)
		return attribute_type::ord_one;
	else if(std::abs(i) % 10 == 4 && std::abs(i) % 100 != 14)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_3kw(int64_t i) {
	if(std::abs(i) >= 1 && std::abs(i) <= 4)
		return attribute_type::ord_one;
	else if(std::abs(i) % 100 >= 1 && std::abs(i) % 100 <= 4)
		return attribute_type::ord_one;
	else if(std::abs(i) % 100 >= 21 && std::abs(i) % 100 <= 24)
		return attribute_type::ord_one;
	else if(std::abs(i) % 100 >= 41 && std::abs(i) % 100 <= 44)
		return attribute_type::ord_one;
	else if(std::abs(i) % 100 >= 61 && std::abs(i) % 100 <= 64)
		return attribute_type::ord_one;
	else if(std::abs(i) % 100 >= 81 && std::abs(i) % 100 <= 84)
		return attribute_type::ord_one;
	else if(std::abs(i) == 5 || std::abs(i) % 100 == 5)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_4en(int64_t i) {
	if(std::abs(i) % 10 == 1 && std::abs(i) % 100 != 11)
		return attribute_type::ord_one;
	else if(std::abs(i) % 10 == 2 && std::abs(i) % 100 != 12)
		return attribute_type::ord_two;
	else if(std::abs(i) % 10 == 3 && std::abs(i) % 100 != 13)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_4mr(int64_t i) {
	if(std::abs(i) == 1)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2 || std::abs(i) == 3)
		return attribute_type::ord_two;
	else if(std::abs(i) == 4)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_4gd(int64_t i) {
	if(std::abs(i) == 1 || std::abs(i) == 11)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2 || std::abs(i) == 12)
		return attribute_type::ord_two;
	else if(std::abs(i) == 3 || std::abs(i) == 13)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_4ca(int64_t i) {
	if(std::abs(i) == 1 || std::abs(i) == 3)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2)
		return attribute_type::ord_two;
	else if(std::abs(i) == 4)
		return attribute_type::ord_few;
	else
		return attribute_type::ord_other;
}

attribute_type ord_4mk(int64_t i) {
	if(i % 10 == 1 && i % 100 != 11)
		return attribute_type::ord_one;
	else if(i % 10 == 2 && i % 100 != 12)
		return attribute_type::ord_two;
	else if((i % 10 == 7 || i % 10 == 8) && i % 100 != 17 && i % 100 != 18)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_4az(int64_t i) {
	if(i % 10 == 1 || i % 10 == 2 || i % 10 == 5 || i % 10 == 7 || i % 10 == 8 || i % 100 == 20 || i % 100 == 50 || i % 100 == 70 || i % 100 == 80)
		return attribute_type::ord_one;
	else if(i % 10 == 3 || i % 10 == 4 || i % 1000 == 100 || i % 1000 == 200 || i % 1000 == 300 || i % 1000 == 400 || i % 1000 == 500 || i % 1000 == 600 || i % 1000 == 700 || i % 1000 == 800 || i % 1000 == 900)
		return attribute_type::ord_few;
	else if(i == 0 || i % 10 == 6 || i % 100 == 40 || i % 100 == 60 || i % 100 == 90)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_5gu(int64_t i) {
	if(std::abs(i) == 1)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2 || std::abs(i) == 3)
		return attribute_type::ord_two;
	else if(std::abs(i) == 4)
		return attribute_type::ord_few;
	else if(std::abs(i) == 6)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_5as(int64_t i) {
	if(std::abs(i) == 1 || std::abs(i) == 5 || std::abs(i) == 7 || std::abs(i) == 8 || std::abs(i) == 9 || std::abs(i) == 10)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2 || std::abs(i) == 3)
		return attribute_type::ord_two;
	else if(std::abs(i) == 4)
		return attribute_type::ord_few;
	else if(std::abs(i) == 6)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_5or(int64_t i) {
	if(std::abs(i) == 1 || std::abs(i) == 5 || std::abs(i) == 7 || std::abs(i) == 8 || std::abs(i) == 9)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2 || std::abs(i) == 3)
		return attribute_type::ord_two;
	else if(std::abs(i) == 4)
		return attribute_type::ord_few;
	else if(std::abs(i) == 6)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}

attribute_type ord_6cy(int64_t i) {
	if(i == 0 || std::abs(i) == 7 || std::abs(i) == 8 || std::abs(i) == 9)
		return attribute_type::ord_zero;
	else if(std::abs(i) == 1)
		return attribute_type::ord_one;
	else if(std::abs(i) == 2)
		return attribute_type::ord_two;
	else if(std::abs(i) == 4 || std::abs(i) == 3)
		return attribute_type::ord_few;
	else if(std::abs(i) == 6 || std::abs(i) == 5)
		return attribute_type::ord_many;
	else
		return attribute_type::ord_other;
}


bool codepoint_is_nonspacing(uint32_t c) noexcept {
	return u_charType(c) == U_NON_SPACING_MARK;
}

uint16_t single_numeric_16[] = {
	0x002B,//		PLUS
	0x002C,//		COMMA
	0x002D,//		MINUS
	0x002E,//		DECIMAL
	0x00B1,//		PLUS MINUS
	0x2052,//		COMMERCIAL MINUS
	0xFF0B,//		FULL PLUS
	0xFF0C,//		FULL COMMA
	0xFF0D,//		FULL MINUS
	0xFF0E,//		FULL DECIMAL
	0xFE50,//		SMALL COMMA
	0xFE62,//		SMALL PLUS
	0xFE52,//		SMALL DECIMAL
	0xFE51,//		SMALL COMMA
	0xFF64,//		HALF COMMA
	0xFF61,//		HALF DECIMAL
	0x2396,//		DECIMAL SEPARATOR
	0x066B,//		ARABIC DECIMAL SEPARATOR
	0x066C//		ARABIC THOUSANDS
};

bool codepoint16_is_numeric(uint16_t c16) noexcept {
	for(uint32_t i = 0; i < sizeof(single_numeric_16) / sizeof(uint16_t); ++i) {
		if(single_numeric_16[i] == c16) {
			return true;
		}
	}
	return u_getIntPropertyValue(c16, UCHAR_NUMERIC_TYPE) != U_NT_NONE;
}

bool codepoint32_is_numeric(uint32_t c) noexcept {
	return u_getIntPropertyValue(c, UCHAR_NUMERIC_TYPE) != U_NT_NONE;
}

uint32_t assemble_codepoint(uint16_t high, uint16_t low) noexcept {
	uint32_t high_bits = (high & 0x03FF) << 10;
	uint32_t low_bits = low & 0x03FF;
	uint32_t temp = high_bits | low_bits;
	return temp + 0x10000;
}

surrogate_pair make_surrogate_pair(uint32_t val) noexcept {
	uint32_t v = val - 0x10000;
	uint32_t h = ((v >> 10) & 0x03FF) | 0xD800;
	uint32_t l = (v & 0x03FF) | 0xDC00;
	return surrogate_pair{ uint16_t(h), uint16_t(l) };
}

bool is_low_surrogate(uint16_t char_code) noexcept {
	return char_code >= 0xDC00 && char_code <= 0xDFFF;
}
bool is_high_surrogate(uint16_t char_code) noexcept {
	return char_code >= 0xD800 && char_code <= 0xDBFF;
}

uint32_t codepoint_from_utf8(char const*& read_position, char const* end) {

	uint8_t byte1 = read_position < end ? uint8_t(*read_position) : uint8_t(0);
	uint8_t byte2 = read_position + 1 < end ? uint8_t(*(read_position + 1)) : uint8_t(0);
	uint8_t byte3 = read_position + 2 < end ? uint8_t(*(read_position + 2)) : uint8_t(0);
	uint8_t byte4 = read_position + 3 < end ? uint8_t(*(read_position + 3)) : uint8_t(0);

	if((byte1 & 0x80) == 0) {
		read_position += 1;
		return uint32_t(byte1);
	} else if((byte1 & 0xE0) == 0xC0) {
		read_position += 2;
		return uint32_t(byte2 & 0x3F) | (uint32_t(byte1 & 0x1F) << 6);
	} else  if((byte1 & 0xF0) == 0xE0) {
		read_position += 3;
		return uint32_t(byte3 & 0x3F) | (uint32_t(byte2 & 0x3F) << 6)| (uint32_t(byte1 & 0x0F) << 12);
	}  else if((byte1 & 0xF8) == 0xF0) {
		read_position += 4;
		return uint32_t(byte4 & 0x3F) | (uint32_t(byte3 & 0x3F) << 6) | (uint32_t(byte2 & 0x3F) << 12) | (uint32_t(byte1 & 0x07) << 18);
	} else {
		read_position += 1;
		return 0;
	}
}

bool requires_surrogate_pair(uint32_t codepoint) {
	return codepoint >= 0x10000;
}

bool cursor_ignorable16(uint16_t at_position, uint16_t trailing) {
	if(at_position >= 0xDC00 && at_position <= 0xDFFF) {
		return false; // low surrogate
	} else if(at_position == 0x200B || at_position == 0x200C || at_position == 0x200D || at_position == 0x2060 || at_position == 0xFEFF) {
		//200B = zero width space
		//200C = zero width non joiner
		//200D = zero width joiner
		//2060 = word joiner
		//FEFF = zero width, no break space
		return false;
	} else if(at_position < 0x20) {
		// legacy ascii control codes
		if(at_position == 0x0A) // '\n'
			return true;
		else if(at_position == 0x09) // '\t'
			return true;
		return false;
	} else if(at_position >= 0x7F && at_position <= 0x9F) {
		return false;
	} else if(at_position == 0x061C || at_position == 0x200E || at_position == 0x200F || (at_position >= 0x202A && at_position <= 0x202E) || (at_position >= 0x2066 && at_position <= 0x2069)) {
		return false;
	} else {
		if(at_position >= 0xD800 && at_position <= 0xDBFF) { // high surrogate
			return codepoint_is_nonspacing(assemble_codepoint(at_position, trailing));
		} else { // non surrogate
			return codepoint_is_nonspacing(at_position);
		}
	}
}

// TODO: replace with proper unicode decomposition
// https://www.unicode.org/reports/tr29/

int32_t num_logical_chars_in_range(std::wstring_view str) {
	int32_t total = 0;
	for(uint32_t i = 0; i < str.length(); ++i) {
		auto char_code = str[i];
		if(char_code >= 0xDC00 && char_code <= 0xDFFF) {
			// don't count a low surrogate
		} else if(char_code == 0x200B || char_code == 0x200C || char_code == 0x200D || char_code == 0x2060 || char_code == 0xFEFF) {
			//200B = zero width space
			//200C = zero width non joiner
			//200D = zero width joiner
			//2060 = word joiner
			//FEFF = zero width, no break space
		} else if(char_code < 0x20) {
			// legacy ascii control codes
			if(char_code == 0x0A) // '\n'
				++total;
			else if(char_code == 0x09) // '\t'
				++total;
		} else if(char_code >= 0x7F && char_code <= 0x9F) {
			// other control codes
		} else if(char_code == 0x061C || char_code == 0x200E || char_code == 0x200F || (char_code >= 0x202A && char_code <= 0x202E) || (char_code >= 0x2066 && char_code <= 0x2069)) {
			// bidi control codes
		} else {
			if(char_code >= 0xD800 && char_code <= 0xDBFF) { // high surrogate
				if(i + 1 != str.length()) {
					if(!codepoint_is_nonspacing(assemble_codepoint(str[i], str[i + 1])))
						++total;
				}
			} else { // non surrogate
				if(!codepoint_is_nonspacing(str[i]))
					++total;
			}
		}
	}
	return total;
}

/*
0009..000D    ; White_Space # Cc   [5] <control-0009>..<control-000D>
0020          ; White_Space # Zs       SPACE
0085          ; White_Space # Cc       <control-0085>
00A0          ; White_Space # Zs       NO-BREAK SPACE
1680          ; White_Space # Zs       OGHAM SPACE MARK
2000..200A    ; White_Space # Zs  [11] EN QUAD..HAIR SPACE
2028          ; White_Space # Zl       LINE SEPARATOR
2029          ; White_Space # Zp       PARAGRAPH SEPARATOR
202F          ; White_Space # Zs       NARROW NO-BREAK SPACE
205F          ; White_Space # Zs       MEDIUM MATHEMATICAL SPACE
3000          ; White_Space # Zs       IDEOGRAPHIC SPACE
*/

bool codepoint_is_space(uint32_t c) noexcept {
	return (c == 0x3000 || c == 0x205F || c == 0x202F || c == 0x2029 || c == 0x2028 || c == 0x00A0
		|| c == 0x0085 ||  c <= 0x0020 || (0x2000 <= c && c <= 0x200A));
}
bool codepoint_is_line_break(uint32_t c) noexcept {
	return  c == 0x2029 || c == 0x2028 || c == uint32_t('\n') || c == uint32_t('\r');
}


/*
key + one or more dotted atributes -- forms a set
key.att text text text \at{attc} 
or \.att
key.attb tex text text \at{attd}
-- if a no-dot version is supplied, that is the default
-- \at{} provides the given attribute for the matched value

key text text \p{name.attb} text \it{\match{param?x paramb?y}{opt a}{param?z}{opt b}{}{else} rest of} text
-- \p{name....} followed by a dotted list, tries to select the best match for the set passed in
-- \match{name?x.y ... } tests the base value passed in for matching attributes
------ uses \match{name.z?blah} for matching against a particular set member instead of the default

key entry
	-- key attributes
	-- content
		-- raw text (unparsed)
		-- provided attributes (from parse result)
		-- main content (utf16 string plus formatting) -- indices into global vectors + lengths
		-- 0 or more match set references pointing into after end content -- indices into global match set vectors and length
			-- each match can match on any number of parameters, doing look up on them with up to 8 attributes, can match on each with up to 8 attributes
	-- ll pointer to next key

	matches vector
	one entry per match set, refers to options vector; tells us how many entries the options have in total

	options vector
	variable number of entries per match (each contains parameter number, 8 lookup attributes, 8 matching attributes)
	last contains indices into global vectors + lengths for string and formatting
	union
		16xK bytes attribute matches (K to get closer to space needed for indices)
		N bytes of indices (uint32t offsets x 2 == 8 + uint16t lengths x 2 = 4)
	+ parameter num or #FF for end
*/


void prepare_key_content(backing_arrays& container, keyed_entry& entry);

keyed_entry& lookup_entry(backing_arrays& container, text::handle base_handle, std::array<text::attribute_type, text::max_attributes> const& attributes = { text::attribute_type::undefined }) {

	uint32_t best_match = base_handle.id;
	int32_t best_attributes_matched = -1;
	int32_t best_attributes_unmatched = 0;

	do {
		int32_t temp_best_attributes_matched = 0;
		int32_t temp_attributes_unmatched = 0;

		auto& entry = container.entries[base_handle.id];
		prepare_key_content(container, entry);

		for(uint32_t j = 0; j < text::max_attributes; ++j) {
			if(entry.lookup_attributes[j] == text::attribute_type::undefined)
				break;

			bool matched_lookup = false;

			for(uint32_t i = 0; i < text::max_attributes; ++i) {
				if(attributes[i] == text::attribute_type::undefined)
					break;
				if(entry.lookup_attributes[j] == attributes[i]) {
					matched_lookup = true;
					break;
				}
				
			}

			if(matched_lookup)
				++temp_best_attributes_matched;
			else
				++temp_attributes_unmatched;
		}

			

		if(temp_best_attributes_matched > best_attributes_matched || (temp_best_attributes_matched == best_attributes_matched && temp_attributes_unmatched < best_attributes_unmatched)) {

			best_match = base_handle.id;
			best_attributes_matched = temp_best_attributes_matched;
			best_attributes_unmatched = temp_attributes_unmatched;
		}

		base_handle.id = entry.next_entry;
	} while(base_handle.id != 0xFFFFFFFF);

	return container.entries[best_match];
}

char const* end_of_slash_identifier(char const* start, char const* terminal) {
	bool first_char = true;
	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if(codepoint_is_space(r) || r == uint32_t('{') || r == uint32_t('}') || (!first_char && r == uint32_t('\\'))) { // r ends sequence
			return start;
		} 

		// sequence continues
		start = next;
		first_char = false;
	}
	return terminal;
}

char const* end_of_whitespace(char const* start, char const* terminal) {
	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if(!codepoint_is_space(r)) { // r ends sequence
			return start;
		} else { // sequence continues
			start = next;
		}
	}
	return terminal;
}

char const* skip_opening_brace(char const* start, char const* terminal) {
	bool opening_brace_found = false;

	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if(!codepoint_is_space(r) && (opening_brace_found || r != uint32_t('{'))) { // r ends sequence
			return start;
		} else if(r == uint32_t('{')) {
			opening_brace_found = true;
		}

		start = next;
	}
	return terminal;
}

char const* skip_terminal_brace(char const* start, char const* terminal) {
	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if(r == uint32_t('}')) { // r ends sequence
			return next;
		} else { // sequence continues
			start = next;
		}
	}
	return terminal;
}

char const* remove_trailing_ws(char const* start, char const* terminal) {
	auto last_non_ws = start;

	while(start < terminal) {
		auto r = codepoint_from_utf8(start, terminal);
		if(!codepoint_is_space(r))
			last_non_ws = start;
	}

	return last_non_ws;
}

bool next_token_is_left_brace(char const* start, char const* terminal) {
	start = end_of_whitespace(start, terminal);
	auto next_cp = codepoint_from_utf8(start, terminal);
	return next_cp == uint32_t('{');
}

char const* end_of_braced_content(char const* start, char const* terminal) {
	int32_t open_brace_count = 0;
	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if(r == uint32_t('{')) {
			++open_brace_count;
		} else if(r == uint32_t('}')) {
			if(open_brace_count <= 0)
				return start;
			else
				--open_brace_count;
		} 
		start = next;
	}

	return terminal;
}

char const* read_until_separator(char const* start, char const* terminal) {
	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if(codepoint_is_space(r) || r == uint32_t('.') || r == uint32_t('?') || r == uint32_t('}') || r == uint32_t('{')) { // r ends sequence
			return start;
		} else { // sequence continues
			start = next;
		}
	}
	return terminal;
}

char const* read_until_newline(char const* start, char const* terminal) {
	while(start < terminal) {
		auto next = start;
		auto r = codepoint_from_utf8(next, terminal);

		if( r == uint32_t('\r') || r == uint32_t('\n')) { // r ends sequence
			return start;
		} else { // sequence continues
			start = next;
		}
	}
	return terminal;
}

struct pending_matches {
	std::string_view conditions;
	std::string_view body;
};

struct body_result {
	uint32_t string_data = 0;
	uint32_t formatting_data = 0;
};

text::attribute_type name_to_attribute(backing_arrays& container, std::string_view attribute_name) {
	if(attribute_name == "zero") {
		return text::attribute_type::zero;
	} else if(attribute_name == "one") {
		return text::attribute_type::one;
	} else if(attribute_name == "two") {
		return text::attribute_type::two;
	} else if(attribute_name == "few") {
		return text::attribute_type::few;
	} else if(attribute_name == "many") {
		return text::attribute_type::many;
	} else if(attribute_name == "other") {
		return text::attribute_type::other;
	} else if(attribute_name == "ord_zero") {
		return text::attribute_type::ord_zero;
	} else if(attribute_name == "ord_one") {
		return text::attribute_type::ord_one;
	} else if(attribute_name == "ord_two") {
		return text::attribute_type::ord_two;
	} else if(attribute_name == "ord_few") {
		return text::attribute_type::ord_few;
	} else if(attribute_name == "ord_many") {
		return text::attribute_type::ord_many;
	} else if(attribute_name == "ord_other") {
		return text::attribute_type::ord_other;
	} else if(attribute_name == "z") {
		return text::attribute_type::z;
	} else if(auto it = container.attribute_lookup.find(attribute_name); it != container.attribute_lookup.end()) {
		return it->second;
	} else {
		auto var_val = text::attribute_type(container.first_free_attribute++);
		container.attribute_lookup.insert_or_assign(attribute_name, var_val);
		return var_val;
	}
}

body_result process_body_data(backing_arrays& container, char const* start, char const* end, std::vector<pending_matches>* matches, keyed_entry* entry) {
	bool space_pending = false;

	uint32_t utf16_codepoint_index = 0;
	auto first_codepoint = container.text_data.size();

	uint32_t formatting_count = 0;
	auto first_formatting = container.fomatting_storage.size();
	std::vector<bool> last_format_is_color;

	start = end_of_whitespace(start, end);

	while(start < end) {
		auto c = codepoint_from_utf8(start, end);

		if(codepoint_is_space(c)) {
			space_pending = true;
		} else {
			if(space_pending) {
				++utf16_codepoint_index;
				container.text_data.push_back(L' ');
				space_pending = false;
			}
		}

		if(codepoint_is_space(c)) {
			// do nothing, space handled above
		} else if(c == uint32_t('}')) {
			if(last_format_is_color.size() > 0) {
				bool ending_color = last_format_is_color.back();
				last_format_is_color.pop_back();

				if(ending_color) {
					++formatting_count;
					container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::color_end{ } });
				} else {
					++formatting_count;
					container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::close_formatting });
				}
			}
		} else if(c == uint32_t('{')) {

			auto cident_start = end_of_whitespace(start, end);
			start = skip_terminal_brace(start, end);
			auto cident_end = remove_trailing_ws(cident_start, start - 1);

			auto end_name = read_until_separator(cident_start, cident_end);
			std::string_view param_name(cident_start, end_name);

			text::variable var_val;
			if(auto it = container.variable_lookup.find(param_name); it != container.variable_lookup.end()) {
				var_val = it->second;
			} else {
				var_val = text::variable{ container.first_free_variable++ };
				container.variable_lookup.insert_or_assign(param_name, var_val);
			}

			++formatting_count;
			container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), var_val });

			text::attribute_type low = text::attribute_type::undefined;
			auto remainder = end_name;
			codepoint_from_utf8(remainder, cident_end);

			while(remainder < cident_end) {
				auto end_attr = read_until_separator(remainder, cident_end);
				if(end_attr != remainder) {
					auto attr = name_to_attribute(container, std::string_view(remainder, end_attr));
					if(low == text::attribute_type::undefined) {
						low = attr;
					} else {
						++formatting_count;
						container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::variable_attributes{ low, attr} });
						low = text::attribute_type::undefined;
					}
				}
				remainder = end_attr;
				codepoint_from_utf8(remainder, cident_end); // discard separator
			}
			if(low != text::attribute_type::undefined) {
				++formatting_count;
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::variable_attributes{ low, text::attribute_type::undefined } });
			}
		} else if(c == uint32_t('\\')) {
			auto identifier_begin = start;
			start = end_of_slash_identifier(start, end);
			std::string_view identifier(identifier_begin, start);

			if(identifier == "t") {
				++utf16_codepoint_index;
				container.text_data.push_back(L'\t');

				start = end_of_whitespace(start, end);
			} else if(identifier == "n") {
				++utf16_codepoint_index;
				container.text_data.push_back(L'\n');

				start = end_of_whitespace(start, end);
			} else if(identifier == "\\") {
				++utf16_codepoint_index;
				container.text_data.push_back(L'\\');

				start = end_of_whitespace(start, end);
			} else if(identifier == "it") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::italic });
				start = skip_opening_brace(start, end);
			} else if(identifier == "b") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::bold });
				start = skip_opening_brace(start, end);
			} else if(identifier == "sc") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::small_caps });
				start = skip_opening_brace(start, end);
			} else if(identifier == "os") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::old_numbers });
				start = skip_opening_brace(start, end);
			} else if(identifier == "sup") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::subscript });
				start = skip_opening_brace(start, end);
			} else if(identifier == "sub") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::subscript });
				start = skip_opening_brace(start, end);
			} else if(identifier == "tab") {
				++formatting_count;
				last_format_is_color.push_back(false);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::tabular_numbers });
				start = skip_opening_brace(start, end);
			} else if(identifier == "c") {
				start = skip_opening_brace(start, end);
				auto cident_start = end_of_whitespace(start, end);
				start = skip_terminal_brace(start, end);
				auto cident_end = remove_trailing_ws(cident_start, start - 1);

				std::string_view char_name(cident_start, cident_end);

				if(char_name == "left-brace") {
					++utf16_codepoint_index;
					container.text_data.push_back(L'{');
				} else if(char_name == "right-brace") {
					++utf16_codepoint_index;
					container.text_data.push_back(L'}');
				} else if(std::isdigit(char_name[0])) {
					std::string temp(char_name);
					uint32_t val = std::strtoul(temp.c_str(), nullptr, 0);

					if(!requires_surrogate_pair(val)) {
						++utf16_codepoint_index;
						container.text_data.push_back(wchar_t(val));
					} else {
						utf16_codepoint_index += 2;
						auto p = make_surrogate_pair(val);
						container.text_data.push_back(wchar_t(p.high));
						container.text_data.push_back(wchar_t(p.low));
					}
				} else if(char_name == "em-space") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2003));
				} else if(char_name == "en-space") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2002));
				} else if(char_name == "3rd-em") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2004));
				} else if(char_name == "4th-em") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2005));
				} else if(char_name == "6th-em") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2006));
				} else if(char_name == "thin-space") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2009));
				} else if(char_name == "hair-space") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x200A));
				} else if(char_name == "figure-space") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2007));
				} else if(char_name == "ideo-space") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x3000));
				} else if(char_name == "hyphen") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2010));
				} else if(char_name == "figure-dash") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2012));
				} else if(char_name == "en-dash") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2013));
				} else if(char_name == "em-dash") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2014));
				} else if(char_name == "minus") {
					++utf16_codepoint_index;
					container.text_data.push_back(wchar_t(0x2212));
				} else {
					++utf16_codepoint_index;
					container.text_data.push_back(L'?');
				}
			} else if(identifier == "o") { // color
				start = skip_opening_brace(start, end);
				auto cident_start = end_of_whitespace(start, end);
				start = skip_terminal_brace(start, end);
				auto cident_end = remove_trailing_ws(cident_start, start - 1);

				std::string_view value(cident_start, cident_end);
				std::string temp(value);
				uint32_t val = std::strtoul(temp.c_str(), nullptr, 0);

				++formatting_count;
				last_format_is_color.push_back(true);
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::color{ uint16_t(val) } });

			} else if(identifier == "i") { // icon
				start = skip_opening_brace(start, end);
				auto cident_start = end_of_whitespace(start, end);
				start = skip_terminal_brace(start, end);
				auto cident_end = remove_trailing_ws(cident_start, start - 1);

				std::string_view value(cident_start, cident_end);
				std::string temp(value);
				uint32_t val = std::strtoul(temp.c_str(), nullptr, 0);

				++formatting_count;
				container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::inline_icon_id{ uint16_t(val) } });

				++utf16_codepoint_index;
				container.text_data.push_back(L'*');

			} else if(identifier == "a" && entry) {
				start = skip_opening_brace(start, end);
				auto cident_start = end_of_whitespace(start, end);
				start = skip_terminal_brace(start, end);
				auto cident_end = remove_trailing_ws(cident_start, start - 1);

				std::string_view attribute_name(cident_start, cident_end);
				text::attribute_type var_val = name_to_attribute(container, attribute_name);

				for(uint32_t j = 0; j < text::max_attributes; ++j) {
					if(entry->provided_attributes[j] == text::attribute_type::undefined) {
						entry->provided_attributes[j] = var_val;
						break;
					}
				}
			} else if(identifier == "match") {
				if(matches) {
					bool at_least_one_match_added = false;

					while(next_token_is_left_brace(start, end)) {
						auto start_condition = skip_opening_brace(start, end);
						auto end_condition_body = end_of_braced_content(start_condition, end);

						auto next_section_start = end_condition_body;
						codepoint_from_utf8(next_section_start, end); // skips closing brace

						if(next_token_is_left_brace(next_section_start, end)) {
							auto start_body = skip_opening_brace(next_section_start, end);
							auto end_body_body = end_of_braced_content(start_body, end);

							auto end_condition = remove_trailing_ws(start_condition, end_condition_body);
							auto end_body = remove_trailing_ws(start_body, end_body_body);



							++formatting_count;
							container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::match_set_reference{ uint16_t(-1 - int64_t(matches->size())) } });
							matches->push_back(pending_matches{ std::string_view(start_condition, end_condition), std::string_view(start_body, end_body) });
							at_least_one_match_added = true;

							start = end_body_body;
							codepoint_from_utf8(start, end); // skips closing brace
						} else { // no match body found
							start = next_section_start;
						}
					}

					if(at_least_one_match_added) {
						container.fomatting_storage.pop_back();
						container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::match_set_terminal_reference{ uint16_t(-1 - int64_t(matches->size() - 1)) } });
					}
				}
			}
		} else {
			if(requires_surrogate_pair(c)) {
				auto sp = make_surrogate_pair(c);
				container.text_data.push_back(wchar_t(sp.high));
				container.text_data.push_back(wchar_t(sp.low));
				utf16_codepoint_index += 2;
			} else {
				++utf16_codepoint_index;
				container.text_data.push_back(wchar_t(c));
			}
		}
	} // while start < end

	while(!last_format_is_color.empty()) { // close open formatting
		bool ending_color = last_format_is_color.back();
		last_format_is_color.pop_back();

		if(ending_color) {
			++formatting_count;
			container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::color_end{ } });
		} else {
			++formatting_count;
			container.fomatting_storage.push_back(format_marker{ uint16_t(utf16_codepoint_index), text::extra_formatting::close_formatting });
		}
	}

	return body_result{
		uint32_t(first_codepoint) << 16 | (utf16_codepoint_index & 0xFFFF),
		uint32_t(first_formatting) << 16 | (formatting_count & 0xFFFF)
	};
}

text::variable get_variable(backing_arrays& container, std::string_view name) {
	if(auto it = container.variable_lookup.find(name); it != container.variable_lookup.end()) {
		return it->second;
	} else {
		auto var_val = text::variable{ container.first_free_variable++ };
		container.variable_lookup.insert_or_assign(name, var_val);
		return var_val;
	}
}

void prepare_key_content(backing_arrays& container, keyed_entry& entry) {
	if(entry.string_data != 0)
		return;

	auto start = entry.raw_text;
	auto end = start + entry.raw_text_length;

	std::vector<pending_matches> matches;

	auto res = process_body_data(container, start, end, &matches, &entry);

	entry.string_data = res.string_data;
	entry.formatting_data = res.formatting_data;
	entry.match_set_entries = uint32_t(container.match_options.size());

	// process match sets
	uint16_t match_number = 0;
	for(auto& m : matches) {
		auto format_start = container.fomatting_storage.data() + (entry.formatting_data >> 16);
		auto format_end = container.fomatting_storage.data() + (entry.formatting_data >> 16) + (entry.formatting_data & 0xFFFF);
		while(format_start < format_end) {
			if(std::holds_alternative<text::match_set_reference>(format_start->format)) {
				if(std::get<text::match_set_reference>(format_start->format).id == uint16_t(-1 -match_number)) {
					std::get<text::match_set_reference>(format_start->format).id = uint16_t(container.match_options.size() - entry.match_set_entries);
					break;
				}
			} else if(std::holds_alternative<text::match_set_terminal_reference>(format_start->format)) {
				if(std::get<text::match_set_terminal_reference>(format_start->format).id == uint16_t(-1 - match_number)) {
					std::get<text::match_set_terminal_reference>(format_start->format).id = uint16_t(container.match_options.size() - entry.match_set_entries);
					break;
				}
			}
			++format_start;
		}
		++match_number;

		//
		// part A: add lookup conditions to options
		//

		auto keystart = m.conditions.data();
		auto keyend = m.conditions.data() + m.conditions.size();

		while(keystart < keyend) {
			auto var_end = read_until_separator(keystart, keyend);
			std::string_view variable(keystart, var_end);

			option_entry* current_opt = nullptr;
			auto separator = codepoint_from_utf8(var_end, keyend);
			keystart = var_end;
			bool adding_lookup_attributes = true;

			while(keystart < keyend) {
				if(codepoint_is_space(separator)) { // end of sequence, go to next variable
					break;
				} else {
					if(!current_opt) {
						container.match_options.emplace_back();
						current_opt = &(container.match_options.back());

						current_opt->variable = get_variable(container, variable);
						for(uint32_t i = 0; i < text::max_attributes; ++i) {
							current_opt->payload.attributes.lookup_attributes[i] = text::attribute_type::undefined;
							current_opt->payload.attributes.match_attributes[i] = text::attribute_type::undefined;
						}
					}

					if(separator == uint32_t('?')) {
						adding_lookup_attributes = false;
					}

					auto att_end = read_until_separator(keystart, keyend);
					std::string_view attribute_name(keystart, att_end);

					text::attribute_type var_val = name_to_attribute(container, attribute_name);

					if(adding_lookup_attributes) {
						for(uint32_t i = 0; i < text::max_attributes; ++i) {
							if(current_opt->payload.attributes.lookup_attributes[i] == text::attribute_type::undefined) {
								current_opt->payload.attributes.lookup_attributes[i] = var_val;
								break;
							}
						}
					} else {
						for(uint32_t i = 0; i < text::max_attributes; ++i) {
							if(current_opt->payload.attributes.match_attributes[i] == text::attribute_type::undefined) {
								current_opt->payload.attributes.match_attributes[i] = var_val;
								break;
							}
						}
					}

					separator = codepoint_from_utf8(att_end, keyend); // setup next loop
					keystart = att_end;
				}
			}

			keystart = end_of_whitespace(keystart, keyend); // advance to start of next variable
		}

		//
		// part B: make body content for option
		//

		auto bodystart = m.body.data();
		auto bodyend = m.body.data() + m.body.size();
		auto res = process_body_data(container, bodystart, bodyend, nullptr, nullptr);

		container.match_options.emplace_back();
		container.match_options.back().variable = text::variable{ uint16_t(0xFFFF) };
		container.match_options.back().payload.content.string_data = res.string_data;
		container.match_options.back().payload.content.formatting_data = res.formatting_data;
	}
}

text::handle lookup_entry(backing_arrays& container, std::string_view name) {
	if(auto it = container.entry_lookup.find(name); it != container.entry_lookup.end()) {
		return it->second;
	} else {
		return text::handle{ };
	}
}

text::formatted_text perform_substitutions(backing_arrays& container, std::array<text::attribute_type, text::max_attributes> const& lookup, text::text_source body_text, text::text_source const* parameters, size_t parameter_count) {

	auto attributes_satisfied = [](auto const& matching_againt, auto const& provided) { 
		for(uint32_t i = 0; i < text::max_attributes; ++i) {
			if(provided[i] == text::attribute_type::undefined)
				break;

			bool matched = false;
			for(uint32_t j = 0; j < text::max_attributes; ++j) {
				if(matching_againt[j] == text::attribute_type::undefined)
					return false;
				if(matching_againt[j] == provided[i]) {
					matched = true;
					break;
				}
			}
			if(!matched)
				return false;
		}
		return true;
	};

	text::formatted_text rval;

	uint32_t match_sets = uint32_t(-1);

	if(std::holds_alternative<text::handle>(body_text.input_text)) {
		auto& main_entry = lookup_entry(container, std::get<text::handle>(body_text.input_text), lookup);
		prepare_key_content(container, main_entry);

		rval.text_content = native_string(container.text_data.data() + (main_entry.string_data >> 16), container.text_data.data() + (main_entry.string_data >> 16) + (main_entry.string_data & 0xFFFF));
		rval.formatting_content = std::vector<text::format_marker>(container.fomatting_storage.data() + (main_entry.formatting_data >> 16), container.fomatting_storage.data() + (main_entry.formatting_data >> 16) + (main_entry.formatting_data & 0xFFFF));
		rval.provided_attribues = main_entry.provided_attributes;

		match_sets = main_entry.match_set_entries;
	} else {
		auto tc = std::get<formatted_text_reference>(body_text.input_text).text_content;
		auto fc = std::get<formatted_text_reference>(body_text.input_text).formatting_content;

		rval.text_content = native_string(tc, tc + std::get<formatted_text_reference>(body_text.input_text).text_length);
		rval.formatting_content = std::vector<text::format_marker>(fc, fc + std::get<formatted_text_reference>(body_text.input_text).formatting_length);
		rval.provided_attribues = std::get<formatted_text_reference>(body_text.input_text).provided_attribues;
	}

	// perform matches and substitutions
	
	for(uint32_t i = 0; i < rval.formatting_content.size(); ++i) {
		if(std::holds_alternative<variable>(rval.formatting_content[i].format)) {
			auto var = std::get<variable>(rval.formatting_content[i].format);
			std::array<text::attribute_type, text::max_attributes> lookup = { attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined, attribute_type::undefined };

			auto original_param_location = i;
			uint32_t lookup_added = 0;
			while(i + 1 < rval.formatting_content.size() && std::holds_alternative<variable_attributes>(rval.formatting_content[i + 1].format)) {
				++i;
				if(lookup_added < text::max_attributes) {
					lookup[lookup_added] = std::get<variable_attributes>(rval.formatting_content[i].format).low;
					lookup[lookup_added + 1] = std::get<variable_attributes>(rval.formatting_content[i].format).high;
				}
				lookup_added += 2;
			}

			bool found_match = false;
			for(uint32_t j = 0; j < parameter_count; ++j) {
				if(parameters[j].var == var) {
					found_match = true;

					if(std::holds_alternative<text::handle>(parameters[j].input_text)) {
						auto& main_entry = lookup_entry(container, std::get<text::handle>(parameters[j].input_text), lookup);
						prepare_key_content(container, main_entry);

						auto added_codepoints = main_entry.string_data & 0xFFFF;
						auto added_formatting = main_entry.formatting_data & 0xFFFF;
						auto insertion_position = rval.formatting_content[i].position;

						rval.text_content.insert(insertion_position, container.text_data.data() + (main_entry.string_data >> 16), added_codepoints);
						// end of substitution mark
						rval.formatting_content.insert(rval.formatting_content.begin() + i + 1, text::format_marker{ uint16_t(insertion_position), substitution_mark{ var.id } });

						for(uint32_t k = i + 1; k < rval.formatting_content.size(); ++k) {
							rval.formatting_content[k].position += uint16_t(added_codepoints);
						}
						rval.formatting_content.insert(rval.formatting_content.begin() + i + 1, container.fomatting_storage.begin() + (main_entry.formatting_data >> 16), container.fomatting_storage.begin() + (main_entry.formatting_data >> 16) + added_formatting);

						for(uint32_t k = 0; k < added_formatting; ++k) {
							rval.formatting_content[i + 1 + k].position += uint16_t(insertion_position);
						}
					} else {
						auto tc = std::get<formatted_text_reference>(parameters[j].input_text).text_content;
						auto fc = std::get<formatted_text_reference>(parameters[j].input_text).formatting_content;

						auto added_codepoints = std::get<formatted_text_reference>(parameters[j].input_text).text_length;
						auto added_formatting = std::get<formatted_text_reference>(parameters[j].input_text).formatting_length;
						auto insertion_position = rval.formatting_content[i].position;

						rval.text_content.insert(insertion_position, tc, added_codepoints);
						// end of substitution mark
						rval.formatting_content.insert(rval.formatting_content.begin() + i + 1, text::format_marker{ uint16_t(insertion_position), substitution_mark{ var.id } });

						for(uint32_t k = i + 1; k < rval.formatting_content.size(); ++k) {
							rval.formatting_content[k].position += uint16_t(added_codepoints);
						}
						rval.formatting_content.insert(rval.formatting_content.begin() + i + 1, fc, fc + added_formatting);

						for(uint32_t k = 0; k < added_formatting; ++k) {
							rval.formatting_content[i + 1 + k].position += uint16_t(insertion_position);
						}
					}
					break;
				}
			}

			if(!found_match) {
				//  currently -- leave blank
			} else {
				rval.formatting_content[original_param_location].format = substitution_mark{ var.id };
			}
		} else if(match_sets != uint32_t(-1) && (std::holds_alternative<match_set_reference>(rval.formatting_content[i].format) || std::holds_alternative<match_set_terminal_reference>(rval.formatting_content[i].format))) {

			while(i < rval.formatting_content.size()) {
				// check for match
				bool match_possible = true;
				uint16_t match_id = 0;
				if(std::holds_alternative<match_set_reference>(rval.formatting_content[i].format)) {
					match_id = std::get<match_set_reference>(rval.formatting_content[i].format).id;
				} else {
					match_id = std::get<match_set_terminal_reference>(rval.formatting_content[i].format).id;
				}
				auto option_offset = match_sets + match_id;

				while(option_offset < container.match_options.size() && container.match_options[option_offset].variable.id != 0xFFFF && match_possible) {
					auto var = container.match_options[option_offset].variable;
					match_possible = false;
					for(uint32_t j = 0; j < parameter_count; ++j) {
						if(parameters[j].var == var) {
							if(std::holds_alternative<text::handle>(parameters[j].input_text)) {
								auto& main_entry = lookup_entry(container, std::get<text::handle>(parameters[j].input_text), container.match_options[option_offset].payload.attributes.lookup_attributes);
								match_possible = attributes_satisfied(main_entry.provided_attributes, container.match_options[option_offset].payload.attributes.match_attributes);
							} else {
								match_possible = attributes_satisfied(std::get<formatted_text_reference>(parameters[j].input_text).provided_attribues, container.match_options[option_offset].payload.attributes.match_attributes);
							}
							break;
						}
					}
					++option_offset;
				}

				if(match_possible) {
					// skip remaining matches
					while(i < rval.formatting_content.size() && !std::holds_alternative<match_set_terminal_reference>(rval.formatting_content[i].format)) {
						++i;
					}

					// substitute content
					if(option_offset < container.match_options.size()) {
						assert(container.match_options[option_offset].variable.id == 0xFFFF);

						auto text_source = container.match_options[option_offset].payload.content.string_data;
						auto fmt_source = container.match_options[option_offset].payload.content.formatting_data;

						auto added_codepoints = text_source & 0xFFFF;
						auto added_formatting = fmt_source & 0xFFFF;
						auto insertion_position = rval.formatting_content[i].position;

						rval.text_content.insert(insertion_position, container.text_data.data() + (text_source >> 16), added_codepoints);

						for(uint32_t k = i + 1; k < rval.formatting_content.size(); ++k) {
							rval.formatting_content[k].position += uint16_t(added_codepoints);
						}
						rval.formatting_content.insert(rval.formatting_content.begin() + i + 1, container.fomatting_storage.begin() + (fmt_source >> 16), container.fomatting_storage.begin() + (fmt_source >> 16) + added_formatting);

						for(uint32_t k = 0; k < added_formatting; ++k) {
							rval.formatting_content[i + 1 + k].position += uint16_t(insertion_position);
						}
					}
					break;
				}

				if(std::holds_alternative<match_set_terminal_reference>(rval.formatting_content[i].format))
					break;
				++i;
			}

			// skip remaining matches
			while(i < rval.formatting_content.size() && std::holds_alternative<match_set_reference>(rval.formatting_content[i].format) && !std::holds_alternative<match_set_terminal_reference>(rval.formatting_content[i].format)) {
				++i;
			}
		}
	}

	return rval;
}

void populate_with_file_content(backing_arrays& container, char const* start, char const* end) {

	start = end_of_whitespace(start, end);
	while(start < end) {
		auto cident_start = end_of_whitespace(start, end);
		auto end_name = read_until_separator(cident_start, end);

		std::string_view name(cident_start, end_name);
		uint32_t index = uint32_t(container.entries.size());
		if(auto it = container.entry_lookup.find(name); it != container.entry_lookup.end()) {
			auto node = it->second.id;
			while(container.entries[node].next_entry != uint32_t(-1)) {
				node = container.entries[node].next_entry;
			}
			container.entries[node].next_entry = index;
		} else {
			container.entry_lookup.insert_or_assign(std::string(name), text::handle{ index });
		}
		container.entries.emplace_back();
		container.entries[index].next_entry = uint32_t(-1);

		start = end_name;
		uint32_t last_attribute = 0;
		while(start < end) {
			auto next = codepoint_from_utf8(start, end);
			if(!codepoint_is_space(next)) {

				auto end_attr = read_until_separator(start, end);
				// read attribute

				auto attr = name_to_attribute(container, std::string_view(start, end_attr));
				if(last_attribute < text::max_attributes)
					container.entries[index].lookup_attributes[last_attribute] = attr;

				++last_attribute;
				start = end_attr;
			} else {
				break;
			}
		}

		if(last_attribute < text::max_attributes)
			container.entries[index].lookup_attributes[last_attribute] = text::attribute_type::undefined;

		start = end_of_whitespace(start, end);
		auto end_of_body = read_until_newline(start, end);

		container.entries[index].raw_text = start;
		container.entries[index].raw_text_length = uint16_t(end_of_body - start);

		start = end_of_whitespace(end_of_body, end);
	}
}

void reset(backing_arrays& container) {
	container.text_data.clear();
	container.fomatting_storage.clear();
	container.entries.clear();
	container.match_options.clear();

	container.first_free_variable = 0;
	container.first_free_attribute = int8_t(text::attribute_type::last_predefined) + 1;

	container.entry_lookup.clear();
	container.variable_lookup.clear();
	container.attribute_lookup.clear();
}

} // namesapce text
} // namespace minui
