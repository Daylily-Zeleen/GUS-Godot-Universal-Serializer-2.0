/**
 * @file godot_universal_serialzer.h
 * @author 忘忧の - DaylilyZeleen (735170336@qq.com)
 * @brief Godot 4.x 的通用序列化器 - GUS 2.0
 * @version 0.1
 * @date 2022-03-04
 * 面向 Godot 内建数据类型的序列化与反序列化。但是不支持与 Object 相关的类型 （Object, RID , Callable, Signal)
 *
 * 特点：
 * 	1、以序列化尺寸优化为目标。
 *  2、整型是变长编码。
 *	3、cpp 构建，性能较高。
 *
 * 注意：
 * 	1、不支持与 Object 相关的类型 （Object, RID , Callable, Signal), 容器类型中也不能包含。
 * 	2、暂不支持自行构建 real_t 为 double 的 Godot 4.x 版本(编译出问题)。
 *
 * @copyright Copyright (c) 2022
 *  MIT License
 * @todo
 *  1、ascii字符串的高效解码（目前 UTF8 和 ASCII 编码 都是通过 UTF8 解码)。
 * 	2、如果将来 GDExtension 推出向绑定静态方法的教程，会把方法绑定为 GDScript 2.0 的静态方法 (目前是以插件的形式向 Godot 添加 'GUS' Autoload)。
 */
#ifndef GODOT_UNIVERSAL_SERIALIZER_H
#define GODOT_UNIVERSAL_SERIALIZER_H

#include <godot_cpp/classes/object.hpp>

// #define REAL_T_IS_DOUBLE

// 根据 Godot 对 real_t 的定义选择， 默认 real_t 为 float
#ifdef REAL_T_IS_DOUBLE
typedef double real_t;
typedef uint64_t uintr_t;
#else
typedef float real_t;
typedef uint32_t uintr_t;
#endif


using namespace godot;

class GodotUniversialSerializer : public Object {
	GDCLASS(GodotUniversialSerializer, Object)

public:
	GodotUniversialSerializer() {}
	~GodotUniversialSerializer() {}

	/**
	 * @brief 颜色的编码类型
	 * RGBA32 :使用4个字节进行编码，精度最低，几乎不可能还原成编码前的数值，在颜色精度低的场合推荐使用，也是默认值
	 * RGBA64 :使用8个字节进行编码，精度较低，几乎不可能还原成编码前的数值，在颜色精度不高的场合推荐使用
	 * RGBA_FLOAT :使用16个字节进行编码，精度最高，除了跨平台或跨架构无法保证外，能准确的还原成编码前数值，推荐只在颜色精度要求最高时使用
	 */
	enum ColorEncoding {
		RGBA32,
		RGBA64,
		RGBA_FLOAT,
	};

	// 绑定属性用访问器 (这些属性都是静态属性)
	void set_string_encoding_utf8(const bool yes) {stringEncodingUtf8 = yes;}
	bool is_string_encoding_utf8() { return stringEncodingUtf8; }

	void set_string_name_encoding_utf8(const bool yes) {stringNameEncodingUtf8 = yes;}
	bool is_string_name_encoding_utf8() { return stringNameEncodingUtf8; }

	void set_node_path_encoding_utf8(const bool yes) {nodePathEncodingUtf8 = yes;}
	bool is_node_path_encoding_utf8() { return nodePathEncodingUtf8; }

	void set_color_encoding(const int encodeing_type) {
		if (encodeing_type < 0 || encodeing_type > 2)
			return;
		colorEncoding = (ColorEncoding)encodeing_type;
	}
	int get_color_encoding() const { return colorEncoding; }

	// 绑定方法（暂时找不到GDextension绑定静态方法的办法）
	PackedByteArray to_bytes(const Variant &var) const { return _to_bytes(var);	}
	Variant to_var(const PackedByteArray &bytes) const { return _to_var(bytes);	}


	// C++ 内调用静态方法
	static inline PackedByteArray _to_bytes(const Variant &p_var) {
		uint32_t len;
		Error err = encode_variant(p_var, nullptr, len, false);
		if (err != OK) {
			return PackedByteArray();
		}

		PackedByteArray barr;
		barr.resize(len);
		{
			uint8_t *w = barr.ptrw();
			err = encode_variant(p_var, w, len, false);
			if (err != OK) {
				return PackedByteArray();
			}
		}

		return barr;
	}
	static inline Variant _to_var(const PackedByteArray &p_arr) {
		Variant ret;
		{
			// UtilityFunctions::printerr(p_arr);
			const uint8_t *r = p_arr.ptr();
			Error err = decode_variant(ret, r, p_arr.size(), nullptr, false);
			if (err != OK) {
				return Variant();
			}
		}
		return ret;
	}

	// C++ 内不与 godot 交互时可采用的方法
	static inline uint8_t *var2bytes(const Variant &p_var, uint32_t &r_size) {
		Error err = encode_variant(p_var, nullptr, r_size, false);
		if (err != OK) {
			return nullptr;
		}
		if (r_size >0){
			uint8_t *buf = memnew_arr(uint8_t,r_size);
			err = encode_variant(p_var, buf, r_size, false);
			if (err != OK) {
				return nullptr;
			}
			else return buf;
		}
		return nullptr;

	}
	static inline Variant *bytes2var(const uint8_t *p_arr, const uint32_t p_size) {
		Variant *ret;
		{
			Error err = decode_variant(*ret, p_arr, p_size, nullptr, false);
			if (err != OK) {
				return nullptr;
			}
		}
		return ret;
	}

private:
	// 属性
	static bool stringEncodingUtf8;// = CharacterEncoding::UTF8;
	static bool stringNameEncodingUtf8;// = CharacterEncoding::UTF8;
	static bool nodePathEncodingUtf8;// = CharacterEncoding::ASCII;
	static ColorEncoding colorEncoding;

	// 类型代号
	enum {
		STR,
		STR_NAME,
		NULL_,
		INT_1,
		INT_2,
		INT_3,
		INT_4,
		INT_5,
		INT_6,
		INT_7,
		INT_8,
		FLOAT_4,
		FLOAT_8,
		VEC2,
		VEC2I,
		RECT2,
		RECT2I,
		VEC3,
		VEC3I,
		TRANSFORM2D,
		PLANE,
		QUAT,
		AABB_,
		BASIS,
		TRANSFORM3D,
		COLOR,
		NODE_PATH,
		// max 2^32
		RAW_ARR,
		INT32_ARR,
		INT64_ARR,
		FLOAT32_ARR,
		FLOAT64_ARR,
		VEC2_ARR,
		STR_ARR,
		VEC3_ARR,
		COLOR_ARR,
		// optimazation size
		EMPTY_DICT,
		EMPTY_ARR,
		EMPTY_RAW_ARR,
		EMPTY_INT32_ARR,
		EMPTY_INT64_ARR,
		EMPTY_FLOAT32_ARR,
		EMPTY_FLOAT64_ARR,
		EMPTY_STR_ARR,
		EMPTY_VEC2_ARR,
		EMPTY_VEC3_ARR,
		EMPTY_COLOR_ARR,
		//
		FALSE_CAP = 70, // 'F'
		TRUE_CAP = 84, // 'T'
		ARR_BEGIN = 91, // '['
		ARR_END = 93, // ']'
		FALSE_LOW = 102, // 'f'
		TRUE_LOW = 116, // 't'
		DICT_BEGIN = 123, // '{'
		DICT_END = 125, // '}'
	};

	union MarshallFloat {
	uint32_t i; ///< int
	float f; ///< float
	};

	union MarshallDouble {
		uint64_t l; ///< long long
		double d; ///< double
	};

	// Behaves like one of the above, depending on compilation setting.
	union MarshallReal {
		uintr_t i;
		real_t r;
	};

	static inline unsigned int encode_type_code(uint8_t p_type_code, uint8_t *p_arr) {
		*p_arr = p_type_code ;
		return sizeof(uint8_t);
	}

