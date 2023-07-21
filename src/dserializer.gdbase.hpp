#pragma once

#include "dserializer.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace dserializer {
// Concepts
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
concept GodotFixedElementLenthArray = !Number<T> &&
		(GodotPackedIntArray<T> ||
				std::same_as<T, PackedByteArray> || std::same_as<T, PackedFloat32Array> ||
				std::same_as<T, PackedFloat64Array> || std::same_as<T, PackedVector2Array> ||
				std::same_as<T, PackedVector3Array> || std::same_as<T, PackedColorArray>);

// GodotStr
_INLINE_ void cal_size(const GodotStr auto &p_val, integral_t &r_len) {
	r_len += godot::String(p_val).utf8().length();
	++r_len;
}
_INLINE_ void encode(buffer_t *p_buf, const GodotStr auto &p_val) {
	auto utf8 = String(p_val).utf8();
	auto length = utf8.length();
	memcpy(p_buf, utf8.get_data(), length);
#ifdef DEBUG_ENABLED
	if (!p_buf[lenth - 1] == 0) {
		throw "ERR utf8";
	}
#endif // DEBUG_ENABLED
	p_buf += length;
	*p_buf = 0;
	++p_buf;
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const GodotStr auto &p_val, integral_t &r_len) {
	auto utf8 = static_cast<String>(p_val).utf8();
	auto length = utf8.length();
	memcpy(p_buf, utf8.get_data(), length);
#ifdef DEBUG_ENABLED
	if (!p_buf[lenth - 1] == 0) {
		throw "ERR utf8";
	}
#endif // DEBUG_ENABLED
	p_buf += length;
	r_len += length;
}
#endif

template <GodotStr T>
_INLINE_ void decode(buffer_t *p_buf, T &r_val) {
	int32_t len = 0;
	while (p_buf[len] != 0) {
		len++;
	}
	r_val = T(String::utf8(reinterpret_cast<char *>(p_buf), len));
	++len;
	p_buf += len;
}

//
template <GodotVec T>
_INLINE_ void cal_size(const T &p_val, integral_t &r_len) {
	r_len += (sizeof(decltype(p_val[0])) * T::AXIS_COUNT);
}
template <GodotVec T>
_INLINE_ void encode(buffer_t *p_buf, const T &p_val) {
	memcpy(p_buf, &p_val, (T::AXIS_COUNT * sizeof(decltype(p_val[0]))));
	p_buf += (T::AXIS_COUNT * sizeof(decltype(p_val[0])));
}
#ifdef ENCODE_LEN_METHOD
template <uint8_t dimension>
_INLINE_ void encode(buffer_t *p_buf, const GodotVec auto &p_val, integral_t &r_len) {
	memcpy(p_buf, &p_val, (T::AXIS_COUNT * sizeof(decltype(p_val[0]))));
	p_buf += (T::AXIS_COUNT * sizeof(decltype(p_val[0])));
	r_len += (sizeof(decltype(p_val[0])) * T::AXIS_COUNT);
}
#endif
template <GodotVec T>
_INLINE_ void decode(buffer_t *p_buf, T &r_val) {
	memcpy(&r_val, p_buf, (T::AXIS_COUNT * sizeof(decltype(r_val[0]))));
	p_buf += (T::AXIS_COUNT * sizeof(decltype(r_val[0])));
}

