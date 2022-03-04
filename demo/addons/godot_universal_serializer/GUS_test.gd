@tool
extends EditorScript

var gus := GodotUniversialSerializer.new()
func _run() -> void:
	gus.color_encoding = GUS.ColorEncoding.RGBA_FLOAT
	await  _print_result("ignore", null)
	
	_test(get_editor_interface())
	
	
var tree:SceneTree
func _test(inside_tree_node:Node)->void:
	tree = inside_tree_node.get_tree() if inside_tree_node and inside_tree_node.is_inside_tree() else null
	await _print_result("null" , null)
	await _print_result("bool" , true)
	await _print_result("bool" , false)
	await _print_result("int1" , 0x7a)
	await _print_result("int2" , 0x5c3b)
	await _print_result("int3" , -0x7a5390)
	await _print_result("int4" , 0x6a53c904)
	await _print_result("int5" , -0x7c340c9041)
	await _print_result("int6" , 0x5a53c904d054)
	await _print_result("int7" , 0x3c90a53c90d041)
	await _print_result("int8" , -0x7aaa53c907a414d0)
	await _print_result("float" , -105.0532)
	await _print_result("String" , "test 测试")
	await _print_result("StringName" , StringName("TestStringName"))
	await _print_result("Vector2" , Vector2(-423, 46.8005))
	await _print_result("Vector2i" , Vector2i(-423, 0x5c3b))
	await _print_result("Vector3" , Vector3(52011,-541.3327,77441))
	await _print_result("Vector3i" , Vector3i(-423,0x5c3b,52011))
	await _print_result("Rect2" , Rect2(56.622, -77.85, 8740.2369, 441.044))
	await _print_result("Rect2i" , Rect2i(randi(), randi(), randi(), randi()))
	await _print_result("Transform2D" , Transform2D.FLIP_Y.translated(Vector2(513,88.5)))
	await _print_result("Plane", Plane(-54.55, 876, 2310,744).normalized())
	await _print_result("Quaternion", Quaternion(-54.55, 876, 2310,744))
	await _print_result("AABB", AABB(Vector3(-77.85, 8740.2369, 441.044),Vector3(105,50,68)))
	await _print_result("Basis", Transform3D.FLIP_Y.translated(Vector3(513,-88.5,105)).basis)
	await _print_result("Transform3D", Transform3D.FLIP_Y.translated(Vector3(513,-88.5,105)))
	await _print_result("Color", Color.BEIGE)
	await _print_result("NodePath", inside_tree_node.get_path())
	var arr = [574,-21.0,Vector2(-423,46.8005),Color.AQUA,"acxx"]
	await _print_result("Array", arr )
	# 字典测试
	var dict :={
		k_str = "value1",
		k_color = Color.ALICE_BLUE,
		k_int = 123123,
		k_float = 44.886,
		k_vec3 = Vector3(52011,-541.3327,77441)	
	}
	dict[532] = [574,-21.0,Vector2(-423,46.8005),Color.AQUA,"acxx"]
	await _print_result("Dictionary",dict)
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
	
	var i := 4096
	while i>0:
		byte_arr.push_back(i%256)
		int32_arr.push_back(i)
		int64_arr.push_back(i*i)
		float32_arr.push_back(i * randf())
		float32_arr.push_back(randf() * i*i)
		string_arr.push_back(str(i*10))
		vector2_arr.push_back(Vector2(i*randf(),i*randf()))
		vector3_arr.push_back(Vector3(i*randf(),i*randf(),i*randf()))
		color_arr.push_back(Color(randf(),randf(),randf(),randf()))
		i-=1
	
	await _print_result("PackedByteArray",byte_arr)
	await _print_result("PackedInt32Array",int32_arr)
	await _print_result("PackedInt64Array",int32_arr)
	await _print_result("PackedFloat32Array",float32_arr)
	await _print_result("PackedFloat64Array",float32_arr)
	await _print_result("PackedStringArray",string_arr)
	await _print_result("PackedVector2Array",vector2_arr)
	await _print_result("PackedVector3Array",vector3_arr)
	await _print_result("PackedColorArray",color_arr)

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
	
	# gus
	var gus_ser_time := Time.get_ticks_usec()
	var gus_bytes := gus.to_bytes(v) # GUS 序列化后数据
	gus_ser_time = Time.get_ticks_usec() - gus_ser_time
	var gus_deser_time := Time.get_ticks_usec()
	var gus_deser = gus.to_var(gus_bytes)
	gus_deser_time = Time.get_ticks_usec() - gus_deser_time
#	print(gus_bytes)
	
	# Native
	var native_ser_time := Time.get_ticks_usec()
	var native_bytes := var2bytes(v)
	native_ser_time = Time.get_ticks_usec() - native_ser_time
	var native_deser_time := Time.get_ticks_usec()
	var native_deser = convert(bytes2var(native_bytes),type)
	native_deser_time = Time.get_ticks_usec() - native_deser_time
	

	if typeof(v) == typeof(gus_deser) and str(v) == str(gus_deser): # 检查序列化前后的类型和具体值是否相同
		if typeof(v) >TYPE_ARRAY: # 池化数组不打印具体值（打印溢出
			print("%s - Size:%d\n\t[GUS]: %d bytes - ser %d usec - deser %d usec\n\t[Native]: %d bytes - ser %d usec - deser %d usec\n\t[JSON]:%d bytes - ser %d usec - deser %d usec."%[
					title_name, v.size(), 
					gus_bytes.size(), gus_ser_time, gus_deser_time ,
					native_bytes.size() , native_ser_time, native_deser_time,
					json_bytes.size(), json_ser_time, json_deser_time])
		else:
			print("%s - value:%s\n\t[GUS]: %d bytes - ser %d usec - deser %d usec\n\t[Native]: %d bytes - ser %d usec - deser %d usec\n\t[JSON]:%d bytes - ser %d usec - deser %d usec."%[
					title_name, str(v), 
					gus_bytes.size(), gus_ser_time, gus_deser_time ,
					native_bytes.size() , native_ser_time, native_deser_time,
					json_bytes.size(), json_ser_time, json_deser_time])
	else: # 反序列化异常
		printerr("%s: %s  解码结果:%s   解码失败\n\t - 数据流: %s"%[title_name,str(v),str(gus_deser),str(gus_bytes)])
	print("")
