#pragma once

#include "dserializer.gdbase.hpp"

using namespace godot;

namespace dserializer {
enum DType : uint8_t {
	NIL,

	// atomic types
	BOOL_T,
	INT,
	FLOAT64, // 优先存储双精度
	STRING,

	// math types
	VECTOR2,
	VECTOR2I,
	RECT2,
	RECT2I,
	VECTOR3,
	VECTOR3I,
	TRANSFORM2D,
	VECTOR4,
	VECTOR4I,
	PLANE,
	QUATERNION,
	AABB,
	BASIS,
	TRANSFORM3D,
	PROJECTION,

	// misc types
	COLOR,
	STRING_NAME,
	NODE_PATH,
	// unsupported ↓
	RID,
	OBJECT,
	CALLABLE,
	SIGNAL,
	// unsupported ↑
	DICTIONARY_BIGIN,
	ARRAY_BEGIN,

	// typed arrays
	PACKED_BYTE_ARRAY,
	PACKED_INT32_ARRAY,
	PACKED_INT64_ARRAY,
	PACKED_FLOAT32_ARRAY,
	PACKED_FLOAT64_ARRAY,
	PACKED_STRING_ARRAY,
	PACKED_VECTOR2_ARRAY,
	PACKED_VECTOR3_ARRAY,
	PACKED_COLOR_ARRAY,

	VARIANT_MAX,

	// mark
	BOOL_F,
	FLOAT32,
	COLOR_HEX,
	COLOR_HEX64,
	PACKED_COLOR_ARRAY_HEX,
	PACKED_COLOR_ARRAY_HEX64,
	PACKED_INT32_ARRAY_VARINT,
	PACKED_INT64_ARRAY_VARINT,
	ARR_DICT_END,

	TYPE_MAX,
};

static auto color_encode_code = DType::COLOR;
static auto color_arr_encode_code = DType::PACKED_COLOR_ARRAY;
static bool varint_encoding_in_packed_array = false;

_INLINE_ void cal_size_type(uint8_t p_val, integral_t &r_len) {
	cal_size_int(p_val, r_len);
}
_INLINE_ void encode_type(buffer_t *p_buf, uint8_t p_val) {
	encode_int(p_buf, p_val);
}
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Array &p_val, integral_t &r_len) {
	encode_int(p_buf, p_val, r_len);
}
#endif // ENCODE_LEN_METHOD
_INLINE_ void decode_type(buffer_t *p_buf, uint8_t &r_val) {
	decode_int(p_buf, r_val);
}

//  声明
_INLINE_ void cal_size(const Array &p_val, integral_t &r_len);
_INLINE_ void encode(buffer_t *p_buf, const Array &p_val);
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Array &p_val, integral_t &r_len);
#endif // ENCODE_LEN_METHOD
_INLINE_ void decode(buffer_t *p_buf, Array &p_val, const uint8_t &type_code, const bool &empty); // 数组类解码需要头

_INLINE_ void cal_size(const Dictionary &p_val, integral_t &r_len);
_INLINE_ void encode(buffer_t *p_buf, const Dictionary &p_val);
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Dictionary &p_val, integral_t &r_len);
#endif // ENCODE_LEN_METHOD
_INLINE_ void decode(buffer_t *p_buf, Dictionary &p_val); // 空字典在外部判断

_INLINE_ void cal_size(const Variant &p_val, integral_t &r_len);
_INLINE_ void encode(buffer_t *p_buf, const Variant &p_val);
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Variant &p_val, integral_t &r_len);
#endif // ENCODE_LEN_METHOD
_INLINE_ void decode(buffer_t *p_buf, Variant &p_val);

_INLINE_ void cal_size_variant(const Variant &p_val, integral_t &r_len, const uint8_t &type);
_INLINE_ void cal_size_variant(const Variant &p_val, integral_t &r_len);
_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val, const uint8_t &type);
_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val);
#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val, integral_t &r_len);
_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val, const uint8_t &type, integral_t &r_len);
#endif // ENCODE_LEN_METHOD
_INLINE_ void decode_variant(buffer_t *p_buf, Variant &p_val, const uint8_t &type);

#define convert_to_encode_code(m_type)                     \
	switch (m_type) {                                      \
		case DType::COLOR: {                               \
			m_type = color_encode_code;                    \
		} break;                                           \
		case DType::PACKED_COLOR_ARRAY: {                  \
			m_type = color_arr_encode_code;                \
		} break;                                           \
		case DType::PACKED_INT32_ARRAY: {                  \
			if (varint_encoding_in_packed_array) {         \
				m_type = DType::PACKED_INT32_ARRAY_VARINT; \
			}                                              \
		} break;                                           \
		case DType::PACKED_INT64_ARRAY: {                  \
			if (varint_encoding_in_packed_array) {         \
				m_type = DType::PACKED_INT64_ARRAY_VARINT; \
			}                                              \
		} break;                                           \
		default: {                                         \
		} break;                                           \
	}

// 数组
_INLINE_ void convert_array_encode_code(const godot::Array &p_arr, uint8_t &r_arr_encode_code) {
	switch (p_arr.get_typed_builtin()) {
		case Variant::NIL: {
			r_arr_encode_code = DType::ARRAY_BEGIN;
		} break;
		case Variant::COLOR: {
			r_arr_encode_code = DType::TYPE_MAX + color_encode_code;
		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			r_arr_encode_code = DType::TYPE_MAX + color_arr_encode_code;
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			r_arr_encode_code = DType::TYPE_MAX + (varint_encoding_in_packed_array ? DType::PACKED_INT32_ARRAY_VARINT : DType::PACKED_INT32_ARRAY);
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			r_arr_encode_code = DType::TYPE_MAX + (varint_encoding_in_packed_array ? DType::PACKED_INT64_ARRAY_VARINT : DType::PACKED_INT64_ARRAY);
		} break;
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::SIGNAL:
		case Variant::CALLABLE: {
			ERR_FAIL_MSG("Unsupport RID, OBJECT, SIGNAL and CALLABLE.");
			return;
		}
		default:
			r_arr_encode_code = DType::TYPE_MAX + p_arr.get_typed_builtin();
			break;
	}
	// if (p_arr.is_empty())
	// 	r_arr_encode_code = (r_arr_encode_code | 0x80);
}