// (Quaternion, Color)
_INLINE_ void cal_size(const GodotComponents4 auto &p_val, integral_t &r_len) {
	r_len += (sizeof(decltype(p_val[0])) * 4);
}
_INLINE_ void encode(buffer_t *p_buf, const GodotComponents4 auto &p_val) {
	memcpy(p_buf, &p_val, (4 * sizeof(decltype(p_val[0]))));
	p_buf += (4 * sizeof(decltype(p_val[0])));
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const GodotComponents4 auto &p_val, integral_t &r_len) {
	memcpy(p_buf, &p_val, (4 * sizeof(decltype(p_val[0]))));
	p_buf += (4 * sizeof(decltype(p_val[0])));
	r_len += (sizeof(decltype(p_val[0])) * 4);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, GodotComponents4 auto &r_val) {
	memcpy(&r_val, p_buf, (4 * sizeof(decltype(r_val[0]))));
	p_buf += (4 * sizeof(decltype(r_val[0])));
}
// Rect2
_INLINE_ void cal_size(const GodotRect2 auto &p_val, integral_t &r_len) {
	cal_size(p_val.position, r_len);
	cal_size(p_val.size, r_len);
}
_INLINE_ void encode(buffer_t *p_buf, const GodotRect2 auto &p_val) {
	encode(p_buf, p_val.position);
	encode(p_buf, p_val.size);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const GodotRect2 auto &p_val, integral_t &r_len) {
	encode(p_buf, p_val.position, r_len);
	encode(p_buf, p_val.size, r_len);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, GodotRect2 auto &r_val) {
	decode(p_buf, r_val.position);
	decode(p_buf, r_val.size);
}
// Transform2D
_INLINE_ void cal_size(const Transform2D &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
}
_INLINE_ void encode(buffer_t *p_buf, const Transform2D &p_val) {
	encode(p_buf, p_val[0]);
	encode(p_buf, p_val[1]);
	encode(p_buf, p_val[2]);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Transform2D &p_val, integral_t &r_len) {
	encode(p_buf, p_val[0], r_len);
	encode(p_buf, p_val[1], r_len);
	encode(p_buf, p_val[2], r_len);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, Transform2D &r_val) {
	decode(p_buf, r_val[0]);
	decode(p_buf, r_val[1]);
	decode(p_buf, r_val[2]);
}
// PLANE
_INLINE_ void cal_size(const Plane &p_val, integral_t &r_len) {
	cal_size(p_val.normal, r_len);
	cal_size(p_val.d, r_len);
}
_INLINE_ void encode(buffer_t *p_buf, const Plane &p_val) {
	encode(p_buf, p_val.normal);
	encode(p_buf, p_val.d);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Plane &p_val, integral_t &r_len) {
	encode(p_buf, p_val.normal, r_len);
	encode(p_buf, p_val.d, r_len);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, Plane &r_val) {
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
_INLINE_ void encode(buffer_t *p_buf, const Projection &p_val) {
	encode(p_buf, p_val[0]);
	encode(p_buf, p_val[1]);
	encode(p_buf, p_val[2]);
	encode(p_buf, p_val[3]);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Projection &p_val, integral_t &r_len) {
	encode(p_buf, p_val[0], r_len);
	encode(p_buf, p_val[1], r_len);
	encode(p_buf, p_val[2], r_len);
	encode(p_buf, p_val[3], r_len);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, Projection &r_val) {
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
_INLINE_ void encode(buffer_t *p_buf, const godot::AABB &p_val) {
	encode(p_buf, p_val.position);
	encode(p_buf, p_val.size);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const godot::AABB &p_val, integral_t &r_len) {
	encode(p_buf, p_val.position, r_len);
	encode(p_buf, p_val.size, r_len);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, godot::AABB &r_val) {
	decode(p_buf, r_val.position);
	decode(p_buf, r_val.size);
}
// Basis
_INLINE_ void cal_size(const Basis &p_val, integral_t &r_len) {
	cal_size(p_val[0], r_len);
	cal_size(p_val[1], r_len);
	cal_size(p_val[2], r_len);
}
_INLINE_ void encode(buffer_t *p_buf, const Basis &p_val) {
	encode(p_buf, p_val[0]);
	encode(p_buf, p_val[1]);
	encode(p_buf, p_val[2]);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Basis &p_val, integral_t &r_len) {
	encode(p_buf, p_val[0], r_len);
	encode(p_buf, p_val[1], r_len);
	encode(p_buf, p_val[2], r_len);
}
#endif
_INLINE_ void decode(buffer_t *p_buf, Basis &r_val) {
	decode(p_buf, r_val[0]);
	decode(p_buf, r_val[1]);
	decode(p_buf, r_val[2]);
}
// Transform3D
_INLINE_ void cal_size(const Transform3D &p_val, integral_t &r_len) {
	cal_size(p_val.basis, r_len);
	cal_size(p_val.origin, r_len);
}
_INLINE_ void encode(buffer_t *p_buf, const Transform3D &p_val) {
	encode(p_buf, p_val.basis);
	encode(p_buf, p_val.origin);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Transform3D &p_val, integral_t &r_len) {
	encode(p_buf, p_val.basis, r_len);
	encode(p_buf, p_val.origin, r_len);
}
#endif

_INLINE_ void decode(buffer_t *p_buf, Transform3D &r_val) {
	decode(p_buf, r_val.basis);
	decode(p_buf, r_val.origin);
}

// Color
template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void cal_size_color(const Color &p_val, integral_t &r_len) {
	if (encode_type == 1) { // Hex
		r_len += sizeof(decltype(p_val.to_rgba32()));
	} else if (encode_type == 1) { // Hex64
		r_len += sizeof(decltype(p_val.to_rgba64()));
	} else { // raw
		cal_size(p_val, r_len);
	}
}

template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void encode_color(buffer_t *p_buf, const Color &p_val) {
	if (encode_type == 1) { // Hex
		encode_int(p_buf, p_val.to_rgba32());
	} else if (encode_type == 1) { // Hex64
		encode_int(p_buf, p_val.to_rgba64());
	} else { // raw
		encode(p_buf, p_val);
	}
}

#ifdef ENCODE_LEN_METHOD
template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void encode_color(buffer_t *p_buf, const Color &p_val, integral_t &r_len) {
	if (encode_type == 1) { // Hex
		encode_int(p_buf, p_val.to_rgba32(), r_len);
	} else if (encode_type == 1) { // Hex64
		encode_int(p_buf, p_val.to_rgba64(), r_len);
	} else { // raw
		encode(p_buf, p_val, r_len);
	}
}
#endif

template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void decode_color(buffer_t *p_buf, Color &r_val) {
	if (encode_type == 1) { // Hex
		uint32_t hex;
		decode_int(p_buf, hex);
		r_val = Color::hex(hex);
	} else if (encode_type == 1) { // Hex64
		uint64_t hex64;
		decode_int(p_buf, hex64);
		r_val = Color::hex64(hex64);
	} else { // raw
		decode(p_buf, r_val);
	}
}

// =================
// GodotFixedElementLenthArray
_INLINE_ void cal_size(const GodotFixedElementLenthArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint(size, r_len);
	using element_t = std::remove_const_t<std::remove_pointer_t<decltype(p_val.ptr())>>;
	r_len += sizeof(element_t) * size;
}

_INLINE_ void encode(buffer_t *p_buf, const GodotFixedElementLenthArray auto &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	using element_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
	size_t increase_len = sizeof(element_t) * size;
	memcpy(p_buf, ptr, increase_len);
	p_buf += increase_len;
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const GodotFixedElementLenthArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	using element_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
	size_t increase_len = sizeof(element_t) * size;
	memcpy(p_buf, ptr, increase_len);
	p_buf += increase_len;
	r_len += increase_len;
}
#endif

_INLINE_ void decode(buffer_t *p_buf, GodotFixedElementLenthArray auto &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	using element_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
	size_t len = sizeof(element_t) * size;
	memcpy(ptr, p_buf, len);
	p_buf += len;
}
// =================

// Special:: godot::PackedStringArray
_INLINE_ void cal_size(const godot::PackedStringArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint(size, r_len);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		cal_size(p_val[i], r_len);
	}
}

_INLINE_ void encode(buffer_t *p_buf, const godot::PackedStringArray &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, p_val[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const godot::PackedStringArray &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, p_val[i], r_len);
	}
}
#endif
_INLINE_ void decode(buffer_t *p_buf, godot::PackedStringArray &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	for (decltype(size) i = 0; i < size; i++) {
		decode(p_buf, r_val[i]);
	}
}

