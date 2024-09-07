/**************************************************************************/
/*  dserializer.gdbase.hpp                                                */
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

#include "dserializer.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace dserializer {
// Concepts
#if HAS_CXX20
// godot Str
template <typename T>
concept GodotStr = !Number<T> && (std::same_as<T, String> || std::same_as<T, StringName> || std::same_as<T, NodePath>);
#define godot_str_t GodotStr auto

// GodotComponents4( Quaternion, Color)
template <typename T>
concept GodotComponents4 = !Number<T> && (std::same_as<T, Quaternion> || std::same_as<T, Color>);
#define godot_components4_t GodotComponents4 auto

template <typename T>
concept GodotVec = !Number<T> && (std::same_as<T, Vector2> || std::same_as<T, Vector2i> || std::same_as<T, Vector3> || std::same_as<T, Vector3i> || std::same_as<T, Vector4> || std::same_as<T, Vector4i>);
#define godot_vec_t GodotVec auto

// Rect2
template <typename T>
concept GodotRect2 = !Number<T> && (std::same_as<T, Rect2> || std::same_as<T, Rect2i>);
#define godot_rect2_t GodotRect2 auto

// PackedXxxArray
template <typename T>
concept GodotPackedIntArray = !Number<T> && (std::same_as<T, PackedInt32Array> || std::same_as<T, PackedInt64Array>);
#define godot_packed_int_array_t GodotPackedIntArray auto

template <typename T>
concept GodotFixedElementLenthArray = !Number<T> &&
		(GodotPackedIntArray<T> ||
				std::same_as<T, PackedByteArray> || std::same_as<T, PackedFloat32Array> ||
				std::same_as<T, PackedFloat64Array> || std::same_as<T, PackedVector2Array> ||
				std::same_as<T, PackedVector3Array> || std::same_as<T, PackedVector4Array> ||
				std::same_as<T, PackedColorArray>);
#define godot_fixed_element_length_array_t GodotFixedElementLenthArray auto

#else // HAS_CXX20

// godot Str
#define IS_GODOT_STR_T(T) std::enable_if_t<std::is_same_v<T, String> || std::is_same_v<T, StringName> || std::is_same_v<T, NodePath>> *_##T = nullptr
template <typename T, IS_GODOT_STR_T(T)>
using GodotStr = T;
#define godot_str_t GodotStr<TGodotStr>

// GodotComponents4( Quaternion, Color)
#define IS_GODOT_COMPONENTS4_T(T) std::enable_if_t<std::is_same_v<T, Quaternion> || std::is_same_v<T, Color>> *_##T = nullptr
template <typename T, IS_GODOT_COMPONENTS4_T(T)>
using GodotComponents4 = T;
#define godot_components4_t GodotComponents4<TGodotComponents4>

#define IS_GODOT_VEC_T(T) std::enable_if_t<std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector3> || std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector4> || std::is_same_v<T, Vector4i>> *_##T = nullptr
template <typename T, IS_GODOT_VEC_T(T)>
using GodotVec = T;
#define godot_vec_t GodotVec<TGodotVec>

// Rect2
#define IS_GODOT_RECT2_T(T) std::enable_if_t<std::is_same_v<T, Rect2> || std::is_same_v<T, Rect2i>> *_##T = nullptr
template <typename T, IS_GODOT_RECT2_T(T)>
using GodotRect2 = T;
#define godot_rect2_t GodotRect2<TGodotRect2>

// PackedXxxArray
#define IS_GODOT_PACKED_INT_ARRAY_T(T) std::enable_if_t<std::is_same_v<T, PackedInt32Array> || std::is_same_v<T, PackedInt64Array>> *_##T = nullptr
template <typename T, IS_GODOT_PACKED_INT_ARRAY_T(T)>
using GodotPackedIntArray = T;
#define godot_packed_int_array_t GodotPackedIntArray<TGodotPackedIntArray>

