#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <type_traits>

using cstr_ptr = char *;

#define BUFFER_U8
#ifdef BUFFER_U8
using buffer_ptr = uint8_t *;
#else
using buffer_ptr = int8_t *;
#endif

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

#define integral_t std::integral auto

// #define ENCODE_LEN_METHOD

namespace dserializer {

template <typename T>
concept Number =
		(std::convertible_to<T, int64_t> && std::integral<T>) || (std::convertible_to<T, double> && std::floating_point<T>);
#define number_t Number auto

template <typename T>
concept random_sized_range = std::ranges::random_access_range<T> && std::ranges::sized_range<T>;

template <typename T>
concept Pair = requires(T t) {
	requires T::first &&std::default_initializable<decltype((t.first))>;
	requires T::second &&std::default_initializable<decltype((t.second))>;
};
#define pair_t Pair auto

template <typename T>
concept NormalRange =
		std::ranges::range<T> && (!std::ranges::random_access_range<T>)&&std::ranges::sized_range<T>;

//==============================================
_INLINE_ void cal_size(number_t p_val, integral_t &r_len) {
	r_len += sizeof(decltype(p_val));
}
_INLINE_ void encode(buffer_ptr p_buf, const number_t &p_val) {
	memcpy(p_buf, &p_val, sizeof(decltype(p_val)));
	p_buf += sizeof(decltype(p_val));
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_ptr p_buf, const number_t &p_val, integral_t &r_len) {
	memcpy(p_buf, &p_val, sizeof(decltype(p_val)));
	p_buf += sizeof(decltype(p_val));
	r_len += sizeof(decltype(p_val));
}
#endif
_INLINE_ void decode(buffer_ptr p_buf, number_t &r_val) {
	memcpy(&r_val, p_buf, sizeof(decltype(r_val)));
	p_buf += sizeof(decltype(r_val));
}

//=============  fixint ================
_INLINE_ void cal_size_int(integral_t p_val, integral_t &r_len) {
	cal_size(p_val, r_len);
}
_INLINE_ void encode_int(buffer_ptr p_buf, const integral_t &p_val) {
	encode(p_buf, p_val);
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_int(buffer_ptr p_buf, const integral_t &p_val, integral_t &r_len) {
	encode(p_buf, p_val, r_len);
}
#endif
_INLINE_ void decode_int(buffer_ptr p_buf, integral_t &r_val) {
	decode(p_buf, r_val);
}

// ======== Zigzag ========
#define unsigned_int_t(int_t) std::make_unsigned<int_t>::type

template <std::integral TInt>
_INLINE_ auto encode_zigzag(TInt p_val) {
	using ret_t = typename unsigned_int_t(TInt);
	return (ret_t)((p_val << 1) ^ (p_val >> (sizeof(ret_t) * 8 - 1)));
}

template <std::integral TInt>
_INLINE_ TInt decode_zigzag(typename unsigned_int_t(TInt) val) {
	return (TInt)((val >> 1) ^ -(val & 1));
}

// ======== Varint ========

template <std::integral TInt>
_INLINE_ void cal_size_varint(TInt p_integer, integral_t &r_len) {
	typename unsigned_int_t(TInt) tmp = 0;
	encode_zigzag(p_integer, tmp);
	while ((tmp >> 7) != 0) {
		tmp >>= 7;
		r_len++;
	}
	r_len++;
}

template <std::integral TInt>
_INLINE_ void encode_varint(buffer_ptr p_buf, TInt integer) {
	typename unsigned_int_t(TInt) tmp = encode_zigzag(integer);
	while ((tmp >> 7) != 0) {
		*(p_buf++) = (uint8_t)(tmp | 0x80);
		tmp >>= 7;
	}
	*(p_buf++) = (uint8_t)tmp;
}

#ifdef ENCODE_LEN_METHOD
template <std::integral TInt>
_INLINE_ void encode_varint(buffer_ptr p_buf, TInt integer, integral_t &r_len) {
	typename unsigned_int_t(TInt) tmp = encode_zigzag(integer);
	while ((tmp >> 7) != 0) {
		*(p_buf++) = (uint8_t)(tmp | 0x80);
		tmp >>= 7;
		r_len++;
	}
	*(p_buf++) = (uint8_t)tmp;
	r_len++;
}
#endif

template <std::integral TInt>
_INLINE_ void decode_varint(buffer_ptr p_buf, TInt &r_val) {
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
	r_val = decode_zigzag(tmp);
}

//// ����δ�ػ�
//_INLINE_ void encode(buffer_ptr& p_buf, const integral_t& p_val, integral_t& r_len) {
//	memcpy(p_buf, &p_val, sizeof(decltype(p_val)));
//	p_buf += sizeof(decltype(p_val));
//	r_len += sizeof(decltype(p_val));
//}
//_INLINE_ void decode(buffer_ptr& p_buf, integral_t& r_val) {
//	memcpy(&r_val, p_buf, sizeof(decltype(r_val)));
//	p_buf += sizeof(decltype(r_val));
//}

// === �ַ��� ===
_INLINE_ void cal_size(const cstr_ptr &p_cstr, integral_t &r_len) {
	cstr_ptr ptr = p_cstr;
	while (*ptr != 0)
		ptr++;
	ptr++;
	r_len += ptr - p_cstr;
}
_INLINE_ void encode(buffer_ptr p_buf, const cstr_ptr &p_cstr) {
	cstr_ptr ptr = p_cstr;
	while (*ptr != 0) {
		*(p_buf++) = *(ptr++);
	}
	*(p_buf++) = *(ptr++);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_ptr p_buf, const cstr_ptr &p_cstr, integral_t &r_len) {
	cstr_ptr ptr = p_cstr;
	while (*ptr != 0) {
		*(p_buf++) = *(ptr++);
	}
	*(p_buf++) = *(ptr++);
	r_len += ptr - p_cstr;
}
#endif
_INLINE_ void cal_size(const std::string &p_str, integral_t &r_len) {
	r_len += p_str.length();
}
_INLINE_ void encode(buffer_ptr p_buf, const std::string &p_str) {
	memcpy(p_buf, p_str.c_str(), p_str.length());
	p_buf += p_str.length();
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_ptr p_buf, const std::string &p_str, integral_t &r_len) {
	memcpy(p_buf, p_str.c_str(), p_str.length());
	p_buf += p_str.length();
	r_len += p_str.length();
}
#endif
_INLINE_ void decode(buffer_ptr p_buf, std::string &r_val) {
	size_t len = 0;
	while (*(p_buf + len) != 0) {
		len++;
	}
	len++;
	r_val = std::string(reinterpret_cast<cstr_ptr>(p_buf), len);
	p_buf += len;
}

// ======== ���飬��֧����Ƕ���飨�޷��жϳ��ȣ� ==========
template <typename T> // ֻ�����ڼ�ģ��
_INLINE_ void cal_size(const T *p_arr, integral_t p_array_size, integral_t &r_len) {
	cal_size(p_array_size, r_len);
	for (integral_t i = 0; i < p_array_size; i++) {
		cal_size<T>(p_array_size[i], r_len);
	}
}
template <typename T> // ֻ�����ڼ�ģ��
_INLINE_ void encode(buffer_ptr p_buf, const T *p_arr, integral_t p_array_size) {
	encode_varint<decltype(p_array_size)>(p_buf, p_array_size);
	for (integral_t i = 0; i < p_array_size; i++) {
		encode<T>(p_buf, p_array_size[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
template <typename T> // ֻ�����ڼ�ģ��
_INLINE_ void encode(buffer_ptr p_buf, const T *p_arr, const integral_t &p_array_size, integral_t &r_len) {
	encode_varint<integral_t>(p_buf, p_array_size, r_len);
	for (integral_t i = 0; i < p_array_size; i++) {
		encode<T>(p_buf, p_array_size[i], r_len);
	}
}
#endif
template <typename T> // ֻ�����ڼ�ģ��
_INLINE_ void decode(buffer_ptr p_buf, T *r_arr, const integral_t &r_array_size) {
	decode_varint<decltype(r_array_size)>(p_buf, r_array_size);
	for (integral_t i = 0; i < r_array_size; i++) {
		decode(p_buf, r_arr[i]);
	}
}

// ================ ���� =====================
template <typename TContainer, std::default_initializable TE>
requires requires {
	requires std::same_as<decltype((TContainer::value_type)), TE>;
	requires std::invocable<decltype((TContainer::emplace_back)), TE>; // ˳����
}
_INLINE_ void _add_element(TContainer &p_container, TE &p_element) {
	p_container.emplace_back(p_element);
}
template <typename TContainer, std::default_initializable TE>
requires requires {
	requires std::same_as<decltype((TContainer::value_type)), TE>;
	requires std::invocable<decltype((TContainer::emplace)), TE>; // ��ϵ��
}
_INLINE_ void _add_element(TContainer &p_container, TE &p_element) {
	p_container.emplace(p_element);
}
// ��ֵ���ػ�
_INLINE_ void cal_size(const pair_t &p_pair, integral_t &r_len) {
	cal_size(p_pair.first, r_len);
	cal_size(p_pair.second, r_len);
}
_INLINE_ void encode(buffer_ptr p_buf, const pair_t &p_pair) {
	encode(p_buf, p_pair.first);
	encode(p_buf, p_pair.second);
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_ptr p_buf, const pair_t &p_pair, integral_t &r_len) {
	encode(p_buf, p_pair.first, r_len);
	encode(p_buf, p_pair.second, r_len);
}
#endif

_INLINE_ void decode(buffer_ptr p_buf, const pair_t &p_pair) {
	decode(p_buf, p_pair.first);
	decode(p_buf, p_pair.second);
}

// ͨ������

_INLINE_ void cal_size(const NormalRange auto &p_container, integral_t &r_len) {
	cal_size(p_container.size(), r_len);
	for (auto element : p_container) {
		cal_size(element, r_len);
	}
}
_INLINE_ void encode(buffer_ptr p_buf, const NormalRange auto &p_container) {
	encode(p_buf, p_container.size());
	for (auto element : p_container) {
		encode(p_buf, element);
	}
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_ptr p_buf, const NormalRange auto &p_container, integral_t &r_len) {
	encode(p_buf, p_container.size(), r_len);
	for (auto element : p_container) {
		encode(p_buf, element, r_len);
	}
}
#endif

// ����±������ػ�
_INLINE_ void cal_size(const random_sized_range auto &p_container, integral_t &r_len) {
	auto size = p_container.size();
	cal_size(size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		cal_size(p_container[i], r_len);
	}
}
_INLINE_ void encode(buffer_ptr p_buf, const random_sized_range auto &p_container) {
	auto size = p_container.size();
	encode(p_buf, size);
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, p_container[i]);
	}
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_ptr p_buf, const random_sized_range auto &p_container, integral_t &r_len) {
	auto size = p_container.size();
	encode(p_buf, size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, p_container[i], r_len);
	}
}
#endif

template <typename TContainer, std::default_initializable TE>
requires requires(TContainer t) {
	requires std::ranges::range<TContainer>;
	requires std::default_initializable<TE>;
	requires std::same_as<decltype((TContainer::value_type)), TE>;
}
_INLINE_ void decode(buffer_ptr p_buf, TContainer &r_container) {
	size_t size;
	decode<size_t>(p_buf, size);
#ifdef CLEAR_CONTAINER_BEFOR_DECODE
	r_container.clear();
#endif
	for (size_t i = size; i > 0; i--) {
		TE e;
		decode(p_buf, e);
		_add_element(r_container, e);
	}
}
} //namespace dserializer