// Special:: GodotPackedIntArray
template <bool varint_encoding> // If true, use zigzag to encode elements.
_INLINE_ void cal_size_int_arr(const GodotPackedIntArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	cal_size_varint(size, r_len);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	if constexpr (varint_encoding) {
		for (decltype(size) i = 0; i < size; i++) {
			cal_size_varint(p_val[i], r_len);
		}
	} else {
		using int_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
		r_len += size * sizeof(int_t);
	}
}

template <bool varint_encoding> // If true, use zigzag to encode elements.
_INLINE_ void encode_int_arr(buffer_t *p_buf, const GodotPackedIntArray auto &p_val) {
	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0) {
		return;
	}
	auto ptr = p_val.ptr();
	if constexpr (varint_encoding) {
		for (decltype(size) i = 0; i < size; i++) {
			encode_varint(p_buf, p_val[i]);
		}
	} else {
		using int_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
		memcpy(p_buf, ptr, sizeof(int_t) * size);
	}
}

#ifdef ENCODE_LEN_METHOD
template <bool varint_encoding> // If true, use zigzag to encode elements.
_INLINE_ void encode_int_arr(buffer_t *p_buf, const GodotPackedIntArray auto &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto ptr = p_val.ptr();
	encode_varint(p_buf, size, r_len);
	if constexpr (varint_encoding) {
		for (decltype(size) i = 0; i < size; i++) {
			encode_varint(p_buf, p_val[i], r_len);
		}
	} else {
		using int_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
		auto len = sizeof(int_t) * size;
		memcpy(p_buf, ptr, len);
		r_len += len;
	}
}
#endif