	static inline unsigned int encode_uint8(uint8_t p_uint, uint8_t *p_arr) {
		*p_arr = p_uint ;
		return sizeof(uint8_t);
	}
	static inline uint8_t decode_type_code(const uint8_t *p_arr){
		return *p_arr;
	}
	static inline uint8_t decode_uint8(const uint8_t *p_arr) {
		return *p_arr;
	}

	static inline unsigned int encode_uint16(uint16_t p_uint, uint8_t *p_arr) {
		for (int i = 0; i < 2; i++) {
			*p_arr = p_uint & 0xFF;
			p_arr++;
			p_uint >>= 8;
		}

		return sizeof(uint16_t);
	}

	static inline unsigned int encode_uint32(uint32_t p_uint, uint8_t *p_arr) {
		for (int i = 0; i < 4; i++) {
			*p_arr = p_uint & 0xFF;
			p_arr++;
			p_uint >>= 8;
		}

		return sizeof(uint32_t);
	}

	static inline unsigned int encode_float(float p_float, uint8_t *p_arr) {
		MarshallFloat mf;
		mf.f = p_float;
		encode_uint32(mf.i, p_arr);

		return sizeof(uint32_t);
	}

	static inline unsigned int encode_uint64(uint64_t p_uint, uint8_t *p_arr) {
		for (int i = 0; i < 8; i++) {
			*p_arr = p_uint & 0xFF;
			p_arr++;
			p_uint >>= 8;
		}

		return sizeof(uint64_t);
	}

	static inline unsigned int encode_double(double p_double, uint8_t *p_arr) {
		MarshallDouble md;
		md.d = p_double;
		encode_uint64(md.l, p_arr);

		return sizeof(uint64_t);
	}

	static inline unsigned int encode_uintr(uintr_t p_uint, uint8_t *p_arr) {
		for (size_t i = 0; i < sizeof(uintr_t); i++) {
			*p_arr = p_uint & 0xFF;
			p_arr++;
			p_uint >>= 8;
		}

		return sizeof(uintr_t);
	}

	static inline unsigned int encode_real(real_t p_real, uint8_t *p_arr) {
		MarshallReal mr;
		mr.r = p_real;
		encode_uintr(mr.i, p_arr);

		return sizeof(uintr_t);
	}

	static inline int encode_cstring(const char *p_string, uint8_t *p_data) {
		int len = 0;

		while (*p_string) {
			if (p_data) {
				*p_data = (uint8_t)*p_string;
				p_data++;
			}
			p_string++;
			len++;
		}

		if (p_data) {
			*p_data = 0;
		}
		return len + 1;
	}

	static inline uint16_t decode_uint16(const uint8_t *p_arr) {
		uint16_t u = 0;

		for (int i = 0; i < 2; i++) {
			uint16_t b = *p_arr;
			b <<= (i * 8);
			u |= b;
			p_arr++;
		}

		return u;
	}

	static inline uint32_t decode_uint32(const uint8_t *p_arr) {
		uint32_t u = 0;

		for (int i = 0; i < 4; i++) {
			uint32_t b = *p_arr;
			b <<= (i * 8);
			u |= b;
			p_arr++;
		}

		return u;
	}

	static inline float decode_float(const uint8_t *p_arr) {
		MarshallFloat mf;
		mf.i = decode_uint32(p_arr);
		return mf.f;
	}

	static inline uint64_t decode_uint64(const uint8_t *p_arr) {
		uint64_t u = 0;

		for (int i = 0; i < 8; i++) {
			uint64_t b = (*p_arr) & 0xFF;
			b <<= (i * 8);
			u |= b;
			p_arr++;
		}

		return u;
	}

	static inline double decode_double(const uint8_t *p_arr) {
		MarshallDouble md;
		md.l = decode_uint64(p_arr);
		return md.d;
	}


	static Error decode_variant(Variant &r_variant, const uint8_t *p_buffer, uint32_t p_len, uint32_t *r_len = nullptr, bool p_allow_objects = false, int p_depth = 0);
	static Error encode_variant(const Variant &p_variant, uint8_t *r_buffer, uint32_t &r_len, bool p_full_objects = false, int p_depth = 0);

	static void _encode_string_utf8_without_length(const String &p_string, uint8_t *&buf, uint32_t &r_len);
	static void _encode_string_ascii_without_length(const String &p_string, uint8_t *&buf, uint32_t &r_len);
	// 如何高效的解码 ascii 字符串?? 目前是只按 utf8 解码
	static Error _decode_string_utf8_without_length(const uint8_t *&buf, uint32_t &len, uint32_t *r_len, String &r_string);

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("to_bytes", "variant"), &to_bytes);
		ClassDB::bind_method(D_METHOD("to_var", "bytes"), &to_var);

		godot::ClassDB::bind_method(godot::D_METHOD("get_color_encoding"), &get_color_encoding);
		godot::ClassDB::bind_method(godot::D_METHOD("set_color_encoding", "color_encoding"), &set_color_encoding);
		ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "color_encoding"), "set_color_encoding", "get_color_encoding");

		godot::ClassDB::bind_method(godot::D_METHOD("is_string_encoding_utf8"), &is_string_encoding_utf8);
		godot::ClassDB::bind_method(godot::D_METHOD("set_string_encoding_utf8", "string_encoding"), &set_string_encoding_utf8);
		ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "string_encoding_utf8"), "set_string_encoding_utf8", "is_string_encoding_utf8");

		godot::ClassDB::bind_method(godot::D_METHOD("is_string_name_encoding_utf8"), &is_string_name_encoding_utf8);
		godot::ClassDB::bind_method(godot::D_METHOD("set_string_name_encoding_utf8", "string_encoding"), &set_string_name_encoding_utf8);
		ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "string_name_encoding_utf8"), "set_string_name_encoding_utf8", "is_string_name_encoding_utf8");

		godot::ClassDB::bind_method(godot::D_METHOD("is_node_path_encoding_utf8"), &is_node_path_encoding_utf8);
		godot::ClassDB::bind_method(godot::D_METHOD("set_node_path_encoding_utf8", "node_path_encoding_utf8"), &set_node_path_encoding_utf8);
		ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "node_path_encoding_utf8"), "set_node_path_encoding_utf8", "is_node_path_encoding_utf8");

		BIND_ENUM_CONSTANT(RGBA32);
		BIND_ENUM_CONSTANT(RGBA64);
		BIND_ENUM_CONSTANT(RGBA_FLOAT);
	}
};
GodotUniversialSerializer::ColorEncoding GodotUniversialSerializer::colorEncoding = GodotUniversialSerializer::ColorEncoding::RGBA32;

bool GodotUniversialSerializer::stringEncodingUtf8 = true;// = CharacterEncoding::UTF8;
bool GodotUniversialSerializer::stringNameEncodingUtf8 = false;// = CharacterEncoding::UTF8;
bool GodotUniversialSerializer::nodePathEncodingUtf8 = false;// = CharacterEncoding::ASCII;

VARIANT_ENUM_CAST(GodotUniversialSerializer, ColorEncoding);

#define Variant_MAX_RECURSION_DEPTH 1024

#define ERR_FAIL_ADD_OF(a, b, err) ERR_FAIL_COND_V(((int32_t)(b)) < 0 || ((int32_t)(a)) < 0 || ((int32_t)(a)) > INT_MAX - ((int32_t)(b)), err)
#define ERR_FAIL_MUL_OF(a, b, err) ERR_FAIL_COND_V(((int32_t)(a)) < 0 || ((int32_t)(b)) <= 0 || ((int32_t)(a)) > INT_MAX / ((int32_t)(b)), err)

