
# GUS - Godot Universal Serializer 2.1.0 - Godot的通用序列化器 - V2.1.0

![Image text](icon.png)

[中文文档](README_zh_cn.md) <- 点击这里。

A Godot's universal serializer for size optimization.    
Support all data types except `Object`, `RID`, `Callable` and `Signal` in Godot 4.x.

# Feature:
  1. Without losing type information. Need not type detection and forced typce conversion in cross end transfer，just like local parameter transfer.
  2. Integer are variable length serialization.
  3. The serialized data size is smaller than JSON text to utf8 and the native serialization method `var_to_bytes()`.GUS is especially suitable for the serialization of structured small data transfer in multiplayer games.
  4. GDExtension plugin, high performance. 

  You can run the `EditorScript` which named `GUS_benchmark.gd` to view the contrast between `GUS`, `Native`, and `JSON`.

# How to use:
  1. Download released plugin, install ~~and enable~~ this plugin just like other plugin.
  2. ~~Modify properties of autoload which named `GUS` as you require.~~
  3. Pass variable which without `Object`, `OID`, `Callable`, `Signal` into `GUS.var_to_bytes()` and get the serialized data, than send to network peer.
  3. After the remote network peer obtains the serialized data, passe it into `GUS.bytes_to_var()` to get the same variable as before serialization.


# Be careful:
  1. ~~The properties of autoload `GUS` between all network peers should keep the same.~~
  2. `Array` and `Dictionary` should not contain `Object`, `RID`, `Callable` and `Signal`.
  3. ~~Currently unsupport Godot 4.x which compiled with `typeof double real_t`.~~
  4. Currently just support platform `windows`.
	