_INLINE_ void cal_size(const Array &p_val, integral_t &r_len) {
	if (p_val.is_empty()) {
		return;
	}
	auto type = p_val.get_typed_builtin();
	switch (type) {
		case Variant::NIL: {
			// Elements
			auto size = p_val.size();
			for (decltype(size) i = 0; i < size; i++) {
				cal_size(p_val[i], r_len);
			}
			// End
			cal_size_type(DType::ARR_DICT_END, r_len);
		} break;
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::SIGNAL:
		case Variant::CALLABLE: {
			ERR_FAIL_MSG("Can't serialize Object, Signal and Callable.");
		} break;
		default: {
			// Typed Array need encode size first.
			auto size = p_val.size();
			cal_size_varint(size, r_len);
			switch (type) {
				case DType::BOOL_T: {
					r_len += size;
				} break;
				case DType::ARRAY_BEGIN: {
					for (decltype(size) i = 0; i < size; i++) {
						cal_size(p_val[i], r_len); // If is Array, it need begining mark.
					}
				} break;
				case DType::DICTIONARY_BIGIN: {
					for (decltype(size) i = 0; i < size; i++) {
						cal_size(p_val[i].operator godot::Dictionary(), r_len);
					}
				} break;
				default: {
					convert_to_encode_code(type);
					for (decltype(size) i = 0; i < size; i++) {
						cal_size_variant(p_val[i], r_len, type);
					}
				} break;
			}
		} break;
	}
}

_INLINE_ void encode(buffer_t *p_buf, const Array &p_val) {
	if (p_val.is_empty()) {
		return;
	}

	auto type = p_val.get_typed_builtin();
	switch (type) {
		case Variant::NIL: {
			auto size = p_val.size();
			for (decltype(size) i = 0; i < size; i++) {
				encode(p_buf, p_val[i]); // With type code.
			}
			// Encode end.
			encode_type(p_buf, DType::ARR_DICT_END);
		} break;
		case Variant::OBJECT:
		case Variant::SIGNAL:
		case Variant::CALLABLE: {
			ERR_FAIL_MSG("Can't serialize Object, Signal and Callable.");
		} break;
		default: {
			// Typed Array need encode sze first.
			auto size = p_val.size();
			encode_varint(p_buf, size);
			// Elements.
			switch (type) {
				case DType::BOOL_T: {
					for (decltype(size) i = 0; i < size; i++) {
						encode_type(p_buf, p_val[i] ? DType::BOOL_T : DType::BOOL_F);
					}
				} break;
				case DType::ARRAY_BEGIN: {
					for (decltype(size) i = 0; i < size; i++) {
						encode(p_buf, p_val[i]); // Array need begin mark.
					}
				} break;
				case DType::DICTIONARY_BIGIN: {
					for (decltype(size) i = 0; i < size; i++) {
						encode(p_buf, p_val[i].operator godot::Dictionary());
					}
				} break;
				default: {
					convert_to_encode_code(type);
					for (decltype(size) i = 0; i < size; i++) {
						encode_variant(p_buf, p_val[i], type);
					}
				} break;
			}
		} break;
	}
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Array &p_val, integral_t &r_len) {
	if (p_val.is_empty()) {
		return;
	}

	auto type = p_val.get_typed_builtin();
	switch (type) {
		case Variant::NIL: {
			auto size = p_val.size();
			for (decltype(size) i = 0; i < size; i++) {
				encode(p_buf, p_val[i], r_len); // With type code.
			}
			// Encode end.
			encode_type(p_buf, DType::ARR_DICT_END, r_len);
		} break;
		case Variant::OBJECT:
		case Variant::SIGNAL:
		case Variant::CALLABLE: {
			ERR_FAIL_MSG("Can't serialize Object, Signal and Callable.");
		} break;
		default: {
			// Typed Array need encode sze first.
			auto size = p_val.size();
			encode_varint(p_buf, size, r_len);
			// Elements.
			switch (type) {
				case DType::BOOL_T: {
					for (decltype(size) i = 0; i < size; i++) {
						encode_type(p_buf, p_val[i] ? DType::BOOL_T : DType::BOOL_F, r_len);
					}
				} break;
				case DType::ARRAY_BEGIN: {
					for (decltype(size) i = 0; i < size; i++) {
						encode(p_buf, p_val[i], r_len); // Array need begin mark.
					}
				} break;
				case DType::DICTIONARY_BIGIN: {
					for (decltype(size) i = 0; i < size; i++) {
						encode(p_buf, p_val[i].operator godot::Dictionary(), r_len);
					}
				} break;
				default: {
					convert_to_encode_code(type);
					for (decltype(size) i = 0; i < size; i++) {
						encode_variant(p_buf, p_val[i], type, r_len);
					}
				} break;
			}
		} break;
	}
}

#endif