#define IS_GODOT_FIXED_ELEMENT_LENGTH_ARRAY_T(T)                                              \
	std::enable_if_t<                                                                         \
			std::is_same_v<T, PackedInt32Array> || std::is_same_v<T, PackedInt64Array> ||     \
			std::is_same_v<T, PackedByteArray> || std::is_same_v<T, PackedFloat32Array> ||    \
			std::is_same_v<T, PackedFloat64Array> || std::is_same_v<T, PackedVector2Array> || \
			std::is_same_v<T, PackedVector3Array> || std::is_same_v<T, PackedVector4Array> || \
			std::is_same_v<T, PackedColorArray>> *_##T = nullptr
template <typename T, IS_GODOT_FIXED_ELEMENT_LENGTH_ARRAY_T(T)>
using GodotFixedElementLengthArray = T;
#define godot_fixed_element_length_array_t GodotFixedElementLengthArray<TGodotFixedElementLengthArray>

#endif // HAS_CXX20

// GodotStr
#if !HAS_CXX20
template <typename TGodotStr, typename TInt, IS_GODOT_STR_T(TGodotStr), IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot_str_t &p_val, integral_t &r_len) {
	r_len += godot::String(p_val).utf8().length();
	++r_len;
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotStr, IS_BUFFER_T(TBuffer), IS_GODOT_STR_T(TGodotStr)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_str_t &p_val) {
	auto utf8 = String(p_val).utf8();
	auto length = utf8.length();
	memcpy(p_buf, utf8.get_data(), length);
	p_buf += length;
	*p_buf = 0;
	++p_buf;
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotStr, typename TInt, IS_BUFFER_T(TBuffer), IS_GODOT_STR_T(TGodotStr), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_str_t &p_val, integral_t &r_len) {
	auto utf8 = static_cast<String>(p_val).utf8();
	auto length = utf8.length();
	memcpy(p_buf, utf8.get_data(), length);
	p_buf += length;
	r_len += length;
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotStr, IS_BUFFER_T(TBuffer), IS_GODOT_STR_T(TGodotStr)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot_str_t &r_val) {
	int32_t len = 0;
	while (p_buf[len] != 0) {
		len++;
	}
	r_val = decltype_pure(r_val)(String::utf8(reinterpret_cast<char *>(p_buf), len));
	++len;
	p_buf += len;
}

