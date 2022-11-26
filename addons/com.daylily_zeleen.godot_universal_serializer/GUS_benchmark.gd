@tool
extends EditorScript

func _run() -> void:
	randomize()
	GUS.set_color_encode_type(GUS.COLOR_ENCODE_TYPE_RAW)
#	_print_result("ignore", null)
	
	_test(get_editor_interface())
	
	
var tree:SceneTree
func _test(inside_tree_node:Node)->void:
	tree = inside_tree_node.get_tree() if inside_tree_node and inside_tree_node.is_inside_tree() else null
	_print_result("null" , null)
	_print_result("bool" , true)
	_print_result("bool" , false)
	_print_result("int1" , 0x7a)
	_print_result("int2" , 0x5c3b)
	_print_result("int3" , -0x7a5390)
	_print_result("int4" , 0x6a53c904)
	_print_result("int5" , -0x7c340c9041)
	_print_result("int6" , 0x5a53c904d054)
	_print_result("int7" , 0x3c90a53c90d041)
	_print_result("int8" , -(0x7aaa53c907a414d0))
	_print_result("float" , -105.0532)
	_print_result("String" , "test 测试")
	_print_result("StringName" , StringName("TestStringName"))
	_print_result("Vector2" , Vector2(-423, 46.8005))
	_print_result("Vector2i" , Vector2i(-423, 0x5c3b))
	_print_result("Vector3" , Vector3(52011,-541.3327,77441))
	_print_result("Vector3i" , Vector3i(-423,0x5c3b,52011))
	_print_result("Vector4" , Vector4(5211,-541.327,7441, 1.23))
	_print_result("Vector4i" , Vector4i(-42,0xc31b,5211,445))
	_print_result("Projection" , Projection.create_light_atlas_rect(Rect2(56.622, -77.85, 8740.2369, 441.044)))
	_print_result("Rect2" , Rect2(56.622, -77.85, 8740.2369, 441.044))
	_print_result("Rect2i" , Rect2i(randi(), randi(), randi(), randi()))
	_print_result("Transform2D" , Transform2D.FLIP_Y.translated(Vector2(513,88.5)))
	_print_result("Plane", Plane(-54.55, 876, 2310,744).normalized())
	_print_result("Quaternion", Quaternion(-54.55, 876, 2310,744))
	_print_result("AABB", AABB(Vector3(-77.85, 8740.2369, 441.044),Vector3(105,50,68)))
	_print_result("Basis", Transform3D.FLIP_Y.translated(Vector3(513,-88.5,105)).basis)
	_print_result("Transform3D", Transform3D.FLIP_Y.translated(Vector3(513,-88.5,105)))
	_print_result("Color", Color.WHITE)
	_print_result("NodePath", inside_tree_node.get_path())
	var arr = [574, -21.0, Vector2(-423,46.8005), Color.AQUA, "acxx"]
	_print_result("Array", arr )
	# 字典测试
	var dict :={
		k_str = "value1",
		k_color = Color.ALICE_BLUE,
		k_int = 123123,
		k_float = 44.886,
		k_vec3 = Vector3(52011,-541.3327,77441)
	}
	dict[532] = [574,-21.0, Vector2(-423,46.8005), Color.AQUA,"acxx"]
	_print_result("Dictionary",dict)
	
	# 池化数组测试
	var byte_arr := PackedByteArray()
	var int32_arr := PackedInt32Array()
	var int64_arr := PackedInt64Array()
	var float32_arr:= PackedFloat32Array()
	var float64_arr:= PackedFloat64Array()
	var string_arr := PackedStringArray()
	var vector2_arr := PackedVector2Array()
	var vector3_arr:= PackedVector3Array()
	var color_arr:= PackedColorArray()
	
	_print_result("Empty Array", [])
	_print_result("Empty Dictionary", {})	
	_print_result("Empty PackedByteArray",byte_arr)
	_print_result("Empty PackedInt32Array",int32_arr)
	_print_result("Empty PackedInt64Array",int64_arr)
	_print_result("Empty PackedFloat32Array",float32_arr)
	_print_result("Empty PackedFloat64Array",float64_arr)
	_print_result("Empty PackedStringArray",string_arr)
	_print_result("Empty PackedVector2Array",vector2_arr)
	_print_result("Empty PackedVector3Array",vector3_arr)
	_print_result("Empty PackedColorArray",color_arr)
	
	var i := 4096
	while i>0:
		byte_arr.push_back(randi()%256)
		int32_arr.push_back(randi())
		int64_arr.push_back(randi()*randi())
		float32_arr.push_back(randf())
		float64_arr.push_back(_randf() * _randf())
		string_arr.push_back(str(i*10))
		vector2_arr.push_back(Vector2(i*randf(),i*randf()))
		vector3_arr.push_back(Vector3(i*randf(),i*randf(),i*randf()))
		color_arr.push_back(Color(randf(),randf(),randf(),randf()))
		i-=1
	
	_print_result("PackedByteArray",byte_arr)
	_print_result("PackedInt32Array",int32_arr)
	_print_result("PackedInt64Array",int64_arr)
	_print_result("PackedFloat32Array",float32_arr)
	_print_result("PackedFloat64Array",float64_arr)
	_print_result("PackedStringArray",string_arr)
	_print_result("PackedVector2Array",vector2_arr)
	_print_result("PackedVector3Array",vector3_arr)
	_print_result("PackedColorArray",color_arr)
	
	# 类型化数组
	var typed_arr_bool :Array[bool] = []
	var typed_arr_int :Array[int] = []
	var typed_arr_float :Array[float] = []
	var typed_arr_string :Array[String] = []
	var typed_arr_string_name :Array[StringName] = []
	var typed_arr_vec2 :Array[Vector2] = []
	var typed_arr_vec2i :Array[Vector2i] = []
	var typed_arr_vec3 :Array[Vector3] = []
	var typed_arr_vec3i :Array[Vector3i] = []
	var typed_arr_vec4 :Array[Vector4] = []
	var typed_arr_vec4i :Array[Vector4i] = []
	var typed_arr_projection :Array[Projection] = []
	var typed_arr_rect2 :Array[Rect2] = []
	var typed_arr_rect2i :Array[Rect2i] = []
	var typed_arr_transform2d :Array[Transform2D] = []
	var typed_arr_plane :Array[Plane] = []
	var typed_arr_quaternion :Array[Quaternion] = []
	var typed_arr_aabb :Array[AABB] = []
	var typed_arr_basis :Array[Basis] = []
	var typed_arr_transform3d :Array[Transform3D] = []
	var typed_arr_color :Array[Color] = []
	var typed_arr_node_path :Array[NodePath] = []
	var typed_arr_array :Array[Array] = []
	var typed_arr_dict :Array[Dictionary] = []
	var typed_arr_byte_arr :Array[PackedByteArray] = []
	var typed_arr_int32_arr :Array[PackedInt32Array] = []
	var typed_arr_int64_arr :Array[PackedInt64Array] = []
	var typed_arr_float32_arr :Array[PackedFloat32Array] = []
	var typed_arr_float64_arr :Array[PackedFloat64Array] = []
	var typed_arr_string_arr :Array[PackedStringArray] = []
	var typed_arr_vector2_arr :Array[PackedVector2Array] = []
	var typed_arr_vector3_arr :Array[PackedVector3Array] = []
	var typed_arr_color_arr :Array[PackedColorArray] = []

	_print_result("Empty Array[bool]", typed_arr_bool)
	_print_result("Empty Array[int]", typed_arr_int)
	_print_result("Empty Array[float]", typed_arr_float)
	_print_result("Empty Array[String]", typed_arr_string)
	_print_result("Empty Array[StringName]", typed_arr_string_name)
	_print_result("Empty Array[Vector2]", typed_arr_vec2)
	_print_result("Empty Array[Vector2i]", typed_arr_vec2i)
	_print_result("Empty Array[Vector3]", typed_arr_vec3)
	_print_result("Empty Array[Vector3i]", typed_arr_vec3i)
	_print_result("Empty Array[Vector4]", typed_arr_vec4)
	_print_result("Empty Array[Vector4i]", typed_arr_vec4i)
	_print_result("Empty Array[Projection]", typed_arr_projection)
	_print_result("Empty Array[Rect2]", typed_arr_rect2)
	_print_result("Empty Array[Rect2i]", typed_arr_rect2i)
	_print_result("Empty Array[Transform2D]", typed_arr_transform2d)
	_print_result("Empty Array[Plane]", typed_arr_plane)
	_print_result("Empty Array[Quaternion]", typed_arr_quaternion)
	_print_result("Empty Array[AABB]", typed_arr_aabb)
	_print_result("Empty Array[Basis]", typed_arr_basis)
	_print_result("Empty Array[Transform3D]", typed_arr_transform3d)
	_print_result("Empty Array[Color]", typed_arr_color)
	_print_result("Empty Array[NodePath]", typed_arr_node_path)
	_print_result("Empty Array[Array]", typed_arr_array)
	_print_result("Empty Array[Dictionary]", typed_arr_dict)
	_print_result("Empty Array[PackedByteArray]", typed_arr_byte_arr)
	_print_result("Empty Array[PackedInt32Array]", typed_arr_int32_arr)
	_print_result("Empty Array[PackedInt64Array]", typed_arr_int64_arr)
	_print_result("Empty Array[PackedFloat32Array]", typed_arr_float32_arr)
	_print_result("Empty Array[PackedFloat64Array]", typed_arr_float64_arr)
	_print_result("Empty Array[PackedStringArray]", typed_arr_string_arr)
	_print_result("Empty Array[PackedVector2Array]", typed_arr_vector2_arr)
	_print_result("Empty Array[PackedVector3Array]", typed_arr_vector3_arr)
	_print_result("Empty Array[PackedColorArray]", typed_arr_color_arr)
	
	i = 4096
	while i>0:
		typed_arr_bool.push_back(randi()%2==0)
		typed_arr_int.push_back(randi())
		typed_arr_float.push_back(_randf())
		typed_arr_string.push_back("typed_str_%d"%i)
		typed_arr_string_name.push_back(StringName("typed_str_name_%d"%i))
		typed_arr_vec2.push_back(Vector2(_randf(), _randf()))
		typed_arr_vec2i.push_back(Vector2i(randi(),randi()))
		typed_arr_vec3.push_back(Vector3(_randf(), _randf(), _randf()))
		typed_arr_vec3i.push_back(Vector3i(randi(),randi(),randi()))
		typed_arr_vec4.push_back(rand_vec4.call())
		typed_arr_vec4i.push_back(Vector4i(randi(), randi(), randi(), randi()))
		typed_arr_projection.push_back(Projection(rand_vec4(), rand_vec4(), rand_vec4(), rand_vec4()))
		typed_arr_rect2.push_back(Rect2(_randf(), _randf(), _randf(), _randf()))
		typed_arr_rect2i.push_back(Rect2i(randi(), randi(), randi(), randi()))
		typed_arr_transform2d.push_back(Transform2D(rand_vec2(),rand_vec2(),rand_vec2()))
		typed_arr_plane.push_back(Plane(rand_vec3(),_randf()))
		typed_arr_quaternion.push_back(Quaternion(rand_vec3(), rand_vec3()))
		typed_arr_aabb.push_back(AABB(rand_vec3(),rand_vec3()))
		typed_arr_basis.push_back(Basis(Quaternion(rand_vec3(), rand_vec3())))
		typed_arr_transform3d.push_back(Transform3D(rand_vec3(),rand_vec3(),rand_vec3(),rand_vec3()))
		typed_arr_color.push_back(Color.hex(randi()))
		typed_arr_node_path.push_back(inside_tree_node.get_path())
		if i %1000 == 0:
			typed_arr_array.push_back([])
			typed_arr_dict.push_back({})
			typed_arr_byte_arr.push_back(PackedByteArray())
			typed_arr_int32_arr.push_back(PackedInt32Array())
			typed_arr_int64_arr.push_back(PackedInt64Array())
			typed_arr_float32_arr.push_back(PackedFloat32Array())
			typed_arr_float64_arr.push_back(PackedFloat64Array())
			typed_arr_string_arr.push_back(PackedStringArray())
			typed_arr_vector2_arr.push_back(PackedVector2Array())
			typed_arr_vector3_arr.push_back(PackedVector3Array())
			typed_arr_color_arr.push_back(PackedColorArray())
		else:
			typed_arr_array.push_back([randi(), "asd", rand_vec4(), {"asd":randf()}])
			typed_arr_dict.push_back({"zxc": [randi(), "asd", rand_vec4()], "sads": rand_vec3()})
			typed_arr_byte_arr.push_back(PackedByteArray([randi()%256,randi()%256,randi()%256,randi()%256]))
			typed_arr_int32_arr.push_back(PackedInt32Array([randi(), randi(), randi(), randi()]))
			typed_arr_int64_arr.push_back(PackedInt64Array([pow(randi(), 2.0), pow(randi(), 2.0), pow(randi(), 2.0), pow(randi(), 2.0),pow(randi(), 2.0)]))
			typed_arr_float32_arr.push_back(PackedFloat32Array([_randf(), _randf(), _randf(), _randf(), _randf()]))
			typed_arr_float64_arr.push_back(PackedFloat64Array([pow(_randf(), 2.0), pow(_randf(), 2.0), pow(_randf(), 2.0), pow(_randf(), 2.0),pow(_randf(), 2.0)]))
			typed_arr_string_arr.push_back(PackedStringArray(["", "a1s56d4", "", "asdasd"]))
			typed_arr_vector2_arr.push_back(PackedVector2Array([rand_vec2(),rand_vec2(),rand_vec2(),rand_vec2()]))
			typed_arr_vector3_arr.push_back(PackedVector3Array([rand_vec3(),rand_vec3(),rand_vec3(),rand_vec3()]))
			typed_arr_color_arr.push_back(PackedColorArray([rand_color(), rand_color(), rand_color(), rand_color(), rand_color()]))
		i-=1
		
	_print_result("Array[bool]", typed_arr_bool)
	_print_result("Array[int]", typed_arr_int)
	_print_result("Array[float]", typed_arr_float)
	_print_result("Array[String]", typed_arr_string)
	_print_result("Array[StringName]", typed_arr_string_name)
	_print_result("Array[Vector2]", typed_arr_vec2)
	_print_result("Array[Vector2i]", typed_arr_vec2i)
	_print_result("Array[Vector3]", typed_arr_vec3)
	_print_result("Array[Vector3i]", typed_arr_vec3i)
	_print_result("Array[Vector4]", typed_arr_vec4)
	_print_result("Array[Vector4i]", typed_arr_vec4i)
	_print_result("Array[Projection]", typed_arr_projection)
	_print_result("Array[Rect2]", typed_arr_rect2)
	_print_result("Array[Rect2i]", typed_arr_rect2i)
	_print_result("Array[Transform2D]", typed_arr_transform2d)
	_print_result("Array[Plane]", typed_arr_plane)
	_print_result("Array[Quaternion]", typed_arr_quaternion)
	_print_result("Array[AABB]", typed_arr_aabb)
	_print_result("Array[Basis]", typed_arr_basis)
	_print_result("Array[Transform3D]", typed_arr_transform3d)
	_print_result("Array[Color]", typed_arr_color)
	_print_result("Array[NodePath]", typed_arr_node_path)
	_print_result("Array[Array]", typed_arr_array)
	_print_result("Array[Dictionary]", typed_arr_dict)
	_print_result("Array[PackedByteArray]", typed_arr_byte_arr)
	_print_result("Array[PackedInt32Array]", typed_arr_int32_arr)
	_print_result("Array[PackedInt64Array]", typed_arr_int64_arr)
	_print_result("Array[PackedFloat32Array]", typed_arr_float32_arr)
	_print_result("Array[PackedFloat64Array]", typed_arr_float64_arr)
	_print_result("Array[PackedStringArray]", typed_arr_string_arr)
	_print_result("Array[PackedVector2Array]", typed_arr_vector2_arr)
	_print_result("Array[PackedVector3Array]", typed_arr_vector3_arr)
	_print_result("Array[PackedColorArray]", typed_arr_color_arr)
	return


