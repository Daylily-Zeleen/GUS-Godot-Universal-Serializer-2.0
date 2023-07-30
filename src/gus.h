/**************************************************************************/
/*  gus.h                                                                 */
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

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <type_traits>

#include "dserializer.gdany.hpp"

using namespace godot;

class GUS : public Object {
	GDCLASS(GUS, Object)
protected:
	static void _bind_methods() {
		ClassDB::bind_static_method(get_class_static(), D_METHOD("get_color_encode_type"), GUS::get_color_encode_type);
		ClassDB::bind_static_method(get_class_static(), D_METHOD("set_color_encode_type", "color_encode_type"), GUS::set_color_encode_type, DEFVAL(ColorEncodeType::COLOR_ENCODE_TYPE_RAW));

		ClassDB::bind_static_method(get_class_static(), D_METHOD("is_varint_incoding_in_packed_array"), GUS::is_varint_incoding_in_packed_array);
		ClassDB::bind_static_method(get_class_static(), D_METHOD("set_varint_incoding_in_packed_array", "enabled"), GUS::set_varint_incoding_in_packed_array);

		ClassDB::bind_static_method(get_class_static(), D_METHOD("var_to_bytes", "var"), GUS::var_to_bytes);
		ClassDB::bind_static_method(get_class_static(), D_METHOD("bytes_to_var", "bytes"), GUS::bytes_to_var);

		ClassDB::bind_static_method(get_class_static(), D_METHOD("is_big_endin_device"), GUS::is_big_endin_device);

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
	static ColorEncodeType get_color_encode_type() { return color_encode_type; }
	static void set_color_encode_type(ColorEncodeType p_color_encode_type = COLOR_ENCODE_TYPE_HEX) {
		color_encode_type = p_color_encode_type;
		switch (color_encode_type) {
			case COLOR_ENCODE_TYPE_RAW: {
				dserializer::color_encode_code = dserializer::DType::COLOR;
				dserializer::color_arr_encode_code = dserializer::DType::PACKED_COLOR_ARRAY;
			} break;
			case COLOR_ENCODE_TYPE_HEX: {
				dserializer::color_encode_code = dserializer::DType::COLOR_HEX;
				dserializer::color_arr_encode_code = dserializer::DType::PACKED_COLOR_ARRAY_HEX;
			} break;
			case COLOR_ENCODE_TYPE_HEX64: {
				dserializer::color_encode_code = dserializer::DType::COLOR_HEX64;
				dserializer::color_arr_encode_code = dserializer::DType::PACKED_COLOR_ARRAY_HEX64;
			} break;

			default:
				break;
		}
	}
	static void set_varint_incoding_in_packed_array(bool p_enabled) {
		dserializer::varint_encoding_in_packed_array = p_enabled;
	}
	static bool is_varint_incoding_in_packed_array() {
		return dserializer::varint_encoding_in_packed_array;
	}

	static godot::PackedByteArray var_to_bytes(const godot::Variant &p_var) {
		return (*var_to_bytes_ptr)(p_var);
	}

	static godot::Variant bytes_to_var(const godot::PackedByteArray &p_bytes) {
		return (*bytes_to_var_ptr)(p_bytes);
	}

	static bool is_big_endin_device() { return big_endin; }

	static void initialize() {
		int i = 1;
		big_endin = (*(char *)&i == 0);
		if (big_endin) {
			var_to_bytes_ptr = &_var_to_bytes<false>;
			bytes_to_var_ptr = &_bytes_to_var<false>;
		} else {
			var_to_bytes_ptr = &_var_to_bytes<true>;
			bytes_to_var_ptr = &_bytes_to_var<true>;
		}
	}

private:
	static ColorEncodeType color_encode_type;
	static decltype(&var_to_bytes) var_to_bytes_ptr;
	static decltype(&bytes_to_var) bytes_to_var_ptr;
	static bool big_endin;

	template <bool little_endin>
	static godot::PackedByteArray _var_to_bytes(const godot::Variant &p_var) {
		PackedByteArray r;
		decltype_pure(r.size()) size = 0;
		dserializer::cal_size<little_endin>(p_var, size);
		r.resize(size);
		auto *buf = r.ptrw();
		dserializer::encode<little_endin>(buf, p_var);
		return r;
	}

	template <bool little_endin>
	static godot::Variant _bytes_to_var(const godot::PackedByteArray &p_bytes) {
		using const_buf_t = decltype_pure(p_bytes.ptr());
		using buf_t = std::remove_const_t<std::remove_pointer_t<const_buf_t>>;
		auto buf = const_cast<buf_t *>(p_bytes.ptr());

		Variant r;
		dserializer::decode<little_endin>(buf, r);
		return r;
	}
};

GUS::ColorEncodeType GUS::color_encode_type = GUS::COLOR_ENCODE_TYPE_RAW;
decltype(&GUS::var_to_bytes) GUS::var_to_bytes_ptr = nullptr;
decltype(&GUS::bytes_to_var) GUS::bytes_to_var_ptr = nullptr;
bool GUS::big_endin = false;

VARIANT_ENUM_CAST(GUS::ColorEncodeType);