_INLINE_ void decode(buffer_t *p_buf, Array &p_val, const uint8_t &p_encode_code, const bool &p_empty) { // 需要类型信息才能解码
	if (p_encode_code == DType::ARRAY_BEGIN) {
		if (p_empty) {
			return;
		}
		while (*p_buf != DType::ARR_DICT_END) {
			Variant element;
			decode(p_buf, element);
			p_val.push_back(element);
		}
		p_buf++;
	} else {
		auto dtype = static_cast<DType>(p_encode_code - DType::TYPE_MAX);
		// Set typed
		switch (dtype) {
			case DType::COLOR:
			case DType::COLOR_HEX:
			case DType::COLOR_HEX64:
				p_val.set_typed(Variant::COLOR, StringName(), Variant());
				break;
			case DType::PACKED_COLOR_ARRAY:
			case DType::PACKED_COLOR_ARRAY_HEX:
			case DType::PACKED_COLOR_ARRAY_HEX64:
				p_val.set_typed(Variant::PACKED_COLOR_ARRAY, StringName(), Variant());
				break;
			case DType::PACKED_INT32_ARRAY:
			case DType::PACKED_INT32_ARRAY_VARINT:
				p_val.set_typed(Variant::PACKED_INT32_ARRAY, StringName(), Variant());
				break;
			case DType::PACKED_INT64_ARRAY:
			case DType::PACKED_INT64_ARRAY_VARINT:
				p_val.set_typed(Variant::PACKED_INT64_ARRAY, StringName(), Variant());
				break;
			default:
				p_val.set_typed(dtype, StringName(), Variant());
				break;
		}
		if (p_empty) {
			return; // Empty.
		}

		// Size
		decltype(p_val.size()) size;
		decode_varint(p_buf, size);
		p_val.resize(size);
		// Elements
		switch (dtype) {
			case DType::BOOL_T:
			case DType::BOOL_F: {
				for (decltype(p_val.size()) i = 0; i < size; i++) {
					uint8_t t;
					decode_type(p_buf, t);
					p_val[i] = t == BOOL_T ? true : false;
				}
			} break;
			case DType::ARRAY_BEGIN: {
				for (decltype(p_val.size()) i = 0; i < size; i++) {
					Variant arr;
					decode(p_buf, arr);
					p_val[i] = arr;
				}
			} break;
			case DType::DICTIONARY_BIGIN: {
				for (decltype(p_val.size()) i = 0; i < size; i++) {
					Dictionary element;
					decode(p_buf, element);
					p_val[i] = element;
				}
			} break;
			default: {
				for (decltype(p_val.size()) i = 0; i < size; i++) {
					Variant element;
					decode_variant(p_buf, element, dtype);
					p_val[i] = element;
				}
			} break;
		}
	}
}

// Dictionary
_INLINE_ void cal_size(const Dictionary &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto keys = p_val.keys();
	auto values = p_val.values();
	for (decltype(size) i = 0; i < size; i++) {
		cal_size(keys[i], r_len);
		cal_size(values[i], r_len);
	}
	cal_size(DType::ARR_DICT_END, r_len);
}

_INLINE_ void encode(buffer_t *p_buf, const Dictionary &p_val) {
	auto size = p_val.size();
	auto keys = p_val.keys();
	auto values = p_val.values();
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, keys[i]);
		encode(p_buf, values[i]);
	}
	encode_type(p_buf, static_cast<uint8_t>(DType::ARR_DICT_END));
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Dictionary &p_val, integral_t &r_len) {
	auto size = p_val.size();
	auto keys = p_val.keys();
	auto values = p_val.values();
	for (decltype(size) i = 0; i < size; i++) {
		encode(p_buf, keys[i], r_len);
		encode(p_buf, values[i], r_len);
	}
	encode_type(p_buf, DType::ARR_DICT_END, r_len);
}
#endif

_INLINE_ void decode(buffer_t *p_buf, Dictionary &p_val) {
	while (*p_buf != DType::ARR_DICT_END) {
		Variant key, value;
		decode(p_buf, key);
		decode(p_buf, value);
		p_val[key] = value;
	}
	p_buf++;
}

// 可变体
_INLINE_ void cal_size_variant(const Variant &p_val, integral_t &r_len, const uint8_t &type) {
	switch (type) {
		case Variant::NIL:
		case Variant::BOOL:
			cal_size_type(DType::NIL, r_len);
			break;
		case Variant::INT:
			cal_size_varint(p_val.operator int64_t(), r_len);
			break;
		case DType::FLOAT64: {
			r_len += sizeof(double);
		} break;
		case DType::FLOAT32: {
			r_len += sizeof(float);
		} break;
		case Variant::STRING: {
			cal_size(p_val.operator godot::String(), r_len);
		} break;
		case Variant::VECTOR2:
			cal_size(p_val.operator godot::Vector2(), r_len);
			break;
		case Variant::VECTOR2I:
			cal_size(p_val.operator godot::Vector2i(), r_len);
			break;
		case Variant::RECT2:
			cal_size(p_val.operator godot::Rect2(), r_len);
			break;
		case Variant::RECT2I:
			cal_size(p_val.operator godot::Rect2i(), r_len);
			break;
		case Variant::VECTOR3:
			cal_size(p_val.operator godot::Vector3(), r_len);
			break;
		case Variant::VECTOR3I:
			cal_size(p_val.operator godot::Vector3i(), r_len);
			break;
		case Variant::TRANSFORM2D:
			cal_size(p_val.operator godot::Transform2D(), r_len);
			break;
		case Variant::VECTOR4:
			cal_size(p_val.operator godot::Vector4(), r_len);
			break;
		case Variant::VECTOR4I:
			cal_size(p_val.operator godot::Vector4i(), r_len);
			break;
		case Variant::PLANE:
			cal_size(p_val.operator godot::Plane(), r_len);
			break;
		case Variant::QUATERNION:
			cal_size(p_val.operator godot::Quaternion(), r_len);
			break;
		case Variant::AABB:
			cal_size(p_val.operator godot::AABB(), r_len);
			break;
		case Variant::BASIS:
			cal_size(p_val.operator godot::Basis(), r_len);
			break;
		case Variant::TRANSFORM3D:
			cal_size(p_val.operator godot::Transform3D(), r_len);
			break;
		case Variant::PROJECTION:
			cal_size(p_val.operator godot::Projection(), r_len);
			break;
		case Variant::COLOR: {
			cal_size_color<0>(p_val.operator godot::Color(), r_len);
		} break;
		case DType::COLOR_HEX: {
			cal_size_color<1>(p_val.operator godot::Color(), r_len);
		} break;
		case DType::COLOR_HEX64: {
			cal_size_color<2>(p_val.operator godot::Color(), r_len);
		} break;
		case Variant::STRING_NAME:
			cal_size(p_val.operator godot::StringName(), r_len);
			break;
		case Variant::NODE_PATH:
			cal_size(p_val.operator godot::NodePath(), r_len);
			break;
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::CALLABLE:
		case Variant::SIGNAL: {
			ERR_FAIL_MSG("Unsupport RID, OBJECT, CALLABLE and SIGNAL.");
			break;
		}
		case Variant::DICTIONARY: {
			cal_size(p_val.operator godot::Dictionary(), r_len);
		} break;
		case Variant::ARRAY: {
			cal_size(p_val.operator godot::Array(), r_len);
		} break;
		// PackedArray
		case Variant::PACKED_BYTE_ARRAY: {
			cal_size(p_val.operator godot::PackedByteArray(), r_len);
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			cal_size_int_arr<false>(p_val.operator godot::PackedInt64Array(), r_len);
		} break;
		case DType::PACKED_INT32_ARRAY_VARINT: {
			cal_size_int_arr<true>(p_val.operator godot::PackedInt32Array(), r_len);
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			cal_size_int_arr<false>(p_val.operator godot::PackedInt64Array(), r_len);
		} break;
		case DType::PACKED_INT64_ARRAY_VARINT: {
			cal_size_int_arr<true>(p_val.operator godot::PackedInt64Array(), r_len);
		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			cal_size(p_val.operator godot::PackedFloat32Array(), r_len);
		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			cal_size(p_val.operator godot::PackedFloat64Array(), r_len);
		} break;
		case Variant::PACKED_STRING_ARRAY: {
			cal_size(p_val.operator godot::PackedStringArray(), r_len);
		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			cal_size(p_val.operator godot::PackedVector2Array(), r_len);
		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			cal_size(p_val.operator godot::PackedVector3Array(), r_len);
		} break;
		case DType::PACKED_COLOR_ARRAY: {
			cal_size_color_arr<0>(p_val.operator godot::PackedColorArray(), r_len);
		} break;
		case DType::PACKED_COLOR_ARRAY_HEX: {
			cal_size_color_arr<1>(p_val.operator godot::PackedColorArray(), r_len);
		} break;
		case DType::PACKED_COLOR_ARRAY_HEX64: {
			cal_size_color_arr<2>(p_val.operator godot::PackedColorArray(), r_len);
		} break;
		default: {
			ERR_FAIL_MSG("Unrecognized type code: " + itos(p_val.get_type()));
		} break;
	}
}