#if !HAS_CXX20
template <typename TGodotVec, typename TInt, IS_GODOT_VEC_T(TGodotVec), IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot_vec_t &p_val, integral_t &r_len) {
	r_len += (sizeof(decltype_pure(p_val[0])) * decltype_pure(p_val)::AXIS_COUNT);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotVec, IS_BUFFER_T(TBuffer), IS_GODOT_VEC_T(TGodotVec)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_vec_t &p_val) {
	using vec_t = decltype_pure(p_val);
	if constexpr (little_endin) {
		memcpy(p_buf, &p_val, vec_t::AXIS_COUNT * sizeof(decltype_pure(p_val[0])));
		p_buf += vec_t::AXIS_COUNT * sizeof(decltype_pure(p_val[0]));
	} else {
		for (uint8_t i = 0; i < vec_t::AXIS_COUNT; ++i) {
			memcpy_reverse(p_buf, &p_val[i], sizeof(p_val[0]));
			p_buf += sizeof(p_val[0]);
		}
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotVec, typename TInt, IS_BUFFER_T(TBuffer), IS_GODOT_VEC_T(TGodotVec), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_vec_t &p_val, integral_t &r_len) {
	using vec_t = decltype_pure(p_val);
	if constexpr (little_endin) {
		memcpy(p_buf, &p_val, (vec_t::AXIS_COUNT * sizeof(decltype_pure(p_val[0]))));
		p_buf += vec_t::AXIS_COUNT * sizeof(decltype_pure(p_val[0]));
	} else {
		for (uint8_t i = 0; i < vec_t::AXIS_COUNT; ++i) {
			memcpy_reverse(p_buf, &p_val[i], sizeof(p_val[0]));
			p_buf += sizeof(p_val[0]);
		}
	}
	r_len += (decltype_pure(p_val)::AXIS_COUNT * sizeof(decltype_pure(p_val[0])));
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotVec, IS_BUFFER_T(TBuffer), IS_GODOT_VEC_T(TGodotVec)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot_vec_t &r_val) {
	using vec_t = decltype_pure(r_val);
	if constexpr (little_endin) {
		memcpy(&r_val, p_buf, vec_t::AXIS_COUNT * sizeof(decltype_pure(r_val[0])));
		p_buf += vec_t::AXIS_COUNT * sizeof(decltype_pure(r_val[0]));
	} else {
		for (uint8_t i = 0; i < vec_t::AXIS_COUNT; ++i) {
			memcpy_reverse(&r_val[i], p_buf, sizeof(r_val[0]));
			p_buf += sizeof(r_val[0]);
		}
	}
}

// (Quaternion, Color)
#if !HAS_CXX20
template <typename TGodotComponents4, typename TInt, IS_GODOT_COMPONENTS4_T(TGodotComponents4), IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot_components4_t &p_val, integral_t &r_len) {
	r_len += (sizeof(decltype_pure(p_val[0])) * 4);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotComponents4, IS_BUFFER_T(TBuffer), IS_GODOT_COMPONENTS4_T(TGodotComponents4)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_components4_t &p_val) {
	if constexpr (little_endin) {
		memcpy(p_buf, &p_val, (4 * sizeof(decltype_pure(p_val[0]))));
		p_buf += (4 * sizeof(decltype_pure(p_val[0])));
	} else {
		for (uint8_t i = 0; i < 4; ++i) {
			memcpy_reverse(p_buf, &p_val[i], sizeof(p_val[0]));
			p_buf += sizeof(p_val[0]);
		}
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotComponents4, typename TInt, IS_BUFFER_T(TBuffer), IS_GODOT_COMPONENTS4_T(TGodotComponents4), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_components4_t &p_val, integral_t &r_len) {
	if constexpr (little_endin) {
		memcpy(p_buf, &p_val, (4 * sizeof(decltype_pure(p_val[0]))));
		p_buf += (4 * sizeof(decltype_pure(p_val[0])));
	} else {
		for (uint8_t i = 0; i < 4; ++i) {
			memcpy_reverse(p_buf, &p_val[i], sizeof(p_val[0]));
			p_buf += sizeof(p_val[0]);
		}
	}
	r_len += (sizeof(decltype_pure(p_val[0])) * 4);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotComponents4, IS_BUFFER_T(TBuffer), IS_GODOT_COMPONENTS4_T(TGodotComponents4)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot_components4_t &r_val) {
	if constexpr (little_endin) {
		memcpy(&r_val, p_buf, (4 * sizeof(decltype_pure(r_val[0]))));
		p_buf += (4 * sizeof(decltype_pure(r_val[0])));
	} else {
		for (uint8_t i = 0; i < 4; ++i) {
			memcpy_reverse(&r_val[i], p_buf, sizeof(decltype(r_val[0])));
			p_buf += sizeof(decltype(r_val[0]));
		}
	}
}

// Rect2
#if !HAS_CXX20
template <typename TGodotRect2, typename TInt, IS_GODOT_RECT2_T(TGodotRect2), IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot_rect2_t &p_val, integral_t &r_len) {
	cal_size(p_val.position, r_len);
	cal_size(p_val.size, r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotRect2, IS_BUFFER_T(TBuffer), IS_GODOT_RECT2_T(TGodotRect2)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_rect2_t &p_val) {
	encode<little_endin>(p_buf, p_val.position);
	encode<little_endin>(p_buf, p_val.size);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotRect2, typename TInt, IS_BUFFER_T(TBuffer), IS_GODOT_RECT2_T(TGodotRect2), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_rect2_t &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val.position, r_len);
	encode<little_endin>(p_buf, p_val.size, r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotRect2, IS_BUFFER_T(TBuffer), IS_GODOT_RECT2_T(TGodotRect2)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot_rect2_t &r_val) {
	decode<little_endin>(p_buf, r_val.position);
	decode<little_endin>(p_buf, r_val.size);
}

// Transform2D
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const Transform2D &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Transform2D &p_val) {
	encode<little_endin>(p_buf, p_val[0]);
	encode<little_endin>(p_buf, p_val[1]);
	encode<little_endin>(p_buf, p_val[2]);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Transform2D &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val[0], r_len);
	encode<little_endin>(p_buf, p_val[1], r_len);
	encode<little_endin>(p_buf, p_val[2], r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, Transform2D &r_val) {
	decode<little_endin>(p_buf, r_val[0]);
	decode<little_endin>(p_buf, r_val[1]);
	decode<little_endin>(p_buf, r_val[2]);
}