func _randf() -> float: return randi()*(randf() - 0.5 )*2
func rand_vec2() -> Vector2: return Vector2(_randf(), _randf())
func rand_vec3() -> Vector3: return Vector3(_randf(), _randf(), _randf())
func rand_vec4() -> Vector4: return Vector4(_randf(), _randf(), _randf(), _randf())
func rand_color() -> Color: return Color(randf(), randf(), randf(), randf())
# 序列化与反序列并打印对比结果
func _print_result(title_name:String, v)->void:
	var type = typeof(v)
	# JSON - utf8
	var json := JSON.new()
	var json_ser_time := Time.get_ticks_usec()
	var json_bytes := json.stringify(v).to_utf8_buffer()
	json_ser_time = Time.get_ticks_usec() - json_ser_time
	var json_deser_time := Time.get_ticks_usec()
	json.parse(json_bytes.get_string_from_utf8())
	var json_deser = json.get_data() 
	json_deser_time = Time.get_ticks_usec() - json_deser_time 
	
	# GUS
	var gus_ser_time := Time.get_ticks_usec()
	var gus_bytes := GUS.var_to_bytes(v) # GUS 序列化后数据
	gus_ser_time = Time.get_ticks_usec() - gus_ser_time
	var gus_deser_time := Time.get_ticks_usec()
	var gus_deser = GUS.bytes_to_var(gus_bytes)
	gus_deser_time = Time.get_ticks_usec() - gus_deser_time
	
	# Native
	var native_ser_time := Time.get_ticks_usec()
	var native_bytes := var_to_bytes(v)
	native_ser_time = Time.get_ticks_usec() - native_ser_time
	var native_deser_time := Time.get_ticks_usec()
	var native_deser = convert(bytes_to_var(native_bytes),type)
	native_deser_time = Time.get_ticks_usec() - native_deser_time
	

	if typeof(v) == typeof(gus_deser) and str(v) == str(gus_deser): # 检查序列化前后的类型和具体值是否相同
		if typeof(v) >TYPE_ARRAY or (v is Array and v.is_typed()): # 池化数组不打印具体值（打印溢出
			print("%s - Size:%d\n\t[GUS]: \t\t%d bytes \t\tser %d usec \t\tdeser %d usec\n\t[Native]: \t%d bytes \t\tser %d usec \t\tdeser %d usec\n\t[JSON]:\t\t%d bytes \t\tser %d usec \t\tdeser %d usec."%[
					title_name, v.size(), 
					gus_bytes.size(), gus_ser_time, gus_deser_time ,
					native_bytes.size() , native_ser_time, native_deser_time,
					json_bytes.size(), json_ser_time, json_deser_time])
		else:
			print("%s - value:%s\n\t[GUS]: \t\t%d bytes \t\tser %d usec \t\tdeser %d usec\n\t[Native]: \t%d bytes \t\tser %d usec \t\tdeser %d usec\n\t[JSON]:\t\t%d bytes \t\tser %d usec \t\tdeser %d usec."%[
					title_name, str(v), 
					gus_bytes.size(), gus_ser_time, gus_deser_time ,
					native_bytes.size() , native_ser_time, native_deser_time,
					json_bytes.size(), json_ser_time, json_deser_time])
	else: # 反序列化异常
#		if typeof(v) >TYPE_ARRAY: # 池化数组不打印具体值（打印溢出
#			printerr("%s 解码失败 源: %d  流： %d\n\t "%[title_name, v.size(), gus_deser.size()])
#		else:
#			printerr("%s: %s  解码结果:%s   解码失败\n\t - 数据流: %s"%[title_name,str(v),str(gus_deser),str(gus_bytes)])
		printerr("%s: \n\t%s \n解码结果: \n\t%s   \n解码失败\n\t - 数据流: %s"%[title_name,str(v),str(gus_deser),str(gus_bytes)])
	print("")