Error GodotUniversialSerializer::_decode_string_utf8_without_length(const uint8_t *&buf, uint32_t &len, uint32_t *r_len, String &r_string) {
	ERR_FAIL_COND_V(len < 1, ERR_INVALID_DATA);

	int32_t strlen = 0;

	while (buf[strlen]!=0){
		strlen ++;
	}
	strlen ++;

	// Ensure buffer is big enough
	ERR_FAIL_COND_V(strlen > len, ERR_FILE_EOF);

	String str;
	str.parse_utf8((const char *)buf, strlen);
	// ERR_FAIL_COND_V(, ERR_INVALID_DATA);
	r_string = str;

	// Update buffer pos, left data count, and return size
	buf += strlen;
	len -= strlen;
	if (r_len) {
		(*r_len) += strlen;
	}

	return OK;
}


template<typename INT32OR64>
static inline Error decode_varlenth_int(uint8_t buflen, const uint8_t *p_buffer, Variant &r_variant, uint32_t &len, uint32_t *r_len){
	ERR_FAIL_COND_V(len < buflen, ERR_INVALID_DATA);
	INT32OR64 val;
	uint8_t *ptr = (uint8_t*)&val;
	uint8_t i = 0;
	for (; i < buflen; i++)	{
		*(ptr + i) = *(p_buffer+i);
	}

	if(buflen < sizeof(INT32OR64)) {
		uint8_t pad = (*(ptr + i-1)&0x80)? 255 : 0;
		for(; i<sizeof(INT32OR64); i++) *(ptr + i) = pad;
	}
	r_variant = val;
	if (r_len) {
		(*r_len) += buflen;
	}
	return OK;
}