// PLANE
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const Plane &p_val, integral_t &r_len) {
	cal_size(p_val.normal, r_len);
	cal_size(p_val.d, r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Plane &p_val) {
	encode<little_endin>(p_buf, p_val.normal);
	encode<little_endin>(p_buf, p_val.d);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Plane &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val.normal, r_len);
	encode<little_endin>(p_buf, p_val.d, r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, Plane &r_val) {
	decode<little_endin>(p_buf, r_val.normal);
	decode<little_endin>(p_buf, r_val.d);
}

// Projection
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const Projection &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
	cal_size(p_val[3], r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Projection &p_val) {
	encode<little_endin>(p_buf, p_val[0]);
	encode<little_endin>(p_buf, p_val[1]);
	encode<little_endin>(p_buf, p_val[2]);
	encode<little_endin>(p_buf, p_val[3]);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Projection &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val[0], r_len);
	encode<little_endin>(p_buf, p_val[1], r_len);
	encode<little_endin>(p_buf, p_val[2], r_len);
	encode<little_endin>(p_buf, p_val[3], r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, Projection &r_val) {
	decode<little_endin>(p_buf, r_val[0]);
	decode<little_endin>(p_buf, r_val[1]);
	decode<little_endin>(p_buf, r_val[2]);
	decode<little_endin>(p_buf, r_val[3]);
}

// AABB
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot::AABB &p_val, integral_t &r_len) {
	cal_size(p_val.position, r_len);
	cal_size(p_val.size, r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot::AABB &p_val) {
	encode<little_endin>(p_buf, p_val.position);
	encode<little_endin>(p_buf, p_val.size);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot::AABB &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val.position, r_len);
	encode<little_endin>(p_buf, p_val.size, r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot::AABB &r_val) {
	decode<little_endin>(p_buf, r_val.position);
	decode<little_endin>(p_buf, r_val.size);
}

// Basis
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const Basis &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Basis &p_val) {
	encode<little_endin>(p_buf, p_val[0]);
	encode<little_endin>(p_buf, p_val[1]);
	encode<little_endin>(p_buf, p_val[2]);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Basis &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val[0], r_len);
	encode<little_endin>(p_buf, p_val[1], r_len);
	encode<little_endin>(p_buf, p_val[2], r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, Basis &r_val) {
	decode<little_endin>(p_buf, r_val[0]);
	decode<little_endin>(p_buf, r_val[1]);
	decode<little_endin>(p_buf, r_val[2]);
}

// Transform3D
#if !HAS_CXX20
template <typename TInt, IS_INTEGRAL_T(TInt)>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const Transform3D &p_val, integral_t &r_len) {
	cal_size(p_val.basis, r_len);
	cal_size(p_val.origin, r_len);
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Transform3D &p_val) {
	encode<little_endin>(p_buf, p_val.basis);
	encode<little_endin>(p_buf, p_val.origin);
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const Transform3D &p_val, integral_t &r_len) {
	encode<little_endin>(p_buf, p_val.basis, r_len);
	encode<little_endin>(p_buf, p_val.origin, r_len);
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, Transform3D &r_val) {
	decode<little_endin>(p_buf, r_val.basis);
	decode<little_endin>(p_buf, r_val.origin);
}

// Color
#if !HAS_CXX20
template <uint8_t encode_type, typename TInt, IS_INTEGRAL_T(TInt)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else //!HAS_CXX20
template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void cal_size_color(const Color &p_val, integral_t &r_len) {
	if (encode_type == 1) { // Hex
		r_len += sizeof(decltype_pure(p_val.to_rgba32()));
	} else if (encode_type == 1) { // Hex64
		r_len += sizeof(decltype_pure(p_val.to_rgba64()));
	} else { // raw
		cal_size(p_val, r_len);
	}
}