_INLINE_ void cal_size_variant(const Variant &p_val, integral_t &r_len) {
	auto type = DType(p_val.get_type());
	convert_to_encode_code(type);
	cal_size_variant(p_val, r_len, type);
}

_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val, const uint8_t &p_type) {
	switch (p_type) {
		case Variant::NIL: {
			encode_type(p_buf, static_cast<uint8_t>(DType::NIL));
		} break;
		case Variant::BOOL: {
			encode_type(p_buf, static_cast<uint8_t>((p_val) ? DType::BOOL_T : DType::BOOL_F));
		} break;
		case Variant::INT: {
			encode_varint(p_buf, p_val.operator int64_t());
		} break;
		case DType::FLOAT64: {
			encode(p_buf, p_val.operator double());
		} break;
		case DType::FLOAT32: {
			encode(p_buf, p_val.operator float());
		} break;
		case Variant::STRING: {
			encode(p_buf, p_val.operator godot::String());
		} break;
		case Variant::VECTOR2: {
			encode(p_buf, p_val.operator godot::Vector2());
		} break;
		case Variant::VECTOR2I: {
			encode(p_buf, p_val.operator godot::Vector2i());
		} break;
		case Variant::RECT2: {
			encode(p_buf, p_val.operator godot::Rect2());
		} break;
		case Variant::RECT2I: {
			encode(p_buf, p_val.operator godot::Rect2i());
		} break;
		case Variant::VECTOR3: {
			encode(p_buf, p_val.operator godot::Vector3());
		} break;
		case Variant::VECTOR3I: {
			encode(p_buf, p_val.operator godot::Vector3i());
		} break;
		case Variant::TRANSFORM2D: {
			encode(p_buf, p_val.operator godot::Transform2D());
		} break;
		case Variant::VECTOR4: {
			encode(p_buf, p_val.operator godot::Vector4());
		} break;
		case Variant::VECTOR4I: {
			encode(p_buf, p_val.operator godot::Vector4i());
		} break;
		case Variant::PLANE: {
			encode(p_buf, p_val.operator godot::Plane());
		} break;
		case Variant::QUATERNION: {
			encode(p_buf, p_val.operator godot::Quaternion());
		} break;
		case Variant::AABB: {
			encode(p_buf, p_val.operator godot::AABB());
		} break;
		case Variant::BASIS: {
			encode(p_buf, p_val.operator godot::Basis());
		} break;
		case Variant::TRANSFORM3D: {
			encode(p_buf, p_val.operator godot::Transform3D());
		} break;
		case Variant::PROJECTION: {
			encode(p_buf, p_val.operator godot::Projection());
		} break;
		case Variant::COLOR: {
			encode_color<0>(p_buf, p_val.operator godot::Color());
		} break;
		case DType::COLOR_HEX: {
			encode_color<1>(p_buf, p_val.operator godot::Color());
		} break;
		case DType::COLOR_HEX64: {
			encode_color<2>(p_buf, p_val.operator godot::Color());
		} break;
		case Variant::STRING_NAME: {
			encode(p_buf, p_val.operator godot::StringName());
		} break;
		case Variant::NODE_PATH: {
			encode(p_buf, p_val.operator godot::NodePath());
		} break;
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::CALLABLE:
		case Variant::SIGNAL: {
			ERR_FAIL_MSG("Unsupport RID, OBJECT, CALLABLE and SIGNAL.");
		} break;
		case Variant::DICTIONARY: {
			encode(p_buf, p_val.operator godot::Dictionary());
		} break;
		case Variant::ARRAY: {
			encode(p_buf, p_val.operator godot::Array());
		} break;
		// PackedArray
		case Variant::PACKED_BYTE_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedByteArray());
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			encode_int_arr<false>(p_buf, p_val.operator godot::PackedInt32Array());
		} break;
		case DType::PACKED_INT32_ARRAY_VARINT: {
			encode_int_arr<true>(p_buf, p_val.operator godot::PackedInt32Array());
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			encode_int_arr<false>(p_buf, p_val.operator godot::PackedInt64Array());
		} break;
		case DType::PACKED_INT64_ARRAY_VARINT: {
			encode_int_arr<true>(p_buf, p_val.operator godot::PackedInt64Array());
		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedFloat32Array());
		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedFloat64Array());
		} break;
		case Variant::PACKED_STRING_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedStringArray());
		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedVector2Array());
		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedVector3Array());
		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			encode_color_arr<0>(p_buf, p_val.operator godot::PackedColorArray());
		} break;
		case DType::PACKED_COLOR_ARRAY_HEX: {
			encode_color_arr<1>(p_buf, p_val.operator godot::PackedColorArray());
		} break;
		case DType::PACKED_COLOR_ARRAY_HEX64: {
			encode_color_arr<2>(p_buf, p_val.operator godot::PackedColorArray());
		} break;
		default: {
			ERR_FAIL_MSG("Unrecognized encode type code: " + itos(p_type));
		} break;
	}
}