Error GodotUniversialSerializer::decode_variant(Variant &r_variant, const uint8_t *p_buffer, uint32_t p_len, uint32_t *r_len, bool p_allow_objects, int p_depth) {
	ERR_FAIL_COND_V_MSG(p_depth > Variant_MAX_RECURSION_DEPTH, ERR_OUT_OF_MEMORY, "Variant is too deep. Bailing.");
	const uint8_t *buf = p_buffer;
	uint32_t len = p_len;

	ERR_FAIL_COND_V(len < 1, ERR_INVALID_DATA);


	uint8_t type = decode_type_code(buf);

	buf ++;
	len --;

	if (r_len) {
		*r_len = 1;
	}

	// Note: We cannot use sizeof(real_t) for decoding, in case a different size is encoded.
	// Decoding math types always checks for the encoded size, while encoding always uses compilation setting.
	// This does lead to some code duplication for decoding, but compatibility is the priority.
	switch (type ) {
		case NULL_: {
			r_variant = Variant();
		}break;
		case FALSE_CAP:
		case FALSE_LOW:{
			r_variant = false;
		}break;
		case TRUE_CAP:
		case TRUE_LOW:{
			r_variant = true;
		}break;
		case INT_1:{
			decode_varlenth_int<int32_t>(1, buf, r_variant, len, r_len);
		} break;
		case INT_2:{
			decode_varlenth_int<int32_t>(2, buf, r_variant, len, r_len);
		} break;
		case INT_3:{
			decode_varlenth_int<int32_t>(3, buf, r_variant, len, r_len);
		} break;
		case INT_4:{
			decode_varlenth_int<int32_t>(4, buf, r_variant, len, r_len);
		} break;
		case INT_5:{
			decode_varlenth_int<int64_t>(5, buf, r_variant, len, r_len);
		} break;
		case INT_6:{
			decode_varlenth_int<int64_t>(6, buf, r_variant, len, r_len);
		} break;
		case INT_7:{
			decode_varlenth_int<int64_t>(7, buf, r_variant, len, r_len);
		} break;
		case INT_8:{
			decode_varlenth_int<int64_t>(8, buf, r_variant, len, r_len);
		} break;
		case FLOAT_4:{
			ERR_FAIL_COND_V((size_t)len < sizeof(float), ERR_INVALID_DATA);
			float val = decode_float(buf);
			r_variant = val;
			if (r_len) {
				(*r_len) += sizeof(float);
			}
		}break;
		case FLOAT_8:{
			ERR_FAIL_COND_V((size_t)len < sizeof(double), ERR_INVALID_DATA);
			double val = decode_double(buf);
			r_variant = val;
			if (r_len) {
				(*r_len) += sizeof(double);
			}
		}break;
		case STR: {
			String str;
			Error err = _decode_string_utf8_without_length(buf, len, r_len, str);
			if (err) {
				return err;
			}
			r_variant = str;
		} break;

		// math types
		case VEC2: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 2, ERR_INVALID_DATA);
			Vector2 val;
			if (sizeof(real_t) == sizeof(float)){
				val.x = decode_float(&buf[0]);
				val.y = decode_float(&buf[sizeof(float)]);
				if (r_len) {
					(*r_len) += sizeof(float) * 2;
				}
			}else{
				val.x = decode_double(&buf[0]);
				val.y = decode_double(&buf[sizeof(double)]);
				if (r_len) {
					(*r_len) += sizeof(double) * 2;
				}
			}

			r_variant = val;
		} break;
		case VEC2I: {
			ERR_FAIL_COND_V(len < 4 * 2, ERR_INVALID_DATA);
			Vector2i val;
			val.x = decode_uint32(&buf[0]);
			val.y = decode_uint32(&buf[4]);
			r_variant = val;

			if (r_len) {
				(*r_len) += 4 * 2;
			}

		} break;
		case RECT2: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 4, ERR_INVALID_DATA);
			Rect2 val;
			if (sizeof(real_t) == sizeof(float)){
				val.position.x = decode_float(&buf[0]);
				val.position.y = decode_float(&buf[sizeof(float)]);
				val.size.x = decode_float(&buf[sizeof(float) * 2]);
				val.size.y = decode_float(&buf[sizeof(float) * 3]);

				if (r_len) {
					(*r_len) += sizeof(float) * 4;
				}
			}else{
				val.position.x = decode_double(&buf[0]);
				val.position.y = decode_double(&buf[sizeof(double)]);
				val.size.x = decode_double(&buf[sizeof(double) * 2]);
				val.size.y = decode_double(&buf[sizeof(double) * 3]);

				if (r_len) {
					(*r_len) += sizeof(double) * 4;
				}
			}

			r_variant = val;

		} break;
		case RECT2I: {
			ERR_FAIL_COND_V(len < 4 * 4, ERR_INVALID_DATA);
			Rect2i val;
			val.position.x = decode_uint32(&buf[0]);
			val.position.y = decode_uint32(&buf[4]);
			val.size.x = decode_uint32(&buf[8]);
			val.size.y = decode_uint32(&buf[12]);
			r_variant = val;

			if (r_len) {
				(*r_len) += 4 * 4;
			}

		} break;
		case VEC3: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 3, ERR_INVALID_DATA);
			Vector3 val;

			if (sizeof(real_t) == sizeof(float)){
				val.x = decode_float(&buf[0]);
				val.y = decode_float(&buf[sizeof(float)]);
				val.z = decode_float(&buf[sizeof(float) * 2]);

				if (r_len) {
					(*r_len) += sizeof(float) * 3;
				}
			}else{
				val.x = decode_double(&buf[0]);
				val.y = decode_double(&buf[sizeof(double)]);
				val.z = decode_double(&buf[sizeof(double) * 2]);

				if (r_len) {
					(*r_len) += sizeof(double) * 3;
				}
			}
			r_variant = val;

		} break;
		case VEC3I: {
			ERR_FAIL_COND_V(len < 4 * 3, ERR_INVALID_DATA);
			Vector3i val;
			val.x = decode_uint32(&buf[0]);
			val.y = decode_uint32(&buf[4]);
			val.z = decode_uint32(&buf[8]);
			r_variant = val;

			if (r_len) {
				(*r_len) += 4 * 3;
			}

		} break;
		case TRANSFORM2D: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 6, ERR_INVALID_DATA);
			Transform2D val;

			if (sizeof(real_t) == sizeof(float)){
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 2; j++) {
						val.elements[i][j] = decode_float(&buf[(i * 2 + j) * sizeof(float)]);
					}
				}

				if (r_len) {
					(*r_len) += sizeof(float) * 6;
				}
			}else{
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 2; j++) {
						val.elements[i][j] = decode_double(&buf[(i * 2 + j) * sizeof(double)]);
					}
				}

				if (r_len) {
					(*r_len) += sizeof(double) * 6;
				}
			}

			r_variant = val;

		} break;
		case PLANE: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 4, ERR_INVALID_DATA);
			Plane val;
			if (sizeof(real_t) == sizeof(float)){
				val.normal.x = decode_float(&buf[0]);
				val.normal.y = decode_float(&buf[sizeof(float)]);
				val.normal.z = decode_float(&buf[sizeof(float) * 2]);
				val.d = decode_float(&buf[sizeof(float) * 3]);

				if (r_len) {
					(*r_len) += sizeof(float) * 4;
				}
			}else{
				val.normal.x = decode_double(&buf[0]);
				val.normal.y = decode_double(&buf[sizeof(double)]);
				val.normal.z = decode_double(&buf[sizeof(double) * 2]);
				val.d = decode_double(&buf[sizeof(double) * 3]);

				if (r_len) {
					(*r_len) += sizeof(double) * 4;
				}
			}
			r_variant = val;

		} break;
		case QUAT: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 4, ERR_INVALID_DATA);
			Quaternion val;

			if (sizeof(real_t) == sizeof(float)){
				val.x = decode_float(&buf[0]);
				val.y = decode_float(&buf[sizeof(float)]);
				val.z = decode_float(&buf[sizeof(float) * 2]);
				val.w = decode_float(&buf[sizeof(float) * 3]);

				if (r_len) {
					(*r_len) += sizeof(float) * 4;
				}
			}else{
				val.x = decode_double(&buf[0]);
				val.y = decode_double(&buf[sizeof(double)]);
				val.z = decode_double(&buf[sizeof(double) * 2]);
				val.w = decode_double(&buf[sizeof(double) * 3]);

				if (r_len) {
					(*r_len) += sizeof(double) * 4;
				}
			}
			r_variant = val;

		} break;
		case AABB_: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 6, ERR_INVALID_DATA);
			AABB val;

			if (sizeof(real_t) == sizeof(float)){
				val.position.x = decode_float(&buf[0]);
				val.position.y = decode_float(&buf[sizeof(float)]);
				val.position.z = decode_float(&buf[sizeof(float) * 2]);
				val.size.x = decode_float(&buf[sizeof(float) * 3]);
				val.size.y = decode_float(&buf[sizeof(float) * 4]);
				val.size.z = decode_float(&buf[sizeof(float) * 5]);

				if (r_len) {
					(*r_len) += sizeof(float) * 6;
				}
			}else{
				val.position.x = decode_double(&buf[0]);
				val.position.y = decode_double(&buf[sizeof(double)]);
				val.position.z = decode_double(&buf[sizeof(double) * 2]);
				val.size.x = decode_double(&buf[sizeof(double) * 3]);
				val.size.y = decode_double(&buf[sizeof(double) * 4]);
				val.size.z = decode_double(&buf[sizeof(double) * 5]);

				if (r_len) {
					(*r_len) += sizeof(double) * 6;
				}
			}

			r_variant = val;

		} break;
		case BASIS: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 9, ERR_INVALID_DATA);
			Basis val;

			if (sizeof(real_t) != sizeof(float)){
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						val.elements[i][j] = decode_double(&buf[(i * 3 + j) * sizeof(double)]);
					}
				}

				if (r_len) {
					(*r_len) += sizeof(double) * 9;
				}
			} else {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						val.elements[i][j] = decode_float(&buf[(i * 3 + j) * sizeof(float)]);
					}
				}

				if (r_len) {
					(*r_len) += sizeof(float) * 9;
				}
			}
			r_variant = val;

		} break;
		case TRANSFORM3D: {
			ERR_FAIL_COND_V((size_t)len < sizeof(real_t) * 12, ERR_INVALID_DATA);
			Transform3D val;
			if (sizeof(real_t) != sizeof(float)) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						val.basis.elements[i][j] = decode_double(&buf[(i * 3 + j) * sizeof(double)]);
					}
				}
				val.origin[0] = decode_double(&buf[sizeof(double) * 9]);
				val.origin[1] = decode_double(&buf[sizeof(double) * 10]);
				val.origin[2] = decode_double(&buf[sizeof(double) * 11]);

				if (r_len) {
					(*r_len) += sizeof(double) * 12;
				}
			} else {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						val.basis.elements[i][j] = decode_float(&buf[(i * 3 + j) * sizeof(float)]);
					}
				}
				val.origin[0] = decode_float(&buf[sizeof(float) * 9]);
				val.origin[1] = decode_float(&buf[sizeof(float) * 10]);
				val.origin[2] = decode_float(&buf[sizeof(float) * 11]);

				if (r_len) {
					(*r_len) += sizeof(float) * 12;
				}
			}
			r_variant = val;

		} break;
		// misc types
		case COLOR: {
			switch (colorEncoding){
				case ColorEncoding::RGBA32:{
					ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
					r_variant = Color::hex(decode_uint32(buf));

					if (r_len) {
						(*r_len) += 4; // Colors should always be in single-precision.
					}
				}break;
				case ColorEncoding::RGBA64:{
					ERR_FAIL_COND_V(len < 8, ERR_INVALID_DATA);
					r_variant =  Color::hex64(decode_uint64(buf));

					if (r_len) {
						(*r_len) += 8; // Colors should always be in single-precision.
					}
				}break;
				default:{
					ERR_FAIL_COND_V(len < 4 * 4, ERR_INVALID_DATA);
					Color val;
					val.r = decode_float(&buf[0]);
					val.g = decode_float(&buf[4]);
					val.b = decode_float(&buf[8]);
					val.a = decode_float(&buf[12]);
					r_variant = val;

					if (r_len) {
						(*r_len) += 4 * 4; // Colors should always be in single-precision.
					}
				}break;
			}

		} break;
		case STR_NAME: {
			String str;
			Error err = _decode_string_utf8_without_length(buf, len, r_len, str);
			if (err) {
				return err;
			}
			r_variant = StringName(str);

		} break;

		case NODE_PATH: {
			String str;
			Error err = _decode_string_utf8_without_length(buf, len, r_len, str);
			if (err) {
				return err;
			}
			r_variant = NodePath(str);
		} break;
		case DICT_BEGIN: {
			ERR_FAIL_COND_V(len < 1, ERR_INVALID_DATA);
			Dictionary d;
			while ( *buf != DICT_END ){
				Variant key, value;

				uint32_t used;
				Error err = decode_variant(key, buf, len, &used, p_allow_objects, p_depth + 1);
				ERR_FAIL_COND_V_MSG(err != OK, err, "Error when trying to decode Variant.");

				buf += used;
				len -= used;
				if (r_len) {
					(*r_len) += used;
				}

				err = decode_variant(value, buf, len, &used, p_allow_objects, p_depth + 1);
				ERR_FAIL_COND_V_MSG(err != OK, err, "Error when trying to decode Variant.");

				buf += used;
				len -= used;
				if (r_len) {
					(*r_len) += used;
				}

				d[key] = value;

				if (len <= 0){
					ERR_FAIL_COND_V_MSG(len <= 0, err, "Convert to Dictionary faild.");
					break;
				}
			}
			if (len >0 ){
				decode_type_code(buf);
				if (r_len) {
					(*r_len) += 1;
				}

				r_variant = d;
			}
		} break;
		case ARR_BEGIN: {
			ERR_FAIL_COND_V(len < 1, ERR_INVALID_DATA);

			Array varr;
			while ( *buf != ARR_END ){
				Variant element;

				uint32_t used;
				Error err = decode_variant(element, buf, len, &used, p_allow_objects, p_depth + 1);
				ERR_FAIL_COND_V_MSG(err != OK, err, "Error when trying to decode Variant.");

				buf += used;
				len -= used;
				if (r_len) {
					(*r_len) += used;
				}

				varr.push_back(element);

				if (len <= 0){
					ERR_FAIL_COND_V_MSG(len <= 0, err, "Convert to Array faild.");
					break;
				}
			}
			if (len > 0){
				decode_type_code(buf);
				if (r_len) {
					(*r_len) += 1;
				}

				r_variant = varr;
			}
		} break;

		// empty contaners
		case EMPTY_ARR: {
			r_variant = Array();
		} break;
		case EMPTY_DICT: {
			r_variant = Dictionary();
		} break;

		// arrays
		case RAW_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;
			ERR_FAIL_COND_V(count < 0 || count > len, ERR_INVALID_DATA);

			PackedByteArray data;

			if (count) {
				data.resize(count);
				uint8_t *w = data.ptrw();
				for (uint32_t i = 0; i < count; i++) {
					w[i] = buf[i];
				}
			}

			r_variant = data;

			if (r_len) {
				(*r_len) += 4 + count;
			}

		} break;
		case INT32_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;
			ERR_FAIL_MUL_OF(count, 4, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * 4 > len, ERR_INVALID_DATA);

			PackedInt32Array data;

			if (count) {
				//const int*rbuf=(const int*)buf;
				data.resize(count);
				int32_t *w = data.ptrw();
				for (uint32_t i = 0; i < count; i++) {
					w[i] = decode_uint32(&buf[i * 4]);
				}
			}
			r_variant = data;
			if (r_len) {
				(*r_len) += 4 + count * sizeof(int32_t);
			}

		} break;
		case INT64_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;
			ERR_FAIL_MUL_OF(count, 8, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * 8 > len, ERR_INVALID_DATA);

			PackedInt64Array data;

			if (count) {
				//const int*rbuf=(const int*)buf;
				data.resize(count);
				int64_t *w = data.ptrw();
				for (uint32_t i = 0; i < count; i++) {
					w[i] = decode_uint64(&buf[i * 8]);
				}
			}
			r_variant = data;
			if (r_len) {
				(*r_len) += 4 + count * sizeof(int64_t);
			}

		} break;
		case FLOAT32_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;
			ERR_FAIL_MUL_OF(count, 4, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * 4 > len, ERR_INVALID_DATA);

			PackedFloat32Array data;

			if (count) {
				//const float*rbuf=(const float*)buf;
				data.resize(count);
				float *w = data.ptrw();
				for (uint32_t i = 0; i < count; i++) {
					w[i] = decode_float(&buf[i * 4]);
				}
			}
			r_variant = data;

			if (r_len) {
				(*r_len) += 4 + count * sizeof(float);
			}

		} break;
		case FLOAT64_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;
			ERR_FAIL_MUL_OF(count, 8, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * 8 > len, ERR_INVALID_DATA);

			PackedFloat64Array data;

			if (count) {
				data.resize(count);
				double *w = data.ptrw();
				for (uint32_t i = 0; i < count; i++) {
					w[i] = decode_double(&buf[i * 8]);
				}
			}
			r_variant = data;

			if (r_len) {
				(*r_len) += 4 + count * sizeof(double);
			}

		} break;
		case STR_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);

			PackedStringArray strings;
			buf += 4;
			len -= 4;

			if (r_len) {
				(*r_len) += 4; // Size of count number.
			}

			for (uint32_t i = 0; i < count; i++) {
				String str;
				Error err = _decode_string_utf8_without_length(buf, len, r_len, str);
				if (err) {
					return err;
				}

				strings.push_back(str);
			}

			r_variant = strings;

		} break;
		case VEC2_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;

			PackedVector2Array varray;

			ERR_FAIL_MUL_OF(count, sizeof(real_t) * 2, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * sizeof(real_t) * 2 > (size_t)len, ERR_INVALID_DATA);
			if (sizeof(real_t) != sizeof(float)) {

				if (r_len) {
					(*r_len) += 4; // Size of count number.
				}

				if (count) {
					varray.resize(count);
					Vector2 *w = varray.ptrw();

					for (uint32_t i = 0; i < count; i++) {
						w[i].x = decode_double(buf + i * sizeof(double) * 2 + sizeof(double) * 0);
						w[i].y = decode_double(buf + i * sizeof(double) * 2 + sizeof(double) * 1);
					}

					int adv = sizeof(double) * 2 * count;

					if (r_len) {
						(*r_len) += adv;
					}
					len -= adv;
					buf += adv;
				}
			} else {

				if (r_len) {
					(*r_len) += 4; // Size of count number.
				}

				if (count) {
					varray.resize(count);
					Vector2 *w = varray.ptrw();

					for (uint32_t i = 0; i < count; i++) {
						w[i].x = decode_float(buf + i * sizeof(float) * 2 + sizeof(float) * 0);
						w[i].y = decode_float(buf + i * sizeof(float) * 2 + sizeof(float) * 1);
					}

					int adv = sizeof(float) * 2 * count;

					if (r_len) {
						(*r_len) += adv;
					}
				}
			}
			r_variant = varray;

		} break;
		case VEC3_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;

			PackedVector3Array varray;

			ERR_FAIL_MUL_OF(count, sizeof(real_t) * 3, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * sizeof(real_t) * 3 > (size_t)len, ERR_INVALID_DATA);
			if (sizeof(real_t) != sizeof(float)) {

				if (r_len) {
					(*r_len) += 4; // Size of count number.
				}

				if (count) {
					varray.resize(count);
					Vector3 *w = varray.ptrw();

					for (uint32_t i = 0; i < count; i++) {
						w[i].x = decode_double(buf + i * sizeof(double) * 3 + sizeof(double) * 0);
						w[i].y = decode_double(buf + i * sizeof(double) * 3 + sizeof(double) * 1);
						w[i].z = decode_double(buf + i * sizeof(double) * 3 + sizeof(double) * 2);
					}

					int adv = sizeof(double) * 3 * count;

					if (r_len) {
						(*r_len) += adv;
					}
					len -= adv;
					buf += adv;
				}
			} else {
				if (r_len) {
					(*r_len) += 4; // Size of count number.
				}

				if (count) {
					varray.resize(count);
					Vector3 *w = varray.ptrw();

					for (uint32_t i = 0; i < count; i++) {
						w[i].x = decode_float(buf + i * sizeof(float) * 3 + sizeof(float) * 0);
						w[i].y = decode_float(buf + i * sizeof(float) * 3 + sizeof(float) * 1);
						w[i].z = decode_float(buf + i * sizeof(float) * 3 + sizeof(float) * 2);
					}

					int adv = sizeof(float) * 3 * count;

					if (r_len) {
						(*r_len) += adv;
					}
					len -= adv;
					buf += adv;
				}
			}
			r_variant = varray;

		} break;
		case COLOR_ARR: {
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			uint32_t count = decode_uint32(buf);
			buf += 4;
			len -= 4;


			PackedColorArray carray;

			// if (r_len) {
			// 	(*r_len) += 4; // Size of count number.
			// }

			if (count) {
				carray.resize(count);
				Color *w = carray.ptrw();

				switch (colorEncoding){
					case ColorEncoding::RGBA32:{
						ERR_FAIL_MUL_OF(count, 4, ERR_INVALID_DATA);
						ERR_FAIL_COND_V(count < 0 || count * 4 > len, ERR_INVALID_DATA);
						for (uint32_t i = 0; i < count; i++) {
							w[i] = Color::hex(decode_uint32(buf + i *4));
						}
						if (r_len) {
							(*r_len) += 4+ 4 * count;
						}
						r_variant = carray;
					}break;
					case ColorEncoding::RGBA64:{
						ERR_FAIL_MUL_OF(count, 8, ERR_INVALID_DATA);
						ERR_FAIL_COND_V(count < 0 || count * 8 > len, ERR_INVALID_DATA);

						for (uint32_t i = 0; i < count; i++) {
							w[i] =  Color::hex64(decode_uint64(buf + i *8));
						}

						if (r_len) {
							(*r_len) += 4+8 * count;
						}
						r_variant = carray;
					}break;
					default:{
						ERR_FAIL_MUL_OF(count, 4 * 4, ERR_INVALID_DATA);
						ERR_FAIL_COND_V(count < 0 || count * 4 * 4 > len, ERR_INVALID_DATA);

						// Colors should always be in single-precision.
						for (uint32_t i = 0; i < count; i++) {
							w[i].r = decode_float(buf + i * 4 * 4 + 4 * 0);
							w[i].g = decode_float(buf + i * 4 * 4 + 4 * 1);
							w[i].b = decode_float(buf + i * 4 * 4 + 4 * 2);
							w[i].a = decode_float(buf + i * 4 * 4 + 4 * 3);
						}
						if (r_len) {
							(*r_len) += 4+ 4 * 4 * count; // Colors should always be in single-precision.
						}
						r_variant = carray;
					}break;
				}
			}

		} break;

		// empty arrays
		case EMPTY_RAW_ARR: {
			r_variant = PackedByteArray();
		} break;
		case EMPTY_INT32_ARR:{
			r_variant = PackedInt32Array();
		}break;
		case EMPTY_INT64_ARR:{
			r_variant = PackedInt64Array();
		}break;
		case EMPTY_FLOAT32_ARR:{
			r_variant = PackedFloat32Array();
		}break;
		case EMPTY_FLOAT64_ARR:{
			r_variant = PackedFloat64Array();
		}break;
		case EMPTY_STR_ARR:{
			r_variant = PackedStringArray();
		}break;
		case EMPTY_VEC2_ARR:{
			r_variant = PackedVector2Array();
		}break;
		case EMPTY_VEC3_ARR:{
			r_variant = PackedVector3Array();
		}break;
		case EMPTY_COLOR_ARR:{
			r_variant = PackedColorArray();
		}break;

		default: {
			ERR_FAIL_V_MSG(ERR_INVALID_DATA, "Unrecognized code: " + Variant(type).operator godot::String() + " -- The fed bytes may not a legal GUS data.");
		}
	}

	return OK;
}


