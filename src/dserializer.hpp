/**************************************************************************/
/*  dserializer.hpp                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                      GODOT UNIVERSAL SERIALIZER                        */
/*  https://github.com/Daylily-Zeleen/GUS-Godot-Universal-Serializer-2.0  */
/**************************************************************************/
/* Copyright (c) 2022-present Daylily Zeleen.                             */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#if _HAS_CXX20
#include <concepts>
#define HAS_CXX20
#else // _HAS_CXX20
#include <type_traits>
#endif // _HAS_CXX20

#include <assert.h>
#include <memory>

#if HAS_CXX20
template <typename T>
concept EncodingBuffer = std::integral<T> && sizeof(T) == 1 && !std::is_same_v<T, bool>;
#define buffer_t EncodingBuffer auto
#else // HAS_CXX20
#define IS_BUFFER_T(T) std::enable_if_t<sizeof(T) == 1 || !std::is_same_v<T, bool>> *_##T = nullptr
template <typename T> //, IS_BUFFER_T(T)>
using EncodingBuffer = T;
#define buffer_t EncodingBuffer<TBuffer>
#endif // HAS_CXX20

// Should always inline no matter what.
#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

// Should always inline, except in debug builds because it makes debugging harder.
#ifndef _FORCE_INLINE_
#ifdef DISABLE_FORCED_INLINE
#define _FORCE_INLINE_ inline
#else
#define _FORCE_INLINE_ _ALWAYS_INLINE_
#endif
#endif

#define _INLINE_ _FORCE_INLINE_

#if HAS_CXX20
#define integral_t std::integral auto
#else // HAS_CXX20
#define IS_INTEGRAL_T(T) std::enable_if_t<std::is_integral_v<T>> *_##T = nullptr
template <typename T, IS_INTEGRAL_T(T)>
using Integral = T;
#define integral_t Integral<TInt>
#endif // HAS_CXX20

#define decltype_pure(m_var) std::remove_const_t<std::remove_reference_t<std::remove_pointer_t<decltype(m_var)>>>

// #define ENCODE_LEN_METHOD

namespace dserializer {

#if HAS_CXX20
template <typename T>
concept Number = (std::convertible_to<T, int64_t> && std::integral<T>) ||
		(std::convertible_to<T, double> && std::floating_point<T>);
#define number_t Number auto
#else // HAS_CXX20
#define IS_NUMBER_T(T) std::enable_if_t<(std::is_convertible_v<T, int64_t> && std::is_integral_v<T>) || (std::is_convertible_v<T, double> && std::is_floating_point_v<T>)> *_##T = nullptr
template <typename T, IS_NUMBER_T(T)>
using Number = T;
#define number_t Number<TNumber>
#endif // HAS_CXX20

#if HAS_CXX20
template <typename T>
concept RandomSizedRange = std::ranges::random_access_range<T> && std::ranges::sized_range<T>;
#define random_sized_range_t RandomSizedRange auto
#else // HAS_CXX20
#define IS_RANDOMSIZED_RANGE_T(T) std::enable_if_t<std::is_invocable_v<decltype(&T::operator[]), int> && std::is_invocable_v<decltype(&T::size)>> *_##T = nullptr
template <typename T, IS_RANDOMSIZED_RANGE_T(T)>
using RandomSizedRange = T;
#define random_sized_range_t RandomSizedRange<TRandomSizedRange>
#endif // HAS_CXX20

#if HAS_CXX20
template <typename T>
concept Pair = requires(T t) {
	requires T::first &&std::default_initializable<decltype((t.first))>;
	requires T::second &&std::default_initializable<decltype((t.second))>;
};
#define pair_t Pair auto
#else //HAS_CXX20
#define IS_PAIR_T(T) std::enable_if_t<(std::is_member_object_pointer_v<decltype(&T::first)> && std::is_member_object_pointer_v<decltype(&T::second)> && \
							 std::is_default_constructible_v<decltype(&T::first)> && std::is_default_constructible_v<decltype(&T::second)>)> *_##T = nullptr
template <typename T, IS_PAIR_T(T)>
using Pair = T;
#define pair_t Pair<TPair>
#endif // HAS_CXX20