_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val) {
	auto type = DType(p_val.get_type());
	convert_to_encode_code(type);
	encode_variant(p_buf, p_val, type);
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val, const uint8_t &type, integral_t &r_len) {
	switch (p_type) {
		case Variant::NIL: {
			encode_type(p_buf, static_cast<uint8_t>(DType::NIL), r_len);
		} break;
		case Variant::BOOL: {
			encode_type(p_buf, static_cast<uint8_t>((p_val) ? DType::BOOL_T : DType::BOOL_F), r_len);
		} break;
		case Variant::INT: {
			encode_varint(p_buf, p_val.operator int64_t(), r_len);
		} break;
		case DType::FLOAT64: {
			encode(p_buf, p_val.operator double(), r_len);
		} break;
		case DType::FLOAT32: {
			encode(p_buf, p_val.operator float(), r_len);
		} break;
		case Variant::STRING: {
			encode(p_buf, p_val.operator godot::String(), r_len);
		} break;
		case Variant::VECTOR2: {
			encode(p_buf, p_val.operator godot::Vector2(), r_len);
		} break;
		case Variant::VECTOR2I: {
			encode(p_buf, p_val.operator godot::Vector2i(), r_len);
		} break;
		case Variant::RECT2: {
			encode(p_buf, p_val.operator godot::Rect2(), r_len);
		} break;
		case Variant::RECT2I: {
			encode(p_buf, p_val.operator godot::Rect2i(), r_len);
		} break;
		case Variant::VECTOR3: {
			encode(p_buf, p_val.operator godot::Vector3(), r_len);
		} break;
		case Variant::VECTOR3I: {
			encode(p_buf, p_val.operator godot::Vector3i(), r_len);
		} break;
		case Variant::TRANSFORM2D: {
			encode(p_buf, p_val.operator godot::Transform2D(), r_len);
		} break;
		case Variant::VECTOR4: {
			encode(p_buf, p_val.operator godot::Vector4(), r_len);
		} break;
		case Variant::VECTOR4I: {
			encode(p_buf, p_val.operator godot::Vector4i(), r_len);
		} break;
		case Variant::PLANE: {
			encode(p_buf, p_val.operator godot::Plane(), r_len);
		} break;
		case Variant::QUATERNION: {
			encode(p_buf, p_val.operator godot::Quaternion(), r_len);
		} break;
		case Variant::AABB: {
			encode(p_buf, p_val.operator godot::AABB(), r_len);
		} break;
		case Variant::BASIS: {
			encode(p_buf, p_val.operator godot::Basis(), r_len);
		} break;
		case Variant::TRANSFORM3D: {
			encode(p_buf, p_val.operator godot::Transform3D(), r_len);
		} break;
		case Variant::PROJECTION: {
			encode(p_buf, p_val.operator godot::Projection(), r_len);
		} break;
		case Variant::COLOR: {
			encode_color<0>(p_buf, p_val.operator godot::Color(), r_len);
		} break;
		case DType::COLOR_HEX: {
			encode_color<1>(p_buf, p_val.operator godot::Color(), r_len);
		} break;
		case DType::COLOR_HEX64: {
			encode_color<2>(p_buf, p_val.operator godot::Color(), r_len);
		} break;
		case Variant::STRING_NAME: {
			encode(p_buf, p_val.operator godot::StringName(), r_len);
		} break;
		case Variant::NODE_PATH: {
			encode(p_buf, p_val.operator godot::NodePath(), r_len);
		} break;
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::CALLABLE:
		case Variant::SIGNAL: {
			ERR_FAIL_MSG("Unsupport RID, OBJECT, CALLABLE and SIGNAL.");
		} break;
		case Variant::DICTIONARY: {
			encode(p_buf, p_val.operator godot::Dictionary(), r_len);
		} break;
		case Variant::ARRAY: {
			encode(p_buf, p_val.operator godot::Array(), r_len);
		} break;
		// PackedArray
		case Variant::PACKED_BYTE_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedByteArray(), r_len);
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			encode_int_arr<false>(p_buf, p_val.operator godot::PackedInt32Array(), r_len);
		} break;
		case DType::PACKED_INT32_ARRAY_VARINT: {
			encode_int_arr<true>(p_buf, p_val.operator godot::PackedInt32Array(), r_len);
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			encode_int_arr<false>(p_buf, p_val.operator godot::PackedInt64Array(), r_len);
		} break;
		case DType::PACKED_INT64_ARRAY_VARINT: {
			encode_int_arr<true>(p_buf, p_val.operator godot::PackedInt64Array(), r_len);
		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedFloat32Array(), r_len);
		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedFloat64Array(), r_len);
		} break;
		case Variant::PACKED_STRING_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedStringArray(), r_len);
		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedVector2Array(), r_len);
		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			encode(p_buf, p_val.operator godot::PackedVector3Array(), r_len);
		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			encode_color_arr<0>(p_buf, p_val.operator godot::PackedColorArray(), r_len);
		} break;
		case DType::PACKED_COLOR_ARRAY_HEX: {
			encode_color_arr<1>(p_buf, p_val.operator godot::PackedColorArray(), r_len);
		} break;
		case DType::PACKED_COLOR_ARRAY_HEX64: {
			encode_color_arr<2>(p_buf, p_val.operator godot::PackedColorArray(), r_len);
		} break;
		default: {
			ERR_FAIL_MSG("Unrecognized encode type code: " + itos(p_type));
		} break;
	}
}

