#pragma once

#include "dserializer.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace dserializer {
// godot Str
template <typename T>
concept GodotStr = !Number<T> && (std::same_as<T, String> || std::same_as<T, StringName> || std::same_as<T, NodePath>);
// GodotComponents4( Quaternion, Color)
template <typename T>
concept GodotComponents4 = !Number<T> && (std::same_as<T, Quaternion> || std::same_as<T, Color>);
template <typename T>
concept GodotVec = !Number<T> && (std::same_as<T, Vector2> || std::same_as<T, Vector2i> || std::same_as<T, Vector3> || std::same_as<T, Vector3i> || std::same_as<T, Vector4> || std::same_as<T, Vector4i>);
// Rect2
template <typename T>
concept GodotRect2 = !Number<T> && (std::same_as<T, Rect2> || std::same_as<T, Rect2i>);
// PackedXxxArray
template <typename T>
concept GodotPackedIntArray = !Number<T> && (std::same_as<T, PackedInt32Array> || std::same_as<T, PackedInt64Array>);
template <typename T>
concept GodotPackedNormalArray = !Number<T> && (std::same_as<T, PackedByteArray> || std::same_as<T, PackedFloat32Array> || std::same_as<T, PackedFloat64Array>);
template <typename T>
concept GodotFixedElementLenthArray = !Number<T> && (GodotPackedNormalArray<T> || std::same_as<T, PackedVector2Array> || std::same_as<T, PackedVector3Array> || std::same_as<T, PackedColorArray>);

// GodotStr
_INLINE_ void cal_size(const GodotStr auto &p_val, integral_t &r_len) {
	r_len += godot::String(p_val).utf8().length();
	++r_len;
}
_INLINE_ void encode(buffer_t * p_buf, const GodotStr auto &p_val) {
	auto utf8 = String(p_val).utf8();
	auto length = utf8.length();
	memcpy(p_buf, utf8.get_data(), length);
#ifdef _DEBUG_
	if (!p_buf[lenth - 1] == 0) {
		throw "ERR utf8";
	}
#endif
	p_buf += length;
	*p_buf = 0;
	++p_buf;
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const GodotStr auto &p_val, integral_t &r_len) {
	auto utf8 = static_cast<String>(p_val).utf8();
	auto length = utf8.length();
	memcpy(p_buf, utf8.get_data(), length);
#ifdef _DEBUG_
	if (!p_buf[lenth - 1] == 0) {
		throw "ERR utf8";
	}
#endif
	p_buf += length;
	r_len += length;
}
#endif
template <GodotStr T>
_INLINE_ void decode(buffer_t * p_buf, T &r_val) {
	int32_t len = 0;
	while (p_buf[len] != 0) {
		len++;
	}
	r_val = T(String::utf8(reinterpret_cast<char *>(p_buf), len));
	++len;
	p_buf += len;
}
// ����
template <GodotVec T>
_INLINE_ void cal_size(const T &p_val, integral_t &r_len) {
	r_len += (sizeof(decltype(p_val[0])) * T::AXIS_COUNT);
}
template <GodotVec T>
_INLINE_ void encode(buffer_t * p_buf, const T &p_val) {
	memcpy(p_buf, &p_val, (T::AXIS_COUNT * sizeof(decltype(p_val[0]))));
	p_buf += (T::AXIS_COUNT * sizeof(decltype(p_val[0])));
}
#ifdef ENCODE_LEN_METHOD
template <uint8_t dimension>
_INLINE_ void encode(buffer_t * p_buf, const GodotVec auto &p_val, integral_t &r_len) {
	memcpy(p_buf, &p_val, (T::AXIS_COUNT * sizeof(decltype(p_val[0]))));
	p_buf += (T::AXIS_COUNT * sizeof(decltype(p_val[0])));
	r_len += (sizeof(decltype(p_val[0])) * T::AXIS_COUNT);
}
#endif
template <GodotVec T>
_INLINE_ void decode(buffer_t * p_buf, T &r_val) {
	memcpy(&r_val, p_buf, (T::AXIS_COUNT * sizeof(decltype(r_val[0]))));
	p_buf += (T::AXIS_COUNT * sizeof(decltype(r_val[0])));
}