#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, class TBuffer, IS_BUFFER_T(TBuffer)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void encode_color(buffer_t *&p_buf, const Color &p_val) {
	if (encode_type == 1) { // Hex
		encode_int<little_endin>(p_buf, p_val.to_rgba32());
	} else if (encode_type == 1) { // Hex64
		encode_int<little_endin>(p_buf, p_val.to_rgba64());
	} else { // raw
		encode<little_endin>(p_buf, p_val);
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, class TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void encode_color(buffer_t *&p_buf, const Color &p_val, integral_t &r_len) {
	if (encode_type == 1) { // Hex
		encode_int<little_endin>(p_buf, p_val.to_rgba32(), r_len);
	} else if (encode_type == 1) { // Hex64
		encode_int<little_endin>(p_buf, p_val.to_rgba64(), r_len);
	} else { // raw
		encode<little_endin>(p_buf, p_val, r_len);
	}
}
#endif

#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, class TBuffer, IS_BUFFER_T(TBuffer)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void decode_color(buffer_t *&p_buf, Color &r_val) {
	if (encode_type == 1) { // Hex
		uint32_t hex;
		decode_int<little_endin>(p_buf, hex);
		r_val = Color::hex(hex);
	} else if (encode_type == 1) { // Hex64
		uint64_t hex64;
		decode_int<little_endin>(p_buf, hex64);
		r_val = Color::hex64(hex64);
	} else { // raw
		decode<little_endin>(p_buf, r_val);
	}
}