#if HAS_CXX20
template <typename T>
concept NormalRange = std::ranges::range<T> && (!std::ranges::random_access_range<T>)&&std::ranges::sized_range<T>;
#define normal_range_t NormalRange auto
#else // HAS_CXX20
#define IS_NORMAL_RANGE_T(T) std::enable_if_t<std::is_invocable_v<decltype(&T::begin)> && std::is_invocable_v<decltype(&T::end)>> *_##T = nullptr
template <typename T, IS_NORMAL_RANGE_T(T)>
using NormalRange = T;
#define normal_range_t NormalRange<TNormalRange>
#endif // HAS_CXX20

#if HAS_CXX20
template <typename T>
concept StdString = std::is_invocable_r_v<const char *, decltype(T::c_str)> && T::length && std::is_constructible_v<T, const char *>;
#define std_str_t StdString auto
#else // HAS_CXX20
#define IS_STD_STR_T(T) std::enable_if_t<std::is_invocable_v<decltype(&T::length)> && std::is_invocable_r_v<const char *, decltype(T::c_str)> && std::is_constructible_v<T, const char *>> *_##T = nullptr
template <typename T, IS_STD_STR_T(T)>
using StdStr = T;
#define std_str_t StdStr<TStdStr>
#endif // HAS_CXX20
//==============================================

#ifndef HAS_CXX20
#define IF_NOT_CXX20(m_code) (m_code)
#else // !HAS_CXX20
#define IF_NOT_CXX20(m_code)
#endif // !HAS_CXX20

// Number types
#if !HAS_CXX20
template <typename TNumber, typename TInt, IS_BUFFER_T(TNumber), IS_INTEGRAL_T(TInt)>
#endif // !HAS_CXX20
_INLINE_ void cal_size(number_t p_val, integral_t &r_len) {
	r_len += sizeof(decltype_pure(p_val));
}

_INLINE_ void memcpy_reverse(
		_Out_writes_bytes_all_(_Size) void *p_dst,
		_In_reads_bytes_(_Size) void const *p_src,
		_In_ size_t p_size) {
	assert(p_dst);
	assert(p_src);
	for (p_size -= 1; p_size >= 0; --p_size) {
		*(char *)p_dst = ((char *)(p_src))[p_size];
		p_dst = (char *)p_dst + 1;
	}
}

#define MEMCPY(m_keep_order, m_dst, m_src, m_size) \
	if constexpr (m_keep_order) {                  \
		memcpy(m_dst, m_src, m_size);              \
	} else {                                       \
		memcpy_reverse(m_dst, m_src, m_size);      \
	}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TNumber, IS_BUFFER_T(TBuffer), IS_NUMBER_T(TNumber)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const number_t &p_val) {
	MEMCPY(little_endin || sizeof(p_val) == 1, p_buf, &p_val, sizeof(decltype_pure(p_val)));
	p_buf += sizeof(decltype_pure(p_val));
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <typename TBuffer, typename TNumber, typename TInt, IS_BUFFER_T(TBuffer), IS_NUMBER_T(TNumber), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, number_t p_val, integral_t &r_len) {
	MEMCPY(little_endin || sizof(p_val) == 1, p_buf, &p_val, sizeof(decltype_pure(p_val)));
	p_buf += sizeof(decltype_pure(p_val));
	r_len += sizeof(decltype_pure(p_val));
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TNumber, IS_BUFFER_T(TBuffer), IS_NUMBER_T(TNumber)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, number_t &r_val) {
	MEMCPY(little_endin || sizeof(r_val) == 1, &r_val, p_buf, sizeof(decltype_pure(r_val)));
	p_buf += sizeof(decltype_pure(r_val));
}