// (Quaternion, Color)
_INLINE_ void cal_size(const GodotComponents4 auto &p_val, integral_t &r_len) {
	r_len += (sizeof(decltype(p_val[0])) * 4);
}
_INLINE_ void encode(buffer_t * p_buf, const GodotComponents4 auto &p_val) {
	memcpy(p_buf, &p_val, (4 * sizeof(decltype(p_val[0]))));
	p_buf += (4 * sizeof(decltype(p_val[0])));
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const GodotComponents4 auto &p_val, integral_t &r_len) {
	memcpy(p_buf, &p_val, (4 * sizeof(decltype(p_val[0]))));
	p_buf += (4 * sizeof(decltype(p_val[0])));
	r_len += (sizeof(decltype(p_val[0])) * 4);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, GodotComponents4 auto &r_val) {
	memcpy(&r_val, p_buf, (4 * sizeof(decltype(r_val[0]))));
	p_buf += (4 * sizeof(decltype(r_val[0])));
}
// Rect2
_INLINE_ void cal_size(const GodotRect2 auto &p_val, integral_t &r_len) {
	cal_size(p_val.position, r_len);
	cal_size(p_val.size, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const GodotRect2 auto &p_val) {
	encode(p_buf, p_val.position);
	encode(p_buf, p_val.size);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const GodotRect2 auto &p_val, integral_t &r_len) {
	encode(p_buf, p_val.position, r_len);
	encode(p_buf, p_val.size, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, GodotRect2 auto &r_val) {
	decode(p_buf, r_val.position);
	decode(p_buf, r_val.size);
}
// Transform2D
_INLINE_ void cal_size(const Transform2D &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const Transform2D &p_val) {
	encode(p_buf, p_val[0]);
	encode(p_buf, p_val[1]);
	encode(p_buf, p_val[2]);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const Transform2D &p_val, integral_t &r_len) {
	encode(p_buf, p_val[0], r_len);
	encode(p_buf, p_val[1], r_len);
	encode(p_buf, p_val[2], r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, Transform2D &r_val) {
	decode(p_buf, r_val[0]);
	decode(p_buf, r_val[1]);
	decode(p_buf, r_val[2]);
}
// PLANE
_INLINE_ void cal_size(const Plane &p_val, integral_t &r_len) {
	cal_size(p_val.normal, r_len);
	cal_size(p_val.d, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const Plane &p_val) {
	encode(p_buf, p_val.normal);
	encode(p_buf, p_val.d);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const Plane &p_val, integral_t &r_len) {
	encode(p_buf, p_val.normal, r_len);
	encode(p_buf, p_val.d, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, Plane &r_val) {
	decode(p_buf, r_val.normal);
	decode(p_buf, r_val.d);
}
// Projection
_INLINE_ void cal_size(const Projection &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
	cal_size(p_val[3], r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const Projection &p_val) {
	encode(p_buf, p_val[0]);
	encode(p_buf, p_val[1]);
	encode(p_buf, p_val[2]);
	encode(p_buf, p_val[3]);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const Projection &p_val, integral_t &r_len) {
	encode(p_buf, p_val[0], r_len);
	encode(p_buf, p_val[1], r_len);
	encode(p_buf, p_val[2], r_len);
	encode(p_buf, p_val[3], r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, Projection &r_val) {
	decode(p_buf, r_val[0]);
	decode(p_buf, r_val[1]);
	decode(p_buf, r_val[2]);
	decode(p_buf, r_val[3]);
}

// AABB
_INLINE_ void cal_size(const godot::AABB &p_val, integral_t &r_len) {
	cal_size(p_val.position, r_len);
	cal_size(p_val.size, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const godot::AABB &p_val) {
	encode(p_buf, p_val.position);
	encode(p_buf, p_val.size);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const godot::AABB &p_val, integral_t &r_len) {
	encode(p_buf, p_val.position, r_len);
	encode(p_buf, p_val.size, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, godot::AABB &r_val) {
	decode(p_buf, r_val.position);
	decode(p_buf, r_val.size);
}
// Basis
_INLINE_ void cal_size(const Basis &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const Basis &p_val) {
	encode(p_buf, p_val[0]);
	encode(p_buf, p_val[1]);
	encode(p_buf, p_val[2]);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const Basis &p_val, integral_t &r_len) {
	encode(p_buf, p_val[0], r_len);
	encode(p_buf, p_val[1], r_len);
	encode(p_buf, p_val[2], r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, Basis &r_val) {
	decode(p_buf, r_val[0]);
	decode(p_buf, r_val[1]);
	decode(p_buf, r_val[2]);
}
// Transform3D
_INLINE_ void cal_size(const Transform3D &p_val, integral_t &r_len) {
	cal_size(p_val.basis, r_len);
	cal_size(p_val.origin, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const Transform3D &p_val) {
	encode(p_buf, p_val.basis);
	encode(p_buf, p_val.origin);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const Transform3D &p_val, integral_t &r_len) {
	encode(p_buf, p_val.basis, r_len);
	encode(p_buf, p_val.origin, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, Transform3D &r_val) {
	decode(p_buf, r_val.basis);
	decode(p_buf, r_val.origin);
}
// Color
_INLINE_ void cal_size_color_raw(const Color &p_val, integral_t &r_len) {
	cal_size(p_val, r_len);
}
_INLINE_ void encode_color_raw(buffer_t * p_buf, const Color &p_val) {
	encode(p_buf, p_val);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_color_raw(buffer_t * p_buf, const Color &p_val, integral_t &r_len) {
	encode(p_buf, p_val, r_len);
}
#endif
_INLINE_ void decode_color_raw(buffer_t * p_buf, Color &r_val) {
	decode(p_buf, r_val);
}

_INLINE_ void cal_size_color_hex(const Color &p_val, integral_t &r_len) {
	r_len += sizeof(decltype(p_val.to_rgba32()));
}
_INLINE_ void encode_color_hex(buffer_t * p_buf, const Color &p_val) {
	encode_int(p_buf, p_val.to_rgba32());
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_color_hex(buffer_t * p_buf, const Color &p_val, integral_t &r_len) {
	encode(p_buf, p_val.to_argb32(), r_len);
}
#endif

_INLINE_ void cal_size_color_hex64(const Color &p_val, integral_t &r_len) {
	r_len += sizeof(decltype(p_val.to_rgba64()));
}
_INLINE_ void encode_color_hex64(buffer_t * p_buf, const Color &p_val) {
	encode_int(p_buf, p_val.to_rgba64());
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_color_hex64(buffer_t * p_buf, const Color &p_val, integral_t &r_len) {
	encode(p_buf, p_val.to_argb64(), r_len);
}
#endif

_INLINE_ void decode_color_hex(buffer_t * p_buf, Color &r_val) {
	uint32_t hex;
	decode_int(p_buf, hex);
	r_val = Color::hex(hex);
}
_INLINE_ void decode_color_hex64(buffer_t * p_buf, Color &r_val) {
	uint64_t hex64;
	decode_int(p_buf, hex64);
	r_val = Color::hex64(hex64);
}

// =================
// GodotFixedElementLenthArray �ػ�
template <typename TElementConponent, uint8_t element_conponent_count>
_INLINE_ void cal_size(const GodotFixedElementLenthArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint(size, r_len);
	r_len += (element_conponent_count * sizeof(TElementConponent)) * size;
}
template <typename TElementConponent, uint8_t element_conponent_count>
_INLINE_ void encode(buffer_t * p_buf, const GodotFixedElementLenthArray auto &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	size_t increase_len = (element_conponent_count * sizeof(TElementConponent)) * size;
	memcpy(p_buf, ptr, increase_len);
	p_buf += increase_len;
}
#ifdef ENCODE_LEN_METHOD
template <uint8_t element_conponent_count>
_INLINE_ void encode(buffer_t * p_buf, const GodotFixedElementLenthArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	size_t increase_len = element_conponent_count * sizeof(TElementConponent) * size;
	memcpy(p_buf, ptr, increase_len);
	p_buf += increase_len;
	r_len += increase_len;
}
#endif
template <typename TElementConponent, uint8_t element_conponent_count>
_INLINE_ void decode(buffer_t * p_buf, GodotFixedElementLenthArray auto &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0)
		return;
	auto ptr = r_val.ptrw();
	size_t len = (element_conponent_count * sizeof(TElementConponent)) * size;
	memcpy(ptr, p_buf, len);
	p_buf += len;
}
// =================

// godot::PackedStringArray
_INLINE_ void cal_size(const godot::PackedStringArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint(size, r_len);
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		cal_size(p_val[i], r_len);
	}
}
_INLINE_ void encode(buffer_t * p_buf, const godot::PackedStringArray &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, p_val[i]);
	}
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const godot::PackedStringArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, p_val[i], r_len);
	}
}
#endif
_INLINE_ void decode(buffer_t * p_buf, godot::PackedStringArray &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0)
		return;
	auto ptr = r_val.ptrw();
	for (decltype(size) i = 0; i < size; i++) {
		decode(p_buf, r_val[i]);
	}
}
// GodotPackedNormalArray
_INLINE_ void cal_size(const GodotPackedNormalArray auto &p_val, integral_t &r_len) {
	cal_size<decltype(*(p_val.ptr())), 1>(p_val, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const GodotPackedNormalArray auto &p_val) {
	encode<decltype(*(p_val.ptr())), 1>(p_buf, p_val);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const GodotPackedNormalArray auto &p_val, integral_t &r_len) {
	encode<decltype(*(p_val.ptr())), 1>(p_val, r_len, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, GodotPackedNormalArray auto &r_val) {
	decode<decltype(*(r_val.ptr())), 1>(p_buf, r_val);
}
// GodotPackedIntArray �ػ�
_INLINE_ void cal_size(const GodotPackedIntArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint(size, r_len);
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		cal_size_varint(p_val[i], r_len);
	}
}
_INLINE_ void encode(buffer_t * p_buf, const GodotPackedIntArray auto &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		encode_varint(p_buf, p_val[i]);
	}
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const GodotPackedIntArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		encode_varint(p_buf, p_val[i], r_len);
	}
}
#endif
_INLINE_ void decode(buffer_t * p_buf, GodotPackedIntArray auto &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0)
		return;
	auto ptr = r_val.ptrw();
	for (decltype(size) i = 0; i < size; i++) {
		decode_varint(p_buf, r_val[i]);
	}
}
// PackedVector2Array �ػ�
_INLINE_ void cal_size(const godot::PackedVector2Array &p_val, integral_t &r_len) {
	cal_size<decltype(Vector2::x), 2>(p_val, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const godot::PackedVector2Array &p_val) {
	encode<decltype(Vector2::x), 2>(p_buf, p_val);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const godot::PackedVector2Array &p_val, integral_t &r_len) {
	encode<decltype(Vector2::x), 2>(p_buf, p_val, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, godot::PackedVector2Array &r_val) {
	decode<decltype(Vector2::x), 2>(p_buf, r_val);
}
// PackedVector3Array �ػ�
_INLINE_ void cal_size(const godot::PackedVector3Array &p_val, integral_t &r_len) {
	cal_size<decltype(Vector3::x), 3>(p_val, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const godot::PackedVector3Array &p_val) {
	encode<decltype(Vector3::x), 3>(p_buf, p_val);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const godot::PackedVector3Array &p_val, integral_t &r_len) {
	encode<decltype(Vector3::x), 3>(p_buf, p_val, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, godot::PackedVector3Array &r_val) {
	decode<decltype(Vector3::x), 3>(p_buf, r_val);
}

// PackedColorArray �ػ�
_INLINE_ void cal_size(const PackedColorArray &p_val, integral_t &r_len) {
	cal_size<decltype(Color::r), 4>(p_val, r_len);
}
_INLINE_ void encode(buffer_t * p_buf, const PackedColorArray &p_val) {
	encode<decltype(Color::r), 4>(p_buf, p_val);
}
_INLINE_ void cal_size_color_arr_raw(const PackedColorArray &p_val, integral_t &r_len) {
	cal_size(p_val, r_len);
}
_INLINE_ void encode_color_arr_raw(buffer_t * p_buf, const PackedColorArray &p_val) {
	encode(p_buf, p_val);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t * p_buf, const PackedColorArray &p_val, integral_t &r_len) {
	encode<decltype(Color::r), 4>(p_buf, p_val, r_len);
}
_INLINE_ void encode_color_arr_raw(buffer_t * p_buf, const PackedColorArray &p_val, integral_t &r_len) {
	encode(p_buf, p_val, r_len);
}
#endif
_INLINE_ void decode(buffer_t * p_buf, PackedColorArray &r_val) {
	decode<decltype(Color::r), 4>(p_buf, r_val);
}
_INLINE_ void decode_color_arr_raw(buffer_t * p_buf, PackedColorArray &r_val) {
	decode(p_buf, r_val);
}
_INLINE_ void cal_size_color_arr_hex(const PackedColorArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	if (size == 0)
		return;
	cal_size_varint(size, r_len);
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		cal_size_color_hex(p_val[i], r_len);
	}
}
_INLINE_ void encode_color_arr_hex(buffer_t * p_buf, const PackedColorArray &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		encode_color_hex(p_buf, p_val[i]);
	}
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_color_arr_hex(buffer_t * p_buf, const PackedColorArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		encode_color_hex(p_buf, p_val[i], r_len);
	}
}
#endif
_INLINE_ void decode_color_arr_hex(buffer_t * p_buf, PackedColorArray &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0)
		return;
	auto ptr = r_val.ptrw();
	for (decltype(size) i = 0; i < size; i++) {
		decode_color_hex(p_buf, r_val[i]);
	}
}

_INLINE_ void cal_size_color_arr_hex64(const PackedColorArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	if (size == 0)
		return;
	cal_size_varint(size, r_len);
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		cal_size_color_hex64(p_val[i], r_len);
	}
}
_INLINE_ void encode_color_arr_hex64(buffer_t * p_buf, const PackedColorArray &p_val) {
	auto size = p_val.size();
	if (size == 0)
		return;
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size);
	for (decltype(size) i = 0; i < size; i++) {
		encode_color_hex64(p_buf, p_val[i]);
	}
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_color_arr_hex64(buffer_t * p_buf, const PackedColorArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		encode_color_hex64(p_buf, p_val[i], r_len);
	}
}
#endif
_INLINE_ void decode_color_arr_hex64(buffer_t * p_buf, PackedColorArray &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0)
		return;
	auto ptr = r_val.ptrw();
	for (decltype(size) i = 0; i < size; i++) {
		decode_color_hex64(p_buf, r_val[i]);
	}
}
} //namespace dserializer