_INLINE_ void encode_variant(buffer_t *p_buf, const Variant &p_val, integral_t &r_len) {
	auto type = DType(p_val.get_type());
	convert_to_encode_code(type);
	encode_variant(p_buf, p_val, type, r_len);
}

#endif

_INLINE_ void decode_variant(buffer_t *p_buf, Variant &p_val, const uint8_t &type) {
	switch (type) {
		case DType::NIL: {
			p_val = Variant();
		} break;
		case DType::BOOL_T: {
			p_val = true;
		} break;
		case DType::BOOL_F: {
			p_val = false;
		} break;
		case DType::INT: {
			int64_t v;
			decode_varint(p_buf, v);
			p_val = v;
		} break;
		case DType::FLOAT32: {
			float real32;
			decode(p_buf, real32);
			p_val = real32;
		} break;
		case DType::FLOAT64: {
			double real64;
			decode(p_buf, real64);
			p_val = real64;
		} break;
		case DType::STRING: {
			godot::String v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::VECTOR2: {
			godot::Vector2 v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::VECTOR2I: {
			godot::Vector2i v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::RECT2: {
			godot::Rect2 v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::RECT2I: {
			godot::Rect2i v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::VECTOR3: {
			godot::Vector3 v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::VECTOR3I: {
			godot::Vector3i v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::TRANSFORM2D: {
			godot::Transform2D v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::VECTOR4: {
			godot::Vector4 v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::VECTOR4I: {
			godot::Vector4i v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::PLANE: {
			godot::Plane v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::QUATERNION: {
			godot::Quaternion v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::AABB: {
			godot::AABB v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::BASIS: {
			godot::Basis v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::TRANSFORM3D: {
			godot::Transform3D v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::PROJECTION: {
			godot::Projection v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::COLOR: {
			godot::Color v;
			decode_color<0>(p_buf, v);
			p_val = v;
		} break;
		case DType::COLOR_HEX: {
			godot::Color v;
			decode_color<1>(p_buf, v);
			p_val = v;
		} break;
		case DType::COLOR_HEX64: {
			godot::Color v;
			decode_color<2>(p_buf, v);
			p_val = v;
		} break;
		case DType::STRING_NAME: {
			godot::StringName v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case DType::NODE_PATH: {
			godot::NodePath v;
			decode(p_buf, v);
			p_val = v;
		} break;
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::CALLABLE:
		case Variant::SIGNAL: {
			ERR_FAIL_MSG("Unsupport RID, OBJECT, CALLABLE and SIGNAL.");
		} break;
		default: { // 容器类
			auto t = type & 0x7f;
			auto empty = t != type;
			switch (t) {
				case DType::DICTIONARY_BIGIN: {
					if (empty) {
						p_val = godot::Dictionary();
					} else {
						godot::Dictionary v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_BYTE_ARRAY: {
					if (empty) {
						p_val = godot::PackedByteArray();
					} else {
						godot::PackedByteArray v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_INT32_ARRAY: {
					if (empty) {
						p_val = godot::PackedInt32Array();
					} else {
						godot::PackedInt32Array v;
						decode_int_arr<false>(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_INT32_ARRAY_VARINT: {
					if (empty) {
						p_val = godot::PackedInt32Array();
					} else {
						godot::PackedInt32Array v;
						decode_int_arr<true>(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_INT64_ARRAY: {
					if (empty) {
						p_val = godot::PackedInt64Array();
					} else {
						godot::PackedInt64Array v;
						decode_int_arr<false>(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_INT64_ARRAY_VARINT: {
					if (empty) {
						p_val = godot::PackedInt64Array();
					} else {
						godot::PackedInt64Array v;
						decode_int_arr<true>(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_FLOAT32_ARRAY: {
					if (empty) {
						p_val = godot::PackedFloat32Array();
					} else {
						godot::PackedFloat32Array v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_FLOAT64_ARRAY: {
					if (empty) {
						p_val = godot::PackedFloat64Array();
					} else {
						godot::PackedFloat64Array v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_STRING_ARRAY: {
					if (empty) {
						p_val = godot::PackedStringArray();
					} else {
						godot::PackedStringArray v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_VECTOR2_ARRAY: {
					if (empty) {
						p_val = godot::PackedVector2Array();
					} else {
						godot::PackedVector2Array v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_VECTOR3_ARRAY: {
					if (empty) {
						p_val = godot::PackedVector3Array();
					} else {
						godot::PackedVector3Array v;
						decode(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_COLOR_ARRAY: {
					if (empty) {
						p_val = godot::PackedColorArray();
					} else {
						godot::PackedColorArray v;
						decode_color_arr<0>(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_COLOR_ARRAY_HEX: {
					if (empty) {
						p_val = godot::PackedColorArray();
					} else {
						godot::PackedColorArray v;
						decode_color_arr<1>(p_buf, v);
						p_val = v;
					}
				} break;
				case DType::PACKED_COLOR_ARRAY_HEX64: {
					if (empty) {
						p_val = godot::PackedColorArray();
					} else {
						godot::PackedColorArray v;
						decode_color_arr<2>(p_buf, v);
						p_val = v;
					}
				} break;
				default: { // 数组类
					godot::Array v;
					decode(p_buf, v, t, empty);
					p_val = v;
				} break;
			}
		}
	}
}

_INLINE_ void cal_size(const Variant &p_val, integral_t &r_len) {
	r_len++;

	uint8_t type = p_val.get_type();
	switch (type) {
		case Variant::NIL:
		case Variant::BOOL:
			return;

		case Variant::ARRAY: {
			auto arr = p_val.operator godot::Array();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::DICTIONARY: {
			auto dict = p_val.operator godot::Dictionary();
			if (dict.is_empty()) {
				return;
			}
			cal_size(dict, r_len);
		} break;
		case Variant::PACKED_BYTE_ARRAY: {
			auto arr = p_val.operator godot::PackedByteArray();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			auto arr = p_val.operator godot::PackedInt32Array();
			if (arr.is_empty()) {
				return;
			}

			if (!varint_encoding_in_packed_array) {
				cal_size_int_arr<false>(arr, r_len);
			} else {
				cal_size_int_arr<true>(arr, r_len);
			}
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			auto arr = p_val.operator godot::PackedInt64Array();
			if (arr.is_empty()) {
				return;
			}

			if (!varint_encoding_in_packed_array) {
				cal_size_int_arr<false>(arr, r_len);
			} else {
				cal_size_int_arr<true>(arr, r_len);
			}
		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			auto arr = p_val.operator godot::PackedFloat32Array();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			auto arr = p_val.operator godot::PackedFloat64Array();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			auto arr = p_val.operator godot::PackedVector2Array();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			auto arr = p_val.operator godot::PackedVector3Array();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::PACKED_STRING_ARRAY: {
			auto arr = p_val.operator godot::PackedStringArray();
			if (arr.is_empty()) {
				return;
			}
			cal_size(arr, r_len);
		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			auto arr = p_val.operator godot::PackedColorArray();
			if (arr.is_empty()) {
				return;
			}
			switch (color_arr_encode_code) {
				case DType::PACKED_COLOR_ARRAY_HEX: {
					cal_size_color_arr<1>(arr, r_len);
				} break;
				case DType::PACKED_COLOR_ARRAY_HEX64: {
					cal_size_color_arr<2>(arr, r_len);
				} break;
				default: {
					cal_size_color_arr<0>(arr, r_len);
				} break;
			}
		} break;
		case Variant::COLOR: {
			switch (color_encode_code) {
				case DType::COLOR_HEX: {
					cal_size_color<1>(p_val, r_len);
				} break;
				case DType::COLOR_HEX64: {
					cal_size_color<2>(p_val, r_len);
				} break;
				default: {
					cal_size_color<0>(p_val, r_len);
				} break;
			}
		}
		default: {
			cal_size_variant(p_val, r_len, type);
		} break;
	}
}

_INLINE_ void encode(buffer_t *p_buf, const Variant &p_val) {
	switch (p_val.get_type()) {
		case Variant::NIL: {
			encode_type(p_buf, DType::NIL);
		} break;
		case Variant::BOOL: {
			encode_type(p_buf, p_val ? DType::BOOL_T : DType::BOOL_F);
		} break;
		case Variant::ARRAY: {
			auto arr = p_val.operator godot::Array();
			uint8_t arr_encode_code;
			convert_array_encode_code(arr, arr_encode_code);
			if (arr.is_empty()) {
				encode_type(p_buf, arr_encode_code | 0x80);
				return;
			}
			encode_type(p_buf, arr_encode_code);
			encode(p_buf, arr);
		} break;
		case Variant::FLOAT: {
			auto f64 = p_val.operator double();
			auto f32 = p_val.operator float();
			if (f64 == double(f32)) {
				encode_type(p_buf, DType::FLOAT32);
				encode(p_buf, f32);
			} else {
				encode_type(p_buf, DType::FLOAT64);
				encode(p_buf, f64);
			}
		} break;
		case Variant::COLOR: {
			encode_type(p_buf, color_encode_code);
			switch (color_encode_code) {
				case DType::COLOR_HEX: {
					encode_color<1>(p_buf, p_val);
				} break;
				case DType::COLOR_HEX64: {
					encode_color<2>(p_buf, p_val);
				} break;
				default: {
					encode_color<0>(p_buf, p_val);
				} break;
			}
		} break;
		case Variant::DICTIONARY: {
			auto dict = p_val.operator godot::Dictionary();
			if (dict.is_empty()) {
				encode_type(p_buf, DType::DICTIONARY_BIGIN | 0x80);
				return;
			}
			encode_type(p_buf, DType::DICTIONARY_BIGIN);
			encode(p_buf, dict);
		} break;
		case Variant::PACKED_BYTE_ARRAY: {
			auto arr = p_val.operator godot::PackedByteArray();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_BYTE_ARRAY | 0x80);
				return;
			}
			encode_type(p_buf, DType::PACKED_BYTE_ARRAY);
			encode(p_buf, arr);
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			auto arr = p_val.operator godot::PackedInt32Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_INT32_ARRAY | 0x80);
				return;
			}
			if (!varint_encoding_in_packed_array) {
				encode_type(p_buf, DType::PACKED_INT32_ARRAY);
				encode_int_arr<false>(p_buf, arr);
			} else {
				encode_type(p_buf, DType::PACKED_INT32_ARRAY_VARINT);
				encode_int_arr<true>(p_buf, arr);
			}
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			auto arr = p_val.operator godot::PackedInt64Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_INT64_ARRAY | 0x80);
				return;
			}
			if (!varint_encoding_in_packed_array) {
				encode_type(p_buf, DType::PACKED_INT64_ARRAY);
				encode_int_arr<false>(p_buf, arr);
			} else {
				encode_type(p_buf, DType::PACKED_INT64_ARRAY_VARINT);
				encode_int_arr<true>(p_buf, arr);
			}
		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			auto arr = p_val.operator godot::PackedFloat32Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_FLOAT32_ARRAY | 0x80);
				return;
			}
			encode_type(p_buf, DType::PACKED_FLOAT32_ARRAY);
			encode(p_buf, arr);
		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			auto arr = p_val.operator godot::PackedFloat64Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_FLOAT64_ARRAY | 0x80);
				return;
			}
			encode_type(p_buf, DType::PACKED_FLOAT64_ARRAY);
			encode(p_buf, arr);
		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			auto arr = p_val.operator godot::PackedVector3Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_VECTOR2_ARRAY | 0x80);
				return;
			}
			encode_type(p_buf, DType::PACKED_VECTOR2_ARRAY);
			encode(p_buf, arr);
		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			auto arr = p_val.operator godot::PackedVector3Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_VECTOR3_ARRAY | 0x80);
				return;
			}
			encode_type(p_buf, DType::PACKED_VECTOR3_ARRAY);
			encode(p_buf, arr);
		} break;
		case Variant::PACKED_STRING_ARRAY: {
			auto arr = p_val.operator godot::PackedStringArray();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_STRING_ARRAY | 0x80);
				return;
			}
			encode_type(p_buf, DType::PACKED_STRING_ARRAY);
			encode(p_buf, arr);
		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			auto arr = p_val.operator godot::PackedColorArray();
			if (arr.is_empty()) {
				encode_type(p_buf, color_arr_encode_code | 0x80);
				return;
			}
			encode_type(p_buf, color_arr_encode_code);
			switch (color_arr_encode_code) {
				case DType::COLOR_HEX: {
					encode_color_arr<1>(p_buf, arr);
				} break;
				case DType::COLOR_HEX64: {
					encode_color_arr<2>(p_buf, arr);
				} break;
				default: {
					encode_color_arr<0>(p_buf, arr);
				} break;
			}
		} break;
		default: {
			encode_type(p_buf, p_val.get_type());
			encode_variant(p_buf, p_val);
		} break;
	}
}

#ifdef ENCODE_LEN_METHOD
_INLINE_ void encode(buffer_t *p_buf, const Variant &p_val, integral_t &r_len) {
	switch (p_val.get_type()) {
		case Variant::NIL: {
			encode_type(p_buf, DType::NIL, r_len);
		} break;
		case Variant::BOOL: {
			encode_type(p_buf, p_val ? DType::BOOL_T : DType::BOOL_F, r_len);
		} break;
		case Variant::ARRAY: {
			auto arr = p_val.operator godot::Array();
			uint8_t arr_encode_code;
			convert_array_encode_code(arr, arr_encode_code);
			if (arr.is_empty()) {
				encode_type(p_buf, arr_encode_code | 0x80, r_len);
				return;
			}
			encode_type(p_buf, arr_encode_code, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::FLOAT: {
			auto f64 = p_val.operator double();
			auto f32 = p_val.operator float();
			if (f64 == double(f32)) {
				encode_type(p_buf, DType::FLOAT32, r_len);
				encode(p_buf, f32, r_len);
			} else {
				encode_type(p_buf, DType::FLOAT64), r_len;
				encode(p_buf, f64, r_len);
			}
		} break;
		case Variant::COLOR: {
			encode_type(p_buf, color_encode_code, r_len);
			switch (color_encode_code) {
				case DType::COLOR_HEX: {
					encode_color<1>(p_buf, p_val, r_len);
				} break;
				case DType::COLOR_HEX64: {
					encode_color<2>(p_buf, p_val, r_len);
				} break;
				default: {
					encode_color<0>(p_buf, p_val, r_len);
				} break;
			}
		} break;
		case Variant::DICTIONARY: {
			auto dict = p_val.operator godot::Dictionary();
			if (dict.is_empty()) {
				encode_type(p_buf, DType::DICTIONARY_BIGIN | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::DICTIONARY_BIGIN, r_len);
			encode(p_buf, dict, r_len);
		} break;
		case Variant::PACKED_BYTE_ARRAY: {
			auto arr = p_val.operator godot::PackedByteArray();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_BYTE_ARRAY | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::PACKED_BYTE_ARRAY, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::PACKED_INT32_ARRAY: {
			auto arr = p_val.operator godot::PackedInt32Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_INT32_ARRAY | 0x80, r_len);
				return;
			}
			if (!varint_encoding_in_packed_array) {
				encode_type(p_buf, DType::PACKED_INT32_ARRAY, r_len);
				encode_int_arr<false>(p_buf, arr, r_len);
			} else {
				encode_type(p_buf, DType::PACKED_INT32_ARRAY_VARINT, r_len);
				encode_int_arr<true>(p_buf, arr, r_len);
			}
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			auto arr = p_val.operator godot::PackedInt64Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_INT64_ARRAY | 0x80, r_len);
				return;
			}
			if (!varint_encoding_in_packed_array) {
				encode_type(p_buf, DType::PACKED_INT64_ARRAY, r_len);
				encode_int_arr<false>(p_buf, arr, r_len);
			} else {
				encode_type(p_buf, DType::PACKED_INT64_ARRAY_VARINT, r_len);
				encode_int_arr<true>(p_buf, arr, r_len);
			}
		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			auto arr = p_val.operator godot::PackedFloat32Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_FLOAT32_ARRAY | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::PACKED_FLOAT32_ARRAY, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			auto arr = p_val.operator godot::PackedFloat64Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_FLOAT64_ARRAY | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::PACKED_FLOAT64_ARRAY, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			auto arr = p_val.operator godot::PackedVector3Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_VECTOR2_ARRAY | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::PACKED_VECTOR2_ARRAY, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			auto arr = p_val.operator godot::PackedVector3Array();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_VECTOR3_ARRAY | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::PACKED_VECTOR3_ARRAY, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::PACKED_STRING_ARRAY: {
			auto arr = p_val.operator godot::PackedStringArray();
			if (arr.is_empty()) {
				encode_type(p_buf, DType::PACKED_STRING_ARRAY | 0x80, r_len);
				return;
			}
			encode_type(p_buf, DType::PACKED_STRING_ARRAY, r_len);
			encode(p_buf, arr, r_len);
		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			auto arr = p_val.operator godot::PackedColorArray();
			if (arr.is_empty()) {
				encode_type(p_buf, color_arr_encode_code | 0x80, r_len);
				return;
			}
			encode_type(p_buf, color_arr_encode_code, r_len);
			switch (color_arr_encode_code) {
				case DType::COLOR_HEX: {
					encode_color_arr<1>(p_buf, arr, r_len);
				} break;
				case DType::COLOR_HEX64: {
					encode_color_arr<2>(p_buf, arr, r_len);
				} break;
				default: {
					encode_color_arr<0>(p_buf, arr, r_len);
				} break;
			}
		} break;
		default: {
			encode_type(p_buf, p_val.get_type());
			encode_variant(p_buf, p_val);
		} break;
	}
}
#endif
_INLINE_ void decode(buffer_t *p_buf, Variant &p_val) {
	uint8_t type;
	decode_type(p_buf, type);
	decode_variant(p_buf, p_val, type);
}
} //namespace dserializer
