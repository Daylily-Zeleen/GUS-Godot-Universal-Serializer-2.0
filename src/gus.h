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
 */
#ifndef GODOT_UNIVERSAL_SERIALIZER_H
#define GODOT_UNIVERSAL_SERIALIZER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/wrapped.hpp>

// #define REAL_T_IS_DOUBLE

#include "dserializer.gdany.hpp"

/*
	类型化数组特化
	对象相关类型
*/

class GUS : public Object {
	GDCLASS(GUS, Object)
protected:
	static void _bind_methods() {
		godot::ClassDB::bind_static_method("GUS", D_METHOD("get_color_encode_type"), GUS::get_color_encode_type);
		godot::ClassDB::bind_static_method("GUS", D_METHOD("set_color_encode_type", "color_encode_type"), GUS::set_color_encode_type, DEFVAL(ColorEncodeType::COLOR_ENCODE_TYPE_RAW));
		
		godot::ClassDB::bind_static_method("GUS", D_METHOD("var_to_bytes", "var"), GUS::var_to_bytes);
		godot::ClassDB::bind_static_method("GUS", D_METHOD("bytes_to_var", "bytes"), GUS::bytes_to_var);

		BIND_ENUM_CONSTANT(COLOR_ENCODE_TYPE_RAW);
		BIND_ENUM_CONSTANT(COLOR_ENCODE_TYPE_HEX);
		BIND_ENUM_CONSTANT(COLOR_ENCODE_TYPE_HEX64);
	}

public:
	enum ColorEncodeType {
		COLOR_ENCODE_TYPE_RAW, // float, float, float, float
		COLOR_ENCODE_TYPE_HEX, // uint32_t
		COLOR_ENCODE_TYPE_HEX64, // uint64_t
	};
	static ColorEncodeType get_color_encode_type(){return _color_encode_type;}
	static void set_color_encode_type(ColorEncodeType color_encode_type = COLOR_ENCODE_TYPE_HEX) {
		_color_encode_type = color_encode_type;
		switch (_color_encode_type) {
			case COLOR_ENCODE_TYPE_RAW: {
				dserializer::color_encode_code = dserializer::DType::COLOR;
				dserializer::color_arr_encode_code = dserializer::DType::PACKED_COLOR_ARRAY;
				dserializer::cal_size_color = &dserializer::cal_size_color_raw<int64_t>;
				dserializer::encode_color = &dserializer::encode_color_raw;
				dserializer::cal_size_color_arr = &dserializer::cal_size_color_arr_raw<int64_t>;
				dserializer::encode_color_arr = &dserializer::encode_color_arr_raw;
			} break;
			case COLOR_ENCODE_TYPE_HEX: {
				dserializer::color_encode_code = dserializer::DType::COLOR_HEX;
				dserializer::color_arr_encode_code = dserializer::DType::PACKED_COLOR_ARRAY_HEX;
				dserializer::cal_size_color = &dserializer::cal_size_color_hex<int64_t>;
				dserializer::encode_color = &dserializer::encode_color_hex;
				dserializer::cal_size_color_arr = &dserializer::cal_size_color_arr_hex<int64_t>;
				dserializer::encode_color_arr = &dserializer::encode_color_arr_hex;
			} break;
			case COLOR_ENCODE_TYPE_HEX64: {
				dserializer::color_encode_code = dserializer::DType::COLOR_HEX64;
				dserializer::color_arr_encode_code = dserializer::DType::PACKED_COLOR_ARRAY_HEX64;
				dserializer::cal_size_color = &dserializer::cal_size_color_hex64<int64_t>;
				dserializer::encode_color = &dserializer::encode_color_hex64;
				dserializer::cal_size_color_arr = &dserializer::cal_size_color_arr_hex64<int64_t>;
				dserializer::encode_color_arr = &dserializer::encode_color_arr_hex64;
			} break;

			default:
				break;
		}
	}

	static godot::PackedByteArray var_to_bytes(const godot::Variant &var) {
		PackedByteArray r;
		decltype(r.size()) size = 0;
		dserializer::cal_size(var, size);
		r.resize(size);
		buffer_ptr buf = r.ptrw();
		dserializer::encode(buf, var);
		return r;
	}
	static godot::Variant bytes_to_var(const godot::PackedByteArray &bytes) {
		Variant r;
		auto addr = uint64_t(bytes.ptr());
		auto buf = (uint8_t *)addr;
		dserializer::decode(buf, r);
		return r;
	}

private:
	static ColorEncodeType _color_encode_type;
};

GUS::ColorEncodeType GUS::_color_encode_type = GUS::COLOR_ENCODE_TYPE_RAW;

VARIANT_ENUM_CAST(GUS::ColorEncodeType);

#endif