// =================
// GodotFixedElementLenthArray
#if !HAS_CXX20
template <bool little_endin, typename TGodotFixedElementLengthArray, typename TInt, IS_GODOT_FIXED_ELEMENT_LENGTH_ARRAY_T(TGodotFixedElementLengthArray), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot_fixed_element_length_array_t &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint<little_endin>(size, r_len);
	using element_t = decltype_pure(p_val.ptr());
	r_len += sizeof(element_t) * size;
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotFixedElementLengthArray, IS_BUFFER_T(TBuffer), IS_GODOT_FIXED_ELEMENT_LENGTH_ARRAY_T(TGodotFixedElementLengthArray)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_fixed_element_length_array_t &p_val) {
	auto size = p_val.size();
	encode_varint<little_endin>(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	using element_t = decltype_pure(ptr);
	if constexpr (little_endin) {
		size_t increase_len = sizeof(element_t) * size;
		memcpy(p_buf, ptr, increase_len);
		p_buf += increase_len;
	} else {
		for (decltype(size) i = 0; i < size; ++i) {
			memcpy_reverse(p_buf, &ptr[i], sizeof(element_t));
			p_buf += sizeof(element_t);
		}
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotFixedElementLengthArray, typename TInt, IS_BUFFER_T(TBuffer), IS_GODOT_FIXED_ELEMENT_LENGTH_ARRAY_T(TGodotFixedElementLengthArray), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot_fixed_element_length_array_t &p_val, integral_t &r_len) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	using element_t = decltype_pure(ptr);
	size_t increase_len = sizeof(element_t) * size;
	if constexpr (little_endin) {
		memcpy(p_buf, ptr, increase_len);
		p_buf += increase_len;
	} else {
		for (decltype(size) i = 0; i < size; ++i) {
			memcpy_reverse(p_buf, &ptr[i], sizeof(element_t));
			p_buf += sizeof(element_t);
		}
	}
	r_len += increase_len;
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TGodotFixedElementLengthArray, IS_BUFFER_T(TBuffer), IS_GODOT_FIXED_ELEMENT_LENGTH_ARRAY_T(TGodotFixedElementLengthArray)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot_fixed_element_length_array_t &r_val) {
	decltype_pure(r_val.size()) size;
	decode_varint<little_endin>(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	using element_t = decltype_pure(ptr);
	if constexpr (little_endin) {
		size_t len = sizeof(element_t) * size;
		memcpy(ptr, p_buf, len);
		p_buf += len;
	} else {
		for (decltype(size) i = 0; i < size; ++i) {
			memcpy_reverse(&p_buf[i], p_buf, sizeof(element_t));
			p_buf += sizeof(element_t);
		}
	}
}
// =================

// Special:: godot::PackedStringArray
#if !HAS_CXX20
template <bool little_endin, typename TInt, IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void cal_size(const godot::PackedStringArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint<little_endin>(size, r_len);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	for (decltype_pure(size) i = 0; i < size; i++) {
		cal_size(p_val[i], r_len);
	}
}

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot::PackedStringArray &p_val) {
	auto size = p_val.size();
	encode_varint<little_endin>(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	for (decltype_pure(size) i = 0; i < size; i++) {
		encode<little_endin>(p_buf, p_val[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void encode(buffer_t *&p_buf, const godot::PackedStringArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint<little_endin>(p_buf, size, r_len);
	for (decltype_pure(size) i = 0; i < size; i++) {
		encode<little_endin>(p_buf, p_val[i], r_len);
	}
}
#endif

#if !HAS_CXX20
template <bool little_endin, typename TBuffer, IS_BUFFER_T(TBuffer)>
#else //!HAS_CXX20
template <bool little_endin>
#endif //!HAS_CXX20
_INLINE_ void decode(buffer_t *&p_buf, godot::PackedStringArray &r_val) {
	decltype_pure(r_val.size()) size;
	decode_varint<little_endin>(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	for (decltype_pure(size) i = 0; i < size; i++) {
		decode<little_endin>(p_buf, r_val[i]);
	}
}

// Special:: GodotPackedIntArray
#if !HAS_CXX20
template <bool little_endin, bool varint_encoding, typename TGodotPackedIntArray, typename TInt, IS_GODOT_PACKED_INT_ARRAY_T(TGodotPackedIntArray), IS_INTEGRAL_T(TInt)>
#else // //!HAS_CXX20
template <bool little_endin, bool varint_encoding> // If true, use zigzag to encode elements.
#endif //!HAS_CXX20
_INLINE_ void cal_size_int_arr(const godot_packed_int_array_t &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint<little_endin>(size, r_len);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	if constexpr (varint_encoding) {
		for (decltype_pure(size) i = 0; i < size; i++) {
			cal_size_varint<little_endin>(p_val[i], r_len);
		}
	} else {
		using int_t = decltype_pure(ptr);
		r_len += size * sizeof(int_t);
	}
}

#if !HAS_CXX20
template <bool little_endin, bool varint_encoding, typename TBuffer, typename TGodotPackedIntArray, IS_BUFFER_T(TBuffer), IS_GODOT_PACKED_INT_ARRAY_T(TGodotPackedIntArray)>
#else // //!HAS_CXX20
template <bool little_endin, bool varint_encoding> // If true, use zigzag to encode elements.
#endif //!HAS_CXX20
_INLINE_ void encode_int_arr(buffer_t *&p_buf, const godot_packed_int_array_t &p_val) {
	auto size = p_val.size();
	encode_varint<little_endin>(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	if constexpr (varint_encoding) {
		for (decltype_pure(size) i = 0; i < size; i++) {
			encode_varint<little_endin>(p_buf, p_val[i]);
		}
	} else {
		using int_t = decltype_pure(ptr);
		if constexpr (little_endin) {
			auto len = sizeof(int_t) * size;
			memcpy(p_buf, ptr, len);
			p_buf += len;
		} else {
			for (decltype(size) i = 0; i < size; ++i) {
				memcpy_reverse(p_buf, &ptr[i], sizeof(int_t));
				p_buf += sizeof(int_t);
			}
		}
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, bool varint_encoding, typename TBuffer, typename TGodotPackedIntArray, typename TInt, IS_BUFFER_T(TBuffer), IS_GODOT_PACKED_INT_ARRAY_T(TGodotPackedIntArray), IS_INTEGRAL_T(TInt)>
#else // //!HAS_CXX20
template <bool little_endin, bool varint_encoding> // If true, use zigzag to encode elements.
#endif //!HAS_CXX20
_INLINE_ void encode_int_arr(buffer_t *&p_buf, const godot_packed_int_array_t &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint<little_endin>(p_buf, size, r_len);
	if constexpr (varint_encoding) {
		for (decltype_pure(size) i = 0; i < size; i++) {
			encode_varint<little_endin>(p_buf, p_val[i], r_len);
		}
	} else {
		using int_t = decltype_pure(ptr);
		if constexpr (little_endin) {
			auto len = sizeof(int_t) * size;
			memcpy(p_buf, ptr, len);
			p_buf += len;
		} else {
			for (decltype(size) i = 0; i < size; ++i) {
				memcpy_reverse(p_buf, &ptr[i], sizeof(int_t));
				p_buf += sizeof(int_t);
			}
		}
		r_len += len;
	}
}
#endif

#if !HAS_CXX20
template <bool little_endin, bool varint_encoding, typename TBuffer, typename TGodotPackedIntArray, IS_BUFFER_T(TBuffer), IS_GODOT_PACKED_INT_ARRAY_T(TGodotPackedIntArray)>
#else // //!HAS_CXX20
template <bool little_endin, bool varint_encoding> // If true, use zigzag to encode elements.
#endif //!HAS_CXX20
_INLINE_ void decode_int_arr(buffer_t *&p_buf, godot_packed_int_array_t &r_val) {
	decltype_pure(r_val.size()) size;
	decode_varint<little_endin>(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	if constexpr (varint_encoding) {
		for (decltype_pure(size) i = 0; i < size; i++) {
			decode_varint<little_endin>(p_buf, r_val[i]);
		}
	} else {
		using int_t = decltype_pure(ptr);
		if constexpr (little_endin) {
			auto len = sizeof(int_t) * size;
			memcpy(ptr, p_buf, len);
			p_buf += len;
		} else {
			for (decltype(size) i = 0; i < size; ++i) {
				memcpy_reverse(&ptr[i], p_buf, sizeof(int_t));
				p_buf += sizeof(int_t);
			}
		}
	}
}

// Special: PackedColorArray
#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, typename TInt, IS_INTEGRAL_T(TInt)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else // //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void cal_size_color_arr(const PackedColorArray &p_val, integral_t &r_len) {
	if (encode_type == 0) {
		cal_size<little_endin>(p_val, r_len);
		return;
	}

	auto size = p_val.size();
	cal_size_varint<little_endin>(size, r_len);
	if (size == 0) {
		return;
	}

	if constexpr (encode_type == 1) {
		r_len = sizeof(decltype_pure(Color().to_rgba32())) * size;
	} else if constexpr (encode_type == 2) {
		r_len = sizeof(decltype_pure(Color().to_rgba64())) * size;
	}
}

#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, typename TBuffer, IS_BUFFER_T(TBuffer)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else // //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void encode_color_arr(buffer_t *&p_buf, const PackedColorArray &p_val) {
	if (encode_type == 0) {
		encode<little_endin>(p_buf, p_val);
		return;
	}

	auto size = p_val.size();
	encode_varint<little_endin>(p_buf, size);
	if (size == 0) {
		return;
	}

	auto ptr = p_val.ptr();
	for (decltype_pure(size) i = 0; i < size; i++) {
		encode_color<little_endin, encode_type>(p_buf, ptr[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, typename TBuffer, typename TInt, IS_BUFFER_T(TBuffer), IS_INTEGRAL_T(TInt)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else // //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void encode_color_arr(buffer_t *&p_buf, const PackedColorArray &p_val, integral_t &r_len) {
	if (encode_type == 0) {
		encode<little_endin>(p_buf, p_val, r_len);
		return;
	}

	auto size = p_val.size();
	encode_varint<little_endin>(p_buf, size, r_len);
	if (size == 0) {
		return;
	}

	auto ptr = p_val.ptr();
	for (decltype_pure(size) i = 0; i < size; i++) {
		encode_color<little_endin, encode_type, decltype_pure(p_buf)>(p_buf, ptr[i], r_len);
	}
}
#endif

#if !HAS_CXX20
template <bool little_endin, uint8_t encode_type, typename TBuffer, IS_BUFFER_T(TBuffer)> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#else // //!HAS_CXX20
template <bool little_endin, uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
#endif //!HAS_CXX20
_INLINE_ void decode_color_arr(buffer_t *&p_buf, PackedColorArray &r_val) {
	if (encode_type == 0) {
		decode<little_endin>(p_buf, r_val);
		return;
	}

	decltype_pure(r_val.size()) size;
	decode_varint<little_endin>(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	for (decltype_pure(size) i = 0; i < size; i++) {
		decode_color<little_endin, encode_type>(p_buf, ptr[i]);
	}
}

} //namespace dserializer