template <bool varint_encoding>
_INLINE_ void decode_int_arr(buffer_t *p_buf, GodotPackedIntArray auto &r_val) {
	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	if constexpr (varint_encoding) {
		for (decltype(size) i = 0; i < size; i++) {
			decode_varint(p_buf, r_val[i]);
		}
	} else {
		using int_t = std::remove_const_t<std::remove_pointer_t<decltype(ptr)>>;
		auto len = sizeof(int_t) * size;
		memcpy(ptr, p_buf, len);
		p_buf += len;
	}
}

// Special: PackedColorArray
template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void cal_size_color_arr(const PackedColorArray &p_val, integral_t &r_len) {
	if (encode_type == 0) {
		cal_size(p_val, r_len);
		return;
	}

	auto size = p_val.size();
	cal_size_varint(size, r_len);
	if (size == 0) {
		return;
	}

	if constexpr (encode_type == 1) {
		r_len = sizeof(decltype(Color().to_rgba32())) * size;
	} else if constexpr (encode_type == 2) {
		r_len = sizeof(decltype(Color().to_rgba64())) * size;
	}
}

template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void encode_color_arr(buffer_t *p_buf, const PackedColorArray &p_val) {
	if (encode_type == 0) {
		encode(p_val, p_val);
		return;
	}

	auto size = p_val.size();
	encode_varint(p_buf, size);
	if (size == 0) {
		return;
	}

	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		encode_color<encode_type>(p_buf, ptr[i]);
	}
}

#ifdef ENCODE_LEN_METHOD
template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void encode_color_arr(buffer_t *p_buf, const PackedColorArray &p_val, integral_t &r_len) {
	if (encode_type == 0) {
		encode(p_val, p_val, r_len);
		return;
	}

	auto size = p_val.size();
	encode_varint(p_buf, size, r_len);
	if (size == 0) {
		return;
	}

	auto ptr = p_val.ptr();
	for (decltype(size) i = 0; i < size; i++) {
		encode_color<encode_type>(p_buf, ptr[i], r_len);
	}
}
#endif

template <uint8_t encode_type> // 0 - rgba (4*4), 1 - hex (8), 2 - hex64 (8)
_INLINE_ void decode_color_arr(buffer_t *p_buf, PackedColorArray &r_val) {
	if (encode_type == 0) {
		decode(p_buf, r_val);
		return;
	}

	decltype(r_val.size()) size;
	decode_varint(p_buf, size);
	r_val.resize(size);
	if (size == 0) {
		return;
	}
	auto ptr = r_val.ptrw();
	for (decltype(size) i = 0; i < size; i++) {
		decode_color<encode_type>(p_buf, ptr[i]);
	}
}

} //namespace dserializer