# TODO:
  1. ~~If I find the way to bind static method for `GDScript`, I will get rid of the autoload `GUS`.~~
  2. Compile `linux` and `osx` version( but I encounter error which I can't handle it).
  3. ~~Compile `typeof double real_t` version to support custom Godot( but I encounter error which I can't handle it, too).~~
  4. Deal `Object`, `RID`, `Callable` and `Signal`.

# How to compile:
  1. Follow [offical tutorial](https://docs.godotengine.org/zh_CN/stable/development/compiling/index.html) to set up you develop enviroment.
  1. Clone this repository (recursive).
  3. Navigate to `godot-cpp` folder, and run command as below :

	cd godot-cpp
	scons
  4. Navigate to root folder, and run command as below :
	
	scons
  5. Now, you can get the addon which be located at `\demo\addons\com.daylily_zeleen.godot_universal_serializer`.

# Benchmark 2.1.0:
```
null - value:<null>
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	4 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		4 bytes 		ser 10 usec 		deser 8 usec.

bool - value:true
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		4 bytes 		ser 6 usec 		deser 4 usec.

bool - value:false
	[GUS]: 		1 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		5 bytes 		ser 5 usec 		deser 4 usec.

int1 - value:122
	[GUS]: 		3 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		3 bytes 		ser 5 usec 		deser 2 usec.

int2 - value:23611
	[GUS]: 		4 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		5 bytes 		ser 5 usec 		deser 2 usec.

int3 - value:-8016784
	[GUS]: 		5 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		8 bytes 		ser 5 usec 		deser 2 usec.

int4 - value:1783875844
	[GUS]: 		6 bytes 		ser 6 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		10 bytes 		ser 7 usec 		deser 3 usec.

int5 - value:-533449183297
	[GUS]: 		7 bytes 		ser 4 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		13 bytes 		ser 6 usec 		deser 2 usec.

int6 - value:99315901321300
	[GUS]: 		8 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		14 bytes 		ser 6 usec 		deser 3 usec.

int7 - value:17047537962766401
	[GUS]: 		9 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		17 bytes 		ser 6 usec 		deser 2 usec.

int8 - value:-8838969341549745360
	[GUS]: 		11 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		20 bytes 		ser 7 usec 		deser 2 usec.

float - value:-105.0532
	[GUS]: 		9 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		9 bytes 		ser 15 usec 		deser 3 usec.

String - value:test 测试
	[GUS]: 		13 bytes 		ser 10 usec 		deser 3 usec
	[Native]: 	20 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		13 bytes 		ser 8 usec 		deser 5 usec.

StringName - value:TestStringName
	[GUS]: 		16 bytes 		ser 8 usec 		deser 4 usec
	[Native]: 	24 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		16 bytes 		ser 8 usec 		deser 4 usec.

Vector2 - value:(-423, 46.8005)
	[GUS]: 		9 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		17 bytes 		ser 17 usec 		deser 4 usec.

Vector2i - value:(-423, 23611)
	[GUS]: 		9 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		15 bytes 		ser 25 usec 		deser 7 usec.

Vector3 - value:(52011, -541.3327, 77441)
	[GUS]: 		13 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	16 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		27 bytes 		ser 22 usec 		deser 6 usec.

Vector3i - value:(-423, 23611, 52011)
	[GUS]: 		13 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	16 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		22 bytes 		ser 14 usec 		deser 5 usec.

Vector4 - value:(5211, -541.327, 7441, 1.23)
	[GUS]: 		17 bytes 		ser 16 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		30 bytes 		ser 25 usec 		deser 6 usec.

Vector4i - value:(-42, 49947, 5211, 445)
	[GUS]: 		17 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		25 bytes 		ser 16 usec 		deser 5 usec.

Projection - value:
8740.2373046875, 0, 0, 0
0, 441.044006347656, 0, 0
0, 0, 1, 0
56.6220016479492, -77.8499984741211, 0, 1
	[GUS]: 		65 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	68 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		110 bytes 		ser 57 usec 		deser 9 usec.

Rect2 - value:[P: (56.622, -77.85), S: (8740.237, 441.044)]
	[GUS]: 		17 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		47 bytes 		ser 27 usec 		deser 6 usec.

Rect2i - value:[P: (-1957397172, 2089700759), S: (692688053, 88406102)]
	[GUS]: 		17 bytes 		ser 6 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		58 bytes 		ser 25 usec 		deser 8 usec.

Transform2D - value:[X: (1, 0), Y: (0, -1), O: (513, 88.5)]
	[GUS]: 		25 bytes 		ser 6 usec 		deser 2 usec
	[Native]: 	28 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		41 bytes 		ser 33 usec 		deser 7 usec.

Plane - value:[N: (-0.022075, 0.354495, 0.934797), D: 0.301078]
	[GUS]: 		17 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		51 bytes 		ser 25 usec 		deser 6 usec.

Quaternion - value:(-54.55, 876, 2310, 744)
	[GUS]: 		17 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		26 bytes 		ser 18 usec 		deser 5 usec.

AABB - value:[P: (-77.85, 8740.237, 441.044), S: (105, 50, 68)]
	[GUS]: 		25 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	28 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		52 bytes 		ser 31 usec 		deser 6 usec.

Basis - value:[X: (1, 0, 0), Y: (0, -1, 0), Z: (0, 0, 1)]
	[GUS]: 		37 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	40 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		45 bytes 		ser 26 usec 		deser 5 usec.

Transform3D - value:[X: (1, 0, 0), Y: (0, -1, 0), Z: (0, 0, 1), O: (513, -88.5, 105)]
	[GUS]: 		49 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	52 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		67 bytes 		ser 36 usec 		deser 8 usec.

Color - value:(1, 1, 1, 1)
	[GUS]: 		17 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		14 bytes 		ser 20 usec 		deser 4 usec.

NodePath - value:/root/@@16380/@@11234
	[GUS]: 		23 bytes 		ser 13 usec 		deser 10 usec
	[Native]: 	48 bytes 		ser 6 usec 		deser 6 usec
	[JSON]:		23 bytes 		ser 12 usec 		deser 5 usec.

Array - value:[574, -21, (-423, 46.8005), (0, 1, 1, 1), "acxx"]
	[GUS]: 		46 bytes 		ser 15 usec 		deser 8 usec
	[Native]: 	68 bytes 		ser 5 usec 		deser 6 usec
	[JSON]:		49 bytes 		ser 44 usec 		deser 12 usec.

Dictionary - value:{ "k_str": "value1", "k_color": (0.9412, 0.9725, 1, 1), "k_int": 123123, "k_float": 44.886, "k_vec3": (52011, -541.3327, 77441), 532: [574, -21, (-423, 46.8005), (0, 1, 1, 1), "acxx"] }
	[GUS]: 		143 bytes 		ser 38 usec 		deser 17 usec
	[Native]: 	236 bytes 		ser 19 usec 		deser 17 usec
	[JSON]:		178 bytes 		ser 95 usec 		deser 30 usec.

Empty Array - value:[]
	[GUS]: 		1 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 2 usec.

Empty Dictionary - value:{  }
	[GUS]: 		1 bytes 		ser 4 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 8 usec 		deser 3 usec.

Empty PackedByteArray - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		4 bytes 		ser 8 usec 		deser 2 usec.

Empty PackedInt32Array - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 2 usec.

Empty PackedInt64Array - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 8 usec 		deser 2 usec.

Empty PackedFloat32Array - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 8 usec 		deser 2 usec.

Empty PackedFloat64Array - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 8 usec 		deser 2 usec.

Empty PackedStringArray - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 8 usec 		deser 2 usec.

Empty PackedVector2Array - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		4 bytes 		ser 8 usec 		deser 3 usec.

Empty PackedVector3Array - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		4 bytes 		ser 8 usec 		deser 2 usec.

Empty PackedColorArray - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 8 usec 		deser 2 usec.

PackedByteArray - Size:4096
	[GUS]: 		4099 bytes 		ser 8 usec 		deser 2 usec
	[Native]: 	4104 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		18729 bytes 		ser 1012 usec 		deser 340 usec.

PackedInt32Array - Size:4096
	[GUS]: 		20220 bytes 		ser 75 usec 		deser 53 usec
	[Native]: 	16392 bytes 		ser 7 usec 		deser 7 usec
	[JSON]:		45018 bytes 		ser 2323 usec 		deser 611 usec.

PackedInt64Array - Size:4096
	[GUS]: 		38369 bytes 		ser 119 usec 		deser 60 usec
	[Native]: 	32776 bytes 		ser 22 usec 		deser 12 usec
	[JSON]:		81558 bytes 		ser 3326 usec 		deser 667 usec.

PackedFloat32Array - Size:4096
	[GUS]: 		16387 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	16392 bytes 		ser 8 usec 		deser 6 usec
	[JSON]:		73720 bytes 		ser 2859 usec 		deser 671 usec.

PackedFloat64Array - Size:4096
	[GUS]: 		32771 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	32776 bytes 		ser 13 usec 		deser 10 usec
	[JSON]:		80125 bytes 		ser 3116 usec 		deser 691 usec.

PackedStringArray - Size:4096
	[GUS]: 		23472 bytes 		ser 2607 usec 		deser 374 usec
	[Native]: 	48764 bytes 		ser 980 usec 		deser 507 usec
	[JSON]:		31662 bytes 		ser 3205 usec 		deser 1660 usec.

PackedVector2Array - Size:4096
	[GUS]: 		49155 bytes 		ser 9 usec 		deser 5 usec
	[Native]: 	32776 bytes 		ser 17 usec 		deser 11 usec
	[JSON]:		89216 bytes 		ser 7336 usec 		deser 1337 usec.

PackedVector3Array - Size:4096
	[GUS]: 		49155 bytes 		ser 10 usec 		deser 8 usec
	[Native]: 	49160 bytes 		ser 22 usec 		deser 18 usec
	[JSON]:		129714 bytes 		ser 10499 usec 		deser 2048 usec.

PackedColorArray - Size:4096
	[GUS]: 		65539 bytes 		ser 13 usec 		deser 8 usec
	[Native]: 	65544 bytes 		ser 28 usec 		deser 18 usec
	[JSON]:		137505 bytes 		ser 14079 usec 		deser 2174 usec.

Empty Array[bool] - Size:0
	[GUS]: 		1 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 6 usec 		deser 1 usec.

Empty Array[int] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec.

Empty Array[float] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[String] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec.

Empty Array[StringName] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Vector2] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 0 usec.

Empty Array[Vector2i] - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 4 usec 		deser 1 usec.

Empty Array[Vector3] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 32 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec.

Empty Array[Vector3i] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec.

Empty Array[Vector4] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Vector4i] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 0 usec.

Empty Array[Projection] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Rect2] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 30 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Rect2i] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 21 usec.

Empty Array[Transform2D] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 17 usec 		deser 0 usec.

Empty Array[Plane] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Quaternion] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[AABB] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Basis] - Size:0
	[GUS]: 		1 bytes 		ser 20 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 0 usec.

Empty Array[Transform3D] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 0 usec.

Empty Array[Color] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[NodePath] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 17 usec.

Empty Array[Dictionary] - Size:0
	[GUS]: 		1 bytes 		ser 15 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[PackedByteArray] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[PackedInt32Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[PackedInt64Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[PackedFloat32Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[PackedFloat64Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 24 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 0 usec.

Empty Array[PackedStringArray] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec.

Empty Array[PackedVector2Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec.

Empty Array[PackedVector3Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 0 usec.

Empty Array[PackedColorArray] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 0 usec.

Array[bool] - Size:4096
	[GUS]: 		4099 bytes 		ser 33 usec 		deser 107 usec
	[Native]: 	32776 bytes 		ser 95 usec 		deser 164 usec
	[JSON]:		22537 bytes 		ser 1000 usec 		deser 1765 usec.

Array[int] - Size:4096
	[GUS]: 		20355 bytes 		ser 94 usec 		deser 177 usec
	[Native]: 	40860 bytes 		ser 129 usec 		deser 173 usec
	[JSON]:		43959 bytes 		ser 2050 usec 		deser 539 usec.

Array[float] - Size:4096
	[GUS]: 		32771 bytes 		ser 67 usec 		deser 149 usec
	[Native]: 	49160 bytes 		ser 101 usec 		deser 155 usec
	[JSON]:		71230 bytes 		ser 2948 usec 		deser 671 usec.

Array[String] - Size:4096
	[GUS]: 		60336 bytes 		ser 3352 usec 		deser 748 usec
	[Native]: 	97916 bytes 		ser 1203 usec 		deser 838 usec
	[JSON]:		68526 bytes 		ser 3633 usec 		deser 2771 usec.

Array[StringName] - Size:4096
	[GUS]: 		80816 bytes 		ser 3417 usec 		deser 1466 usec
	[Native]: 	114660 bytes 		ser 1317 usec 		deser 1386 usec
	[JSON]:		89006 bytes 		ser 16444 usec 		deser 3365 usec.

Array[Vector2] - Size:4096
	[GUS]: 		32771 bytes 		ser 82 usec 		deser 166 usec
	[Native]: 	49160 bytes 		ser 86 usec 		deser 170 usec
	[JSON]:		109071 bytes 		ser 8875 usec 		deser 3541 usec.

Array[Vector2i] - Size:4096
	[GUS]: 		32771 bytes 		ser 85 usec 		deser 165 usec
	[Native]: 	49160 bytes 		ser 86 usec 		deser 170 usec
	[JSON]:		110408 bytes 		ser 8821 usec 		deser 3575 usec.

Array[Vector3] - Size:4096
	[GUS]: 		49155 bytes 		ser 87 usec 		deser 170 usec
	[Native]: 	65544 bytes 		ser 93 usec 		deser 172 usec
	[JSON]:		157378 bytes 		ser 12065 usec 		deser 4880 usec.

Array[Vector3i] - Size:4096
	[GUS]: 		49155 bytes 		ser 91 usec 		deser 165 usec
	[Native]: 	65544 bytes 		ser 90 usec 		deser 174 usec
	[JSON]:		159579 bytes 		ser 12208 usec 		deser 4825 usec.

Array[Vector4] - Size:4096
	[GUS]: 		65539 bytes 		ser 98 usec 		deser 173 usec
	[Native]: 	81928 bytes 		ser 160 usec 		deser 210 usec
	[JSON]:		205592 bytes 		ser 15397 usec 		deser 5609 usec.

Array[Vector4i] - Size:4096
	[GUS]: 		65539 bytes 		ser 96 usec 		deser 175 usec
	[Native]: 	81928 bytes 		ser 101 usec 		deser 176 usec
	[JSON]:		208515 bytes 		ser 14953 usec 		deser 5682 usec.

Array[Projection] - Size:4096
	[GUS]: 		262147 bytes 		ser 343 usec 		deser 443 usec
	[Native]: 	278536 bytes 		ser 126 usec 		deser 530 usec
	[JSON]:		787336 bytes 		ser 63237 usec 		deser 15486 usec.

Array[Rect2] - Size:4096
	[GUS]: 		65539 bytes 		ser 95 usec 		deser 177 usec
	[Native]: 	81928 bytes 		ser 102 usec 		deser 188 usec
	[JSON]:		246707 bytes 		ser 18833 usec 		deser 6150 usec.

Array[Rect2i] - Size:4096
	[GUS]: 		65539 bytes 		ser 100 usec 		deser 184 usec
	[Native]: 	81928 bytes 		ser 94 usec 		deser 177 usec
	[JSON]:		249473 bytes 		ser 18718 usec 		deser 6412 usec.

Array[Transform2D] - Size:4096
	[GUS]: 		98307 bytes 		ser 115 usec 		deser 279 usec
	[Native]: 	114696 bytes 		ser 92 usec 		deser 410 usec
	[JSON]:		363884 bytes 		ser 26804 usec 		deser 8343 usec.

Array[Plane] - Size:4096
	[GUS]: 		65539 bytes 		ser 90 usec 		deser 181 usec
	[Native]: 	81928 bytes 		ser 99 usec 		deser 258 usec
	[JSON]:		238316 bytes 		ser 17074 usec 		deser 6113 usec.

Array[Quaternion] - Size:4096
	[GUS]: 		65539 bytes 		ser 93 usec 		deser 319 usec
	[Native]: 	81928 bytes 		ser 102 usec 		deser 173 usec
	[JSON]:		132694 bytes 		ser 13117 usec 		deser 4095 usec.

Array[AABB] - Size:4096
	[GUS]: 		98307 bytes 		ser 120 usec 		deser 278 usec
	[Native]: 	114696 bytes 		ser 114 usec 		deser 395 usec
	[JSON]:		343175 bytes 		ser 25618 usec 		deser 8101 usec.

Array[Basis] - Size:4096
	[GUS]: 		147459 bytes 		ser 153 usec 		deser 288 usec
	[Native]: 	163848 bytes 		ser 99 usec 		deser 439 usec
	[JSON]:		323255 bytes 		ser 33166 usec 		deser 7559 usec.

Array[Transform3D] - Size:4096
	[GUS]: 		196611 bytes 		ser 208 usec 		deser 302 usec
	[Native]: 	213000 bytes 		ser 125 usec 		deser 441 usec
	[JSON]:		674292 bytes 		ser 48547 usec 		deser 13698 usec.

Array[Color] - Size:4096
	[GUS]: 		65539 bytes 		ser 95 usec 		deser 177 usec
	[Native]: 	81928 bytes 		ser 101 usec 		deser 173 usec
	[JSON]:		140672 bytes 		ser 16292 usec 		deser 4490 usec.

Array[NodePath] - Size:4096
	[GUS]: 		90115 bytes 		ser 7099 usec 		deser 5188 usec
	[Native]: 	196616 bytes 		ser 4340 usec 		deser 4479 usec
	[JSON]:		98305 bytes 		ser 5485 usec 		deser 3443 usec.

Array[Array] - Size:4096
	[GUS]: 		192209 bytes 		ser 14368 usec 		deser 6285 usec
	[Native]: 	319112 bytes 		ser 8801 usec 		deser 5025 usec
	[JSON]:		388548 bytes 		ser 33553 usec 		deser 12888 usec.

Array[Dictionary] - Size:4096
	[GUS]: 		229035 bytes 		ser 17793 usec 		deser 7079 usec
	[Native]: 	401032 bytes 		ser 8543 usec 		deser 6305 usec
	[JSON]:		500944 bytes 		ser 48386 usec 		deser 18849 usec.

Array[PackedByteArray] - Size:4096
	[GUS]: 		20467 bytes 		ser 507 usec 		deser 746 usec
	[Native]: 	49144 bytes 		ser 213 usec 		deser 757 usec
	[JSON]:		87049 bytes 		ser 8208 usec 		deser 3452 usec.

Array[PackedInt32Array] - Size:4096
	[GUS]: 		84983 bytes 		ser 747 usec 		deser 889 usec
	[Native]: 	98248 bytes 		ser 410 usec 		deser 806 usec
	[JSON]:		188240 bytes 		ser 12074 usec 		deser 3921 usec.

Array[PackedInt64Array] - Size:4096
	[GUS]: 		197471 bytes 		ser 1711 usec 		deser 1186 usec
	[Native]: 	196456 bytes 		ser 339 usec 		deser 852 usec
	[JSON]:		416321 bytes 		ser 20143 usec 		deser 5401 usec.

Array[PackedFloat32Array] - Size:4096
	[GUS]: 		85939 bytes 		ser 840 usec 		deser 750 usec
	[Native]: 	114616 bytes 		ser 258 usec 		deser 789 usec
	[JSON]:		229887 bytes 		ser 17555 usec 		deser 4669 usec.

Array[PackedFloat64Array] - Size:4096
	[GUS]: 		167779 bytes 		ser 1069 usec 		deser 814 usec
	[Native]: 	196456 bytes 		ser 319 usec 		deser 857 usec
	[JSON]:		399217 bytes 		ser 19182 usec 		deser 5325 usec.

Array[PackedStringArray] - Size:4096
	[GUS]: 		73663 bytes 		ser 7654 usec 		deser 1694 usec
	[Native]: 	196456 bytes 		ser 3004 usec 		deser 2666 usec
	[JSON]:		110497 bytes 		ser 16081 usec 		deser 5809 usec.

Array[PackedVector2Array] - Size:4096
	[GUS]: 		135043 bytes 		ser 874 usec 		deser 738 usec
	[Native]: 	163720 bytes 		ser 290 usec 		deser 846 usec
	[JSON]:		431172 bytes 		ser 31307 usec 		deser 9351 usec.

Array[PackedVector3Array] - Size:4096
	[GUS]: 		200515 bytes 		ser 1352 usec 		deser 782 usec
	[Native]: 	229192 bytes 		ser 376 usec 		deser 904 usec
	[JSON]:		624761 bytes 		ser 45277 usec 		deser 13331 usec.

Array[PackedColorArray] - Size:4096
	[GUS]: 		331459 bytes 		ser 1128 usec 		deser 854 usec
	[Native]: 	360136 bytes 		ser 416 usec 		deser 1001 usec
	[JSON]:		698851 bytes 		ser 76333 usec 		deser 14316 usec.
```
# Benchmark 2.0.0:
![Image text](benchmark/1.png)
![Image text](benchmark/2.png)
![Image text](benchmark/3.png)
![Image text](benchmark/4.png)
![Image text](benchmark/5.png)