void GodotUniversialSerializer::_encode_string_utf8_without_length(const String &p_string, uint8_t *&buf, uint32_t &r_len) {
	CharString utf8 = p_string.utf8();

	if (buf) {
		ERR_FAIL_COND(utf8.length() < 0);
		// encode_uint32(utf8.length(), buf);
		// buf += 4;
		ERR_FAIL_COND(!utf8.get_data());
		memcpy(buf, utf8.get_data(), utf8.length());
		buf += utf8.length();
	}

	r_len += utf8.length();
	// while (r_len % 4) {
	// 	r_len++; //pad
	// 	if (buf) {
	// 		*(buf++) = 0;
	// 	}
	// }
}
void GodotUniversialSerializer::_encode_string_ascii_without_length(const String &p_string, uint8_t *&buf, uint32_t &r_len) {
	CharString ascii = p_string.ascii();

	if (buf) {
		ERR_FAIL_COND(ascii.length() < 0);
		ERR_FAIL_COND(!ascii.get_data());
		memcpy(buf, ascii.get_data(), ascii.length());
		buf += ascii.length();
	}

	r_len += ascii.length();
}


Error GodotUniversialSerializer::encode_variant(const Variant &p_variant, uint8_t *r_buffer, uint32_t &r_len, bool p_full_objects, int p_depth) {
	ERR_FAIL_COND_V_MSG(p_depth > Variant_MAX_RECURSION_DEPTH, ERR_OUT_OF_MEMORY, "Potential infinite recursion detected. Bailing.");
	uint8_t *buf = r_buffer;

	r_len = 0;

	switch (p_variant.get_type()) {
		case Variant::NIL: {
			if (buf)
				encode_type_code(NULL_, buf);
			r_len ++;
		} break;
		case Variant::BOOL: {
			if (buf){
				if (p_variant.operator bool()) encode_type_code(TRUE_CAP, buf);
				else encode_type_code(FALSE_CAP, buf);
			}

			r_len ++;
		} break;
		case Variant::INT: {
			int64_t val = p_variant.operator int64_t();
			uint8_t *ptr = (uint8_t*)&val;
			uint8_t extra_size = 7;

			if (val < 0) { // negetive
				for (; extra_size >0; extra_size--)
					if ((*(ptr+extra_size)) != 255)
						break;
				if (!((*(ptr+extra_size)) & 0x80)) extra_size++;
			}else{ // positive
				for (; extra_size > 0; extra_size--){

					if ((*(ptr+extra_size)) != 0)
						break;
				}
				if ((*(ptr+extra_size)) & 0x80) extra_size++;
			}
			r_len += 1 + (1 + extra_size);
			// 写入
			if(buf){
				encode_type_code( INT_1 + extra_size, buf);
				*(buf+1) = *ptr;
				ptr++;
				buf+=2;
				for (uint8_t i=0; i < extra_size; i++){
					*(buf+i) = *(ptr+i);
				}
			}
		} break;
		case Variant::FLOAT: {
			double real64 = p_variant.operator double();
			float real32 = real64;
			if (double(real32) != real64) {
				if (buf){
					encode_type_code(FLOAT_8, &buf[0]);
					encode_double(p_variant.operator double(), &buf[1]);
				}
				r_len += (1 + 8);

			} else {
				if (buf){
					encode_type_code(FLOAT_4, &buf[0]);
					encode_float(p_variant.operator float(), &buf[1]);
				}
				r_len += (1 + 4);
			}

		} break;
		case Variant::NODE_PATH: {
			if (buf){
				encode_type_code(NODE_PATH, buf);
			}

			auto buf_next = (buf)? &buf[1] : nullptr;

			if(nodePathEncodingUtf8){
				_encode_string_utf8_without_length(p_variant.operator godot::NodePath(), buf_next , r_len);
			}else{
				_encode_string_ascii_without_length(p_variant.operator godot::NodePath(), buf_next , r_len);
			}
			r_len ++;
		} break;
		case Variant::STRING:{
			if (buf){
				encode_type_code(STR, buf);
			}

			auto buf_next = (buf)? &buf[1] : nullptr;
			if(stringEncodingUtf8){
				_encode_string_utf8_without_length(p_variant, buf_next , r_len);
			}else{
				_encode_string_ascii_without_length(p_variant, buf_next , r_len);
			}
			r_len ++;
		} break;
		case Variant::STRING_NAME: {
			if (buf){
				encode_type_code(STR_NAME, buf);
			}

			auto buf_next = (buf)? &buf[1] : nullptr;
			if(stringEncodingUtf8){
				_encode_string_utf8_without_length(p_variant.operator godot::StringName(), buf_next , r_len);
			}else{
				_encode_string_ascii_without_length(p_variant.operator godot::StringName(), buf_next , r_len);
			}
			r_len ++;
		} break;

		// math types
		case Variant::VECTOR2: {
			if (buf){
				encode_type_code(VEC2, buf);
				Vector2 v2 = p_variant.operator godot::Vector2();
				encode_real(v2.x, &buf[1]);
				encode_real(v2.y, &buf[1+sizeof(real_t)]);
			}
			r_len += 1 + 2 * sizeof(real_t);
		} break;
		case Variant::VECTOR2I: {
			if (buf){
				encode_type_code(VEC2I, buf);
				Vector2i v2i = p_variant.operator godot::Vector2i();
				encode_uint32(v2i.x, &buf[1]);
				encode_uint32(v2i.y, &buf[1 + sizeof(int32_t)]);
			}
			r_len += 1 + 2 * sizeof(int32_t);
		} break;
		case Variant::RECT2: {
			if (buf){
				encode_type_code(RECT2, buf);
				Rect2 r2 = p_variant.operator godot::Rect2();
				encode_real(r2.position.x, &buf[1]);
				encode_real(r2.position.y, &buf[1+sizeof(real_t)]);
				encode_real(r2.size.x, &buf[1+sizeof(real_t) * 2]);
				encode_real(r2.size.y, &buf[1+sizeof(real_t) * 3]);
			}
			r_len += 1+ 4 * sizeof(real_t);
		} break;
		case Variant::RECT2I: {
			if (buf){
				encode_type_code(RECT2I, buf);
				Rect2i r2i = p_variant.operator godot::Rect2i();
				encode_uint32(r2i.position.x, &buf[1]);
				encode_uint32(r2i.position.y, &buf[1+sizeof(int32_t)]);
				encode_uint32(r2i.size.x, &buf[1+2 * sizeof(int32_t)]);
				encode_uint32(r2i.size.y, &buf[1+3 * sizeof(int32_t)]);
			}
			r_len += 1+4 * sizeof(int32_t);
		} break;
		case Variant::VECTOR3: {
			if (buf){
				encode_type_code(VEC3, buf);
				Vector3 v3 = p_variant.operator godot::Vector3();
				encode_real(v3.x, &buf[1]);
				encode_real(v3.y, &buf[1+sizeof(real_t)]);
				encode_real(v3.z, &buf[1+sizeof(real_t) * 2]);
			}
			r_len += 1+3 * sizeof(real_t);
		} break;
		case Variant::VECTOR3I: {
			if (buf){
				encode_type_code(VEC3I, buf);
				Vector3i v3i = p_variant.operator godot::Vector3i();
				encode_uint32(v3i.x, &buf[1]);
				encode_uint32(v3i.y, &buf[1+sizeof(int32_t)]);
				encode_uint32(v3i.z, &buf[1+2 * sizeof(int32_t)]);
			}
			r_len += 1+3 * sizeof(int32_t);
		} break;
		case Variant::TRANSFORM2D: {
			if (buf){
				encode_type_code(TRANSFORM2D, buf);
				Transform2D val = p_variant;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 2; j++) {
						memcpy(&buf[1+(i * 2 + j) * sizeof(real_t)], &val.elements[i][j], sizeof(real_t));
					}
				}
			}
			r_len += 1 + 6 * sizeof(real_t);
		} break;
		case Variant::PLANE: {
			if (buf){
				encode_type_code(PLANE, buf);
				Plane p = p_variant;
				encode_real(p.normal.x, &buf[1]);
				encode_real(p.normal.y, &buf[1+sizeof(real_t)]);
				encode_real(p.normal.z, &buf[1+sizeof(real_t) * 2]);
				encode_real(p.d, &buf[1+sizeof(real_t) * 3]);
			}
			r_len += 1 + 4 * sizeof(real_t);
		} break;
		case Variant::QUATERNION: {
			if (buf){
				encode_type_code(QUAT, buf);
				Quaternion q = p_variant;
				encode_real(q.x, &buf[1]);
				encode_real(q.y, &buf[1+sizeof(real_t)]);
				encode_real(q.z, &buf[1+sizeof(real_t) * 2]);
				encode_real(q.w, &buf[1+sizeof(real_t) * 3]);
			}
			r_len += 1 + 4 * sizeof(real_t);
		} break;
		case Variant::AABB: {
			if (buf){
				encode_type_code(AABB_, buf);
				AABB aabb = p_variant;
				encode_real(aabb.position.x, &buf[1]);
				encode_real(aabb.position.y, &buf[1+sizeof(real_t)]);
				encode_real(aabb.position.z, &buf[1+sizeof(real_t) * 2]);
				encode_real(aabb.size.x, &buf[1+sizeof(real_t) * 3]);
				encode_real(aabb.size.y, &buf[1+sizeof(real_t) * 4]);
				encode_real(aabb.size.z, &buf[1+sizeof(real_t) * 5]);
			}
			r_len += 1+ 6 * sizeof(real_t);
		} break;
		case Variant::BASIS: {
			if (buf){
				encode_type_code(BASIS, buf);
				Basis val = p_variant;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						memcpy(&buf[1+(i * 3 + j) * sizeof(real_t)], &val.elements[i][j], sizeof(real_t));
					}
				}
			}
			r_len +=1 + 9 * sizeof(real_t);

		} break;
		case Variant::TRANSFORM3D: {
			if (buf){
				encode_type_code(TRANSFORM3D, buf);
				Transform3D val = p_variant;

				buf += 1;

				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						memcpy(&buf[(i * 3 + j) * sizeof(real_t)], &val.basis.elements[i][j], sizeof(real_t));
					}
				}

				encode_real(val.origin.x, &buf[sizeof(real_t) * 9]);
				encode_real(val.origin.y, &buf[sizeof(real_t) * 10]);
				encode_real(val.origin.z, &buf[sizeof(real_t) * 11]);
			}
			r_len += 1 + 12 * sizeof(real_t);
		} break;

		// misc types
		case Variant::COLOR: {
			switch (colorEncoding){
				case ColorEncoding::RGBA32:{
					if (buf){
						encode_type_code(COLOR, buf);
						encode_uint32(p_variant.operator godot::Color().to_rgba32(), &buf[1]);
					}
					r_len += 1 + sizeof(uint32_t);
				}break;
				case ColorEncoding::RGBA64:{
					if (buf){
						encode_type_code(COLOR, buf);
						encode_uint64(p_variant.operator godot::Color().to_rgba64(), &buf[1]);
					}
					r_len += 1 + sizeof(uint64_t);
				}break;
				default:{
					if (buf){
						encode_type_code(COLOR, buf);
						Color c = p_variant;
						encode_float(c.r, &buf[1]);
						encode_float(c.g, &buf[1+4]);
						encode_float(c.b, &buf[1+8]);
						encode_float(c.a, &buf[1+12]);
					}
					r_len += 1 + 4 * sizeof(float); // Colors should always be in single-precision.
				}break;
			}
		} break;
		case Variant::DICTIONARY: {
			Dictionary d = p_variant;

			r_len ++;

			if ( !d.size()){
				encode_type_code(EMPTY_DICT, buf);
				break;
			}

			if(buf) {
				encode_type_code(DICT_BEGIN, buf);
				buf ++;
			}


			auto keys = d.keys();
			for (uint32_t i =0 ; i < keys.size(); i++){
				uint32_t len;
				Error err = encode_variant(keys[i], buf, len, p_full_objects, p_depth + 1);
				ERR_FAIL_COND_V(err, err);
				// ERR_FAIL_COND_V(len % 4, ERR_BUG);
				r_len += len;
				if(buf) buf += len;
				err = encode_variant(d[keys[i]], buf, len, p_full_objects, p_depth + 1);
				ERR_FAIL_COND_V(err, err);
				// ERR_FAIL_COND_V(len % 4, ERR_BUG);
				r_len += len;
				if(buf) buf += len;
			}

			if(buf) encode_type_code(DICT_END, buf);
			r_len ++;
		} break;
		case Variant::ARRAY: {
			Array v = p_variant;
			r_len ++;

			if (!v.size()){
				encode_type_code(EMPTY_ARR, buf);
				break;
			}
			// type
			if(buf){
				encode_type_code(ARR_BEGIN, buf);
				buf ++;
			}


			for (uint32_t i = 0; i < v.size(); i++) {
				uint32_t len;
				Error err = encode_variant(v[i], buf, len, p_full_objects, p_depth + 1);
				ERR_FAIL_COND_V(err, err);
				r_len += len;
				if(buf) buf += len;
			}

			r_len ++;
			// end code
			if(buf) {
				*buf = ARR_END;
			}
		} break;
		// arrays
		case Variant::PACKED_BYTE_ARRAY: {
			// length
			PackedByteArray data = p_variant;
			uint32_t datalen = data.size();
			if (!datalen){
				encode_type_code(EMPTY_RAW_ARR, buf);
				r_len ++;
				break;
			}
			// type
			if(buf) {
				encode_type_code(RAW_ARR, buf);
				encode_uint32(datalen, &buf[1]);
				// datas
				buf += (1 + sizeof(uint32_t));
				const uint8_t *r = data.ptr();
				memcpy(buf, &r[0], datalen * sizeof(uint8_t));
				buf += datalen * sizeof(uint8_t);
			}
			r_len += 1 + sizeof(uint32_t) + datalen * sizeof(uint8_t);

		} break;
		case Variant::PACKED_INT32_ARRAY: {
			PackedInt32Array data = p_variant;
			uint32_t datalen = data.size();
			if (!datalen){
				encode_type_code(EMPTY_INT32_ARR, buf);
				r_len++;
				break;
			}
			if(buf){
				encode_type_code(INT32_ARR, buf);
				encode_uint32(datalen, &buf[1]);
				buf += (1 + 4);

				const int32_t *r = data.ptr();
				for (uint32_t i = 0; i < datalen; i++) {
					encode_uint32(r[i], &buf[i * sizeof(int32_t)]);
				}
			}
			r_len += 1 + sizeof(uint32_t) + datalen * sizeof(int32_t);
		} break;
		case Variant::PACKED_INT64_ARRAY: {
			PackedInt64Array data = p_variant;
			uint32_t datalen = data.size();
			if (!datalen){
				encode_type_code(EMPTY_INT64_ARR, buf);
				r_len++;
				break;
			}
			if(buf){
				encode_type_code(INT64_ARR, buf);
				encode_uint32(datalen, &buf[1]);
				buf += (1 + 4);

				const int64_t *r = data.ptr();
				for (uint32_t i = 0; i < datalen; i++) {
					encode_uint64(r[i], &buf[i * sizeof(int64_t)]);
				}
			}
			r_len += 1 + sizeof(uint32_t) + datalen * sizeof(int64_t);

		} break;
		case Variant::PACKED_FLOAT32_ARRAY: {
			PackedFloat32Array data = p_variant;
			uint32_t datalen = data.size();
			if (!datalen){
				encode_type_code(EMPTY_FLOAT32_ARR, buf);
				r_len++;
				break;
			}
			if(buf){
				encode_type_code(FLOAT32_ARR, buf);
				encode_uint32(datalen, &buf[1]);
				buf += (1 + 4);

				const float *r = data.ptr();
				for (uint32_t i = 0; i < datalen; i++) {
					encode_float(r[i], &buf[i * sizeof(float)]);
				}
			}
			r_len += 1 + sizeof(uint32_t) + datalen * sizeof(float);

		} break;
		case Variant::PACKED_FLOAT64_ARRAY: {
			PackedFloat64Array data = p_variant;
			uint32_t datalen = data.size();
			if (!datalen){
				encode_type_code(EMPTY_FLOAT64_ARR, buf);
				r_len++;
				break;
			}
			if(buf){
				encode_type_code(FLOAT64_ARR, buf);
				encode_uint32(datalen, &buf[1]);
				buf += (1 + 4);

				const double *r = data.ptr();
				for (uint32_t i = 0; i < datalen; i++) {
					encode_double(r[i], &buf[i * sizeof(double)]);
				}
			}
			r_len += 1 + sizeof(uint32_t) + datalen * sizeof(double);

		} break;
		case Variant::PACKED_STRING_ARRAY: {
			PackedStringArray data = p_variant;
			uint32_t datalen = data.size();
			if (!datalen){
				encode_type_code(EMPTY_STR_ARR, buf);
				r_len++;
				break;
			}

			if(buf){
				encode_type_code(STR_ARR, buf);
				encode_uint32(datalen, &buf[1]);
				buf += (1 + 4);
			}
			r_len += 1+4;

			if(stringEncodingUtf8){
				for (uint32_t i = 0; i < datalen; i++) {
					CharString utf8 = data[i].utf8();

					if (buf) {
						memcpy(buf, utf8.get_data(), utf8.length());
						buf += utf8.length() ;
					}

					r_len += utf8.length() ;
				}
			}else{
				for (uint32_t i = 0; i < datalen; i++) {
					CharString ascii = data[i].ascii();

					if (buf) {
						memcpy(buf, ascii.get_data(), ascii.length());
						buf += ascii.length() ;
					}

					r_len += ascii.length() ;
				}
			}


		} break;
		case Variant::PACKED_VECTOR2_ARRAY: {
			PackedVector2Array data = p_variant;
			uint32_t len = data.size();
			if (!len){
				encode_type_code(EMPTY_VEC2_ARR, buf);
				r_len++;
				break;
			}
			if(buf){
				encode_type_code(VEC2_ARR, buf);
				encode_uint32(len, &buf[1]);
				buf += (1 + 4);

				for (uint32_t i = 0; i < len; i++) {
					Vector2 v = data[i];
					encode_real(v.x, &buf[0]);
					encode_real(v.y, &buf[sizeof(real_t)]);
					buf += sizeof(real_t) * 2;
				}
			}
			r_len += 1+4 +sizeof(real_t) * 2 * len;

		} break;
		case Variant::PACKED_VECTOR3_ARRAY: {
			PackedVector3Array data = p_variant;
			uint32_t len = data.size();
			if (!len){
				encode_type_code(EMPTY_VEC3_ARR, buf);
				r_len++;
				break;
			}
			if(buf){
				encode_type_code(VEC3_ARR, buf);
				encode_uint32(len, &buf[1]);
				buf += (1 + 4);

				for (uint32_t i = 0; i < len; i++) {
					Vector3 v = data[i];

					encode_real(v.x, &buf[0]);
					encode_real(v.y, &buf[sizeof(real_t)]);
					encode_real(v.z, &buf[sizeof(real_t) * 2]);
					buf += sizeof(real_t) * 3;
				}
			}
			r_len += 1+4 +sizeof(real_t) * 3 * len;

		} break;
		case Variant::PACKED_COLOR_ARRAY: {
			PackedColorArray data = p_variant;
			uint32_t len = data.size();
			if (!len){
				encode_type_code(EMPTY_COLOR_ARR, buf);
				r_len++;
				break;
			}

			switch (colorEncoding){
				case ColorEncoding::RGBA32:{
					if (buf){
						encode_type_code(COLOR_ARR, buf);
						encode_uint32(len, &buf[1]);
						buf += (1 + 4);

						for (uint32_t i = 0; i < len; i++)
							encode_uint32(data[i].to_rgba32(), &buf[i]);
					}
					r_len += 1 +4+ sizeof(uint32_t) *len;
				}break;
				case ColorEncoding::RGBA64:{
					if (buf){
						encode_type_code(COLOR_ARR, buf);
						encode_uint32(len, &buf[1]);
						buf += (1 + 4);

						for (uint32_t i = 0; i < len; i++) {
							encode_uint64(data[i].to_rgba64(), buf);
							buf += sizeof(uint64_t);
						}
					}
					r_len += 1 +4+ sizeof(uint64_t) * len;
				}break;
				default:{
					if (buf){
						encode_type_code(COLOR_ARR, buf);
						encode_uint32(len, &buf[1]);
						buf += 1 + 4;

						for (uint32_t i = 0; i < len; i++) {
							Color c = data[i];
							encode_float(c.r, buf);
							encode_float(c.g, &buf[sizeof(float)]);
							encode_float(c.b, &buf[sizeof(float) * 2]);
							encode_float(c.a, &buf[sizeof(float) * 3]);
							buf += 4 * 4;
						}
					}
					r_len += 1 +4+ 4 * sizeof(float) * len; // Colors should always be in single-precision.
				}break;
			}

		} break;
		default: {
			ERR_FAIL_V_MSG(ERR_INVALID_DATA,"Unsupport Variant Type: "+Variant::get_type_name(p_variant.get_type()));
		}break;
	}

	return OK;
}


#endif