//============= Fixint ================
#if !HAS_CXX20
template <typename TV, typename TR, IS_INTEGRAL_T(TV), IS_INTEGRAL_T(TR)>
_INLINE_ void cal_size_int(Integral<TV> p_val, Integral<TR> &r_len) {
#else // !HAS_CXX20
_INLINE_ void cal_size_int(integral_t p_val, integral_t &r_len) {
#endif // !HAS_CXX20
	cal_size(p_val, r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode_int(buffer_t *&p_buf, const integral_t &p_val) {
	encode<little_endin>(p_buf, p_val);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TNumber, typename TInt, IS_BUFFER_T(TBuffer), IS_NUMBER_T(TNumber), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode_int(buffer_t *&p_buf, integral_t p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val, r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void decode_int(buffer_t *&p_buf, integral_t &r_val) {
	decode<little_endin>(p_buf, r_val);
}

// ======== Varint Zigzag ========
#define unsigned_int_t(int_t) std::make_unsigned<int_t>::type

#if !HAS_CXX20
template <bool little_endin, typename TInt, IS_INTEGRAL_T(TInt)>
_INLINE_ auto encode_zigzag(integral_t p_val) {
#else // !HAS_CXX20
template <bool little_endin, std::integral TInt>
_INLINE_ auto encode_zigzag(TInt p_val) {
#endif // !HAS_CXX20
	using ret_t = typename unsigned_int_t(TInt);
	if constexpr (!little_endin && sizeof(p_val) > 1) {
		auto tmp = p_val;
		memcpy_reverse(&p_val, &tmp, sizeof(p_val));
	}
	return (ret_t)((p_val << 1) ^ (p_val >> (sizeof(ret_t) * 8 - 1)));
}

#if !HAS_CXX20
template <bool little_endin, typename TInt, IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin, std::integral TInt>
#endif // !HAS_CXX20
_INLINE_ TInt decode_zigzag(typename unsigned_int_t(TInt) p_val) {
	auto ret = (TInt)((p_val >> 1) ^ -(p_val & 1));
	if constexpr (!little_endin && sizeof(p_val) > 1) {
		auto tmp = ret;
		memcpy_reverse(&ret, &tmp, sizeof(p_val));
		return ret;
	} else {
		return ret;
	}
}

#if !HAS_CXX20
template <bool little_endin, typename TInt, typename TR, IS_INTEGRAL_T(TInt), IS_INTEGRAL_T(TR)>
_INLINE_ void cal_size_varint(Integral<TInt> p_integer, Integral<TR> &r_len) {
#else // !HAS_CXX20
template <bool little_endin, std::integral TInt>
_INLINE_ void cal_size_varint(TInt p_integer, integral_t &r_len) {
#endif // !HAS_CXX20
	typename unsigned_int_t(TInt) tmp = encode_zigzag<little_endin>(p_integer);
	while ((tmp >> 7) != 0) {
		tmp >>= 7;
		r_len++;
	}
	r_len++;
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
_INLINE_ void encode_varint(buffer_t *&p_buf, integral_t p_integer) {
#else // !HAS_CXX20
template <bool little_endin>
_INLINE_ void encode_varint(buffer_t *&p_buf, integral_t p_integer) {
#endif // !HAS_CXX20
	typename unsigned_int_t(decltype_pure(p_integer)) tmp = encode_zigzag<little_endin>(p_integer);
	while ((tmp >> 7) != 0) {
		*(p_buf++) = (uint8_t)(tmp | 0x80);
		tmp >>= 7;
	}
	*(p_buf++) = (uint8_t)tmp;
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, typename TR, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt), IS_INTEGRAL_T(TR)>
_INLINE_ void encode_varint(buffer_t *&p_buf, Integral<TInt> &p_integer, Integral<TR> &r_len) {
#else // !HAS_CXX20
template <bool little_endin, std::integral TInt>
_INLINE_ void encode_varint(buffer_t *&p_buf, TInt integer, integral_t &r_len) {
#endif // !HAS_CXX20
	typename unsigned_int_t(TInt) tmp = encode_zigzag<little_endin>(integer);
	while ((tmp >> 7) != 0) {
		*(p_buf++) = (uint8_t)(tmp | 0x80);
		tmp >>= 7;
		r_len++;
	}
	*(p_buf++) = (uint8_t)tmp;
	r_len++;
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
_INLINE_ void decode_varint(buffer_t *&p_buf, integral_t &r_val) {
#else // !HAS_CXX20
template <bool little_endin, std::integral TInt>
_INLINE_ void decode_varint(buffer_t *&p_buf, TInt &r_val) {
#endif // !HAS_CXX20
	using tmp_t = typename unsigned_int_t(TInt);
	tmp_t tmp = 0;
	uint8_t i = 0;
	tmp_t b = (tmp_t)(*(p_buf++));
	while ((b & 0x80) != 0) {
		tmp |= ((b & 0x7f) << (7 * i));
		b = *(p_buf++);
		i++;
	}
	tmp |= (b << (7 * i));
	r_val = decode_zigzag<little_endin, TInt>(tmp);
}

// === String ===
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif // !HAS_CXX20
_INLINE_ void cal_size(const char *p_cstr, integral_t &r_len) {
	const char *ptr = p_cstr;
	while (*ptr != 0) {
		ptr++;
	}
	ptr++;
	r_len += ptr - p_cstr;
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const char *p_cstr) {
	const char *ptr = p_cstr;
	while (*ptr != 0) {
		*(p_buf++) = *(ptr++);
	}
	*(p_buf++) = *(ptr++);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const char *p_cstr, integral_t &r_len) {
	const char *ptr = p_cstr;
	while (*ptr != 0) {
		*(p_buf++) = *(ptr++);
	}
	*(p_buf++) = *(ptr++);
	r_len += ptr - p_cstr;
}
#endif

#if !HAS_CXX20
template <typename TStdStr, typename TInt, IS_STD_STR_T(TStdStr), IS_INTEGRAL_T(TInt)>
#endif // !HAS_CXX20
_INLINE_ void cal_size(const std_str_t &p_str, integral_t &r_len) {
	r_len += p_str.length();
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TStdStr, IS_BUFFER_T(TBuffer), IS_STD_STR_T(TStdStr)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const std_str_t &p_str) {
	memcpy(p_buf, p_str.c_str(), p_str.length());
	p_buf += p_str.length();
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TStdStr, typename TInt, IS_BUFFER_T(TBuffer), IS_STD_STR_T(TStdStr), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const std_str_t &p_str, integral_t &r_len) {
	memcpy(p_buf, p_str.c_str(), p_str.length());
	p_buf += p_str.length();
	r_len += p_str.length();
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TStdStr, IS_BUFFER_T(TBuffer), IS_STD_STR_T(TStdStr)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, std_str_t &r_val) {
	size_t len = 0;
	while (*(p_buf + len) != 0) {
		len++;
	}
	len++;
	r_val = decltype_pure(r_val)(reinterpret_cast<const char *>(p_buf));
	p_buf += len;
}

// ======== c Array ==========
#if !HAS_CXX20
template <typename T, typename TInt, typename TR, IS_INTEGRAL_T(TInt), IS_INTEGRAL_T(TR)>
_INLINE_ void cal_size(const T *p_arr, integral_t p_array_size, Integral<TR> &r_len) {
#else // !HAS_CXX20
template <typename T>
_INLINE_ void cal_size(const T *p_arr, integral_t p_array_size, integral_t &r_len) {
#endif // !HAS_CXX20
	cal_size(p_array_size, r_len);
	for (integral_t i = 0; i < p_array_size; i++) {
		cal_size<T>(p_array_size[i], r_len);
	}
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, typename T, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin, typename T>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const T *p_arr, integral_t p_array_size) {
	encode_varint<little_endin, decltype_pure(p_array_size)>(p_buf, p_array_size);
	for (integral_t i = 0; i < p_array_size; i++) {
		encode<little_endin, T>(p_buf, p_arr[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, typename TR, typename T, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt), IS_INTEGRAL_T(TR)>
_INLINE_ void encode(buffer_t *&p_buf, const T *p_arr, integral_t p_array_size, Integral<TR> &r_len) {
#else // !HAS_CXX20
template <bool little_endin, typename T>
_INLINE_ void encode(buffer_t *&p_buf, const T *p_arr, integral_t p_array_size, integral_t &r_len) {
#endif // !HAS_CXX20
	encode_varint<little_endin, decltype_pure(p_array_size)>(p_buf, p_array_size, r_len);
	for (integral_t i = 0; i < p_array_size; i++) {
		encode<little_endin, T>(p_buf, p_arr[i], r_len);
	}
}
#endif

// Your should ensure the passed "r_arr" size is larger than "r_array_size".
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, typename T, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin, typename T>
#endif // !HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, T *r_arr, integral_t r_array_size) {
	decode_varint<little_endin, decltype_pure(r_array_size)>(p_buf, r_array_size);
	for (integral_t i = 0; i < r_array_size; i++) {
		decode<little_endin>(p_buf, r_arr[i]);
	}
}

// ================ Pairs  =====================
#if !HAS_CXX20
template <typename TPair, typename TInt, IS_PAIR_T(TPair), IS_INTEGRAL_T(TInt)>
#endif // !HAS_CXX20
_INLINE_ void cal_size(const pair_t &p_pair, integral_t &r_len) {
	cal_size(p_pair.first, r_len);
	cal_size(p_pair.second, r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TPair, IS_BUFFER_T(TBuffer), IS_PAIR_T(TPair)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const pair_t &p_pair) {
	encode<little_endin>(p_buf, p_pair.first);
	encode<little_endin>(p_buf, p_pair.second);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TPair, typename TInt, IS_BUFFER_T(TBuffer), IS_PAIR_T(TPair), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const pair_t &p_pair, integral_t &r_len) {
	encode<little_endin>(p_buf, p_pair.first, r_len);
	encode<little_endin>(p_buf, p_pair.second, r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TPair, IS_BUFFER_T(TBuffer), IS_PAIR_T(TPair)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, pair_t &r_pair) {
	decode<little_endin>(p_buf, r_pair.first);
	decode<little_endin>(p_buf, r_pair.second);
}

// Normal Rnage containers.

#if !HAS_CXX20
template <typename TNormalRange, typename TInt, IS_NORMAL_RANGE_T(TNormalRange), IS_INTEGRAL_T(TInt)>
#endif // !HAS_CXX20
_INLINE_ void cal_size(const normal_range_t &p_container, integral_t &r_len) {
	cal_size(p_container.size(), r_len);
	for (auto element : p_container) {
		cal_size(element, r_len);
	}
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TNormalRange, IS_BUFFER_T(TBuffer), IS_NORMAL_RANGE_T(TNormalRange)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const normal_range_t &p_container) {
	encode<little_endin>(p_buf, p_container.size());
	for (auto element : p_container) {
		encode<little_endin>(p_buf, element);
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TNormalRange, typename TInt, IS_BUFFER_T(TBuffer), IS_NORMAL_RANGE_T(TNormalRange), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const normal_range_t &p_container, integral_t &r_len) {
	encode<little_endin>(p_buf, p_container.size(), r_len);
	for (auto element : p_container) {
		encode<little_endin>(p_buf, element, r_len);
	}
}
#endif

// Randaom access range contaners.
#if !HAS_CXX20
template <typename TRandomSizedRange, typename TInt, IS_RANDOMSIZED_RANGE_T(TRandomSizedRange), IS_INTEGRAL_T(TInt)>
#endif // !HAS_CXX20
_INLINE_ void cal_size(const random_sized_range_t &p_container, integral_t &r_len) {
	auto size = p_container.size();
	cal_size(size, r_len);
	for (decltype_pure(size) i = 0; i < size; i++) {
		cal_size(p_container[i], r_len);
	}
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TRandomSizedRange, IS_BUFFER_T(TBuffer), IS_RANDOMSIZED_RANGE_T(TRandomSizedRange)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const random_sized_range_t &p_container) {
	auto size = p_container.size();
	encode<little_endin>(p_buf, size);
	for (decltype_pure(size) i = 0; i < size; i++) {
		encode<little_endin>(p_buf, p_container[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TRandomSizedRange, typename TInt, IS_BUFFER_T(TBuffer), IS_RANDOMSIZED_RANGE_T(TRandomSizedRange), IS_INTEGRAL_T(TInt)>
#else // !HAS_CXX20
template <bool little_endin>
#endif // !HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const random_sized_range_t &p_container, integral_t &r_len) {
	auto size = p_container.size();
	encode<little_endin>(p_buf, size, r_len);
	for (decltype_pure(size) i = 0; i < size; i++) {
		encode<little_endin>(p_buf, p_container[i], r_len);
	}
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TContainer, typename TE, IS_BUFFER_T(TBuffer), std::enable_if<std::is_default_constructible_v<TE> && std::is_same_v<decltype((TContainer::value_type)), TE>> *dammpy = nullptr>
#else // !HAS_CXX20
template <bool little_endin, typename TContainer, std::default_initializable TE>
requires requires(TContainer t) {
	requires std::ranges::range<TContainer>;
	requires std::default_initializable<TE>;
	requires std::same_as<decltype((TContainer::value_type)), TE>;
}
#endif // !HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, TContainer &r_container) {
	size_t size;
	decode<little_endin, size_t>(p_buf, size);
	for (size_t i = size; i > 0; i--) {
		TE e;
		decode<little_endin>(p_buf, e);
		if constexpr (std::is_invocable_v<decltype(TContainer::emplace_back), TE>) {
			r_container.emplace_back(e);
		} else if constexpr (std::is_invocable_v<decltype(TContainer::emplace), TE>) {
			r_container.emplace(e);
		}
	}
}
} //namespace dserializer
