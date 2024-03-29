
# GUS - Godot Universal Serializer 2 - Godot的通用序列化器2

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
  1. Pass variable which without `Object`, `OID`, `Callable`, `Signal` into `GUS.var_to_bytes()` and get the serialized data, than send to network peer.
  2. After the remote network peer obtains the serialized data, passe it into `GUS.bytes_to_var()` to get the same variable as before serialization.


# Be careful:
  1. `Array` and `Dictionary` should not contain `Object`, `RID`, `Callable` and `Signal`.


# How to compile:
  1. Follow [offical tutorial](https://docs.godotengine.org/zh_CN/stable/development/compiling/index.html) to set up you develop enviroment.
  2. Clone this repository (with submodule).
  3. You must generate bindings and build library of `godot-cpp` at least once.
		- You can use generate and build manully by steps below:
			a. Navigate to `godot-cpp` folder.
			b. Run command:
			```
			scons generate_bindings=yes build_library=yes
			```
			c. Now you can add `generate_bindings=no build_library=no` at next stage to avoid generate bindings and build library every time when compiling `GUS`.

		- (Recommand) Generate bindings and build library every time when compiling `GUS`, to keep the compiling condition of `godot-cpp` same as `GUS`.
		Just skip this stage and add `generate_bindings=yes build_library=yes` or ignore these command arguments.
		Don't worry, generate bindings and build library of `godot-cpp` would not take many time if the compile conditions have not change.
  4. Here is a little different from `godot-cpp` to compile `GUS` (of course, you can use `scons` to compile, too).
     To do some post process, I use a tool script to compile `GUS`.
	 Navigate to root folder, run command as below to compile `GUS`:
	 ```
	 python build.py
	 ```

	 You can add arguments like to build `godot-cpp` at this stage.

	 To learn more arguments detail, run this command.
	 ```
	 python build.py -h
	 ```

	 **Specially**:
	 - If you not specify the argument `target`, this tool will build both `target=template_debug` and `target=template_release`.
	 - If you add argument `dev_build`, this tool will copy and rename the built shared libraries to `dist` folder with replacing `.dev.` to `.` (in order to avoid edit `gus2.gdextension`).

  5. Now, you can get the addon which be located at `dist`.

	 **Notice**:
	 - If your use `scons` instead of `python build_tool`, you can only get dynamic library at `bin` folder.
	 - If build for `macos`, its binaries will generate at `dist` directly.

# Benchmark 2.1.5:
```
null - value:<null>
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	4 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		4 bytes 		ser 18 usec 		deser 9 usec

bool - value:true
	[GUS]: 		1 bytes 		ser 5 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		4 bytes 		ser 10 usec 		deser 6 usec

bool - value:false
	[GUS]: 		1 bytes 		ser 4 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		5 bytes 		ser 9 usec 		deser 5 usec

int1 - value:122
	[GUS]: 		3 bytes 		ser 4 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		3 bytes 		ser 9 usec 		deser 3 usec

int2 - value:23611
	[GUS]: 		4 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 2 usec
	[JSON]:		5 bytes 		ser 9 usec 		deser 4 usec

int3 - value:-8016784
	[GUS]: 		5 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 2 usec
	[JSON]:		8 bytes 		ser 23 usec 		deser 3 usec

int4 - value:1783875844
	[GUS]: 		6 bytes 		ser 7 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 4 usec 		deser 3 usec
	[JSON]:		10 bytes 		ser 59 usec 		deser 6 usec

int5 - value:-533449183297
	[GUS]: 		7 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 4 usec 		deser 3 usec
	[JSON]:		13 bytes 		ser 21 usec 		deser 6 usec

int6 - value:99315901321300
	[GUS]: 		8 bytes 		ser 8 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 4 usec 		deser 3 usec
	[JSON]:		14 bytes 		ser 38 usec 		deser 7 usec

int7 - value:17047537962766401
	[GUS]: 		9 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 4 usec 		deser 4 usec
	[JSON]:		17 bytes 		ser 19 usec 		deser 7 usec

int8 - value:-8838969341549745360
	[GUS]: 		11 bytes 		ser 21 usec 		deser 4 usec
	[Native]: 	12 bytes 		ser 5 usec 		deser 4 usec
	[JSON]:		20 bytes 		ser 20 usec 		deser 21 usec

float - value:-105.0532
	[GUS]: 		9 bytes 		ser 8 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 5 usec 		deser 4 usec
	[JSON]:		9 bytes 		ser 32 usec 		deser 8 usec

String - value:test 测试
	[GUS]: 		13 bytes 		ser 35 usec 		deser 6 usec
	[Native]: 	20 bytes 		ser 8 usec 		deser 7 usec
	[JSON]:		13 bytes 		ser 26 usec 		deser 12 usec

StringName - value:TestStringName
	[GUS]: 		16 bytes 		ser 17 usec 		deser 10 usec
	[Native]: 	24 bytes 		ser 8 usec 		deser 23 usec
	[JSON]:		16 bytes 		ser 27 usec 		deser 25 usec

Vector2 - value:(-423, 46.8005)
	[GUS]: 		9 bytes 		ser 9 usec 		deser 4 usec
	[Native]: 	12 bytes 		ser 6 usec 		deser 6 usec
	[JSON]:		17 bytes 		ser 42 usec 		deser 12 usec

Vector2i - value:(-423, 23611)
	[GUS]: 		9 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 6 usec 		deser 5 usec
	[JSON]:		15 bytes 		ser 35 usec 		deser 12 usec

Vector3 - value:(52011, -541.3327, 77441)
	[GUS]: 		13 bytes 		ser 9 usec 		deser 5 usec
	[Native]: 	16 bytes 		ser 7 usec 		deser 6 usec
	[JSON]:		27 bytes 		ser 47 usec 		deser 20 usec

Vector3i - value:(-423, 23611, 52011)
	[GUS]: 		13 bytes 		ser 8 usec 		deser 3 usec
	[Native]: 	16 bytes 		ser 6 usec 		deser 5 usec
	[JSON]:		22 bytes 		ser 39 usec 		deser 16 usec

Vector4 - value:(5211, -541.327, 7441, 1.23)
	[GUS]: 		17 bytes 		ser 10 usec 		deser 4 usec
	[Native]: 	20 bytes 		ser 5 usec 		deser 7 usec
	[JSON]:		30 bytes 		ser 52 usec 		deser 15 usec

Vector4i - value:(-42, 49947, 5211, 445)
	[GUS]: 		17 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		25 bytes 		ser 23 usec 		deser 8 usec

Projection - value:
8740.2373046875, 0, 0, 0
0, 441.044006347656, 0, 0
0, 0, 1, 0
56.6220016479492, -77.8499984741211, 0, 1
	[GUS]: 		65 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	68 bytes 		ser 3 usec 		deser 6 usec
	[JSON]:		110 bytes 		ser 64 usec 		deser 14 usec

Rect2 - value:[P: (56.622, -77.85), S: (8740.237, 441.044)]
	[GUS]: 		17 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		47 bytes 		ser 33 usec 		deser 10 usec

Rect2i - value:[P: (-1731580097, 1577736616), S: (622087478, 177854727)]
	[GUS]: 		17 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		59 bytes 		ser 26 usec 		deser 11 usec

Transform2D - value:[X: (1, 0), Y: (0, -1), O: (513, 88.5)]
	[GUS]: 		25 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	28 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		41 bytes 		ser 35 usec 		deser 9 usec

Plane - value:[N: (-0.022075, 0.354495, 0.934797), D: 0.301078]
	[GUS]: 		17 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		51 bytes 		ser 31 usec 		deser 10 usec

Quaternion - value:(-54.55, 876, 2310, 744)
	[GUS]: 		17 bytes 		ser 4 usec 		deser 3 usec
	[Native]: 	20 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		26 bytes 		ser 25 usec 		deser 8 usec

AABB - value:[P: (-77.85, 8740.237, 441.044), S: (105, 50, 68)]
	[GUS]: 		25 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	28 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		52 bytes 		ser 35 usec 		deser 10 usec

Basis - value:[X: (1, 0, 0), Y: (0, -1, 0), Z: (0, 0, 1)]
	[GUS]: 		37 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	40 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		45 bytes 		ser 35 usec 		deser 10 usec

Transform3D - value:[X: (1, 0, 0), Y: (0, -1, 0), Z: (0, 0, 1), O: (513, -88.5, 105)]
	[GUS]: 		49 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	52 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		67 bytes 		ser 48 usec 		deser 11 usec

Color - value:(1, 1, 1, 1)
	[GUS]: 		33 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		14 bytes 		ser 27 usec 		deser 7 usec

NodePath - value:/root/@EditorNode@17637/@Control@697/@Panel@698
	[GUS]: 		49 bytes 		ser 17 usec 		deser 20 usec
	[Native]: 	80 bytes 		ser 13 usec 		deser 13 usec
	[JSON]:		49 bytes 		ser 21 usec 		deser 9 usec

Array - value:[574, -21, (-423, 46.8005), (0, 1, 1, 1), "acxx"]
	[GUS]: 		62 bytes 		ser 16 usec 		deser 9 usec
	[Native]: 	68 bytes 		ser 7 usec 		deser 11 usec
	[JSON]:		49 bytes 		ser 57 usec 		deser 16 usec

Dictionary - value:{ "k_str": "value1", "k_color": (0.9412, 0.9725, 1, 1), "k_int": 123123, "k_float": 44.886, "k_vec3": (52011, -541.3327, 77441), 532: [574, -21, (-423, 46.8005), (0, 1, 1, 1), "acxx"] }
	[GUS]: 		175 bytes 		ser 59 usec 		deser 21 usec
	[Native]: 	236 bytes 		ser 30 usec 		deser 21 usec
	[JSON]:		178 bytes 		ser 131 usec 		deser 40 usec

Empty Array - value:[]
	[GUS]: 		1 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 4 usec 		deser 5 usec
	[JSON]:		2 bytes 		ser 12 usec 		deser 33 usec

Empty Dictionary - value:{  }
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 4 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 44 usec 		deser 5 usec

Empty PackedByteArray - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		4 bytes 		ser 14 usec 		deser 6 usec

Empty PackedInt32Array - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 32 usec
	[JSON]:		2 bytes 		ser 26 usec 		deser 4 usec

Empty PackedInt64Array - Size:0
	[GUS]: 		1 bytes 		ser 19 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 12 usec 		deser 4 usec

Empty PackedFloat32Array - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 2 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		2 bytes 		ser 27 usec 		deser 5 usec

Empty PackedFloat64Array - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		2 bytes 		ser 12 usec 		deser 5 usec

Empty PackedStringArray - Size:0
	[GUS]: 		1 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		2 bytes 		ser 13 usec 		deser 5 usec

Empty PackedVector2Array - Size:0
	[GUS]: 		1 bytes 		ser 21 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		4 bytes 		ser 29 usec 		deser 5 usec

Empty PackedVector3Array - Size:0
	[GUS]: 		1 bytes 		ser 7 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		4 bytes 		ser 15 usec 		deser 5 usec

Empty PackedColorArray - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		4 bytes 		ser 14 usec 		deser 5 usec

PackedByteArray - Size:4096
	[GUS]: 		4099 bytes 		ser 14 usec 		deser 3 usec
	[Native]: 	4104 bytes 		ser 5 usec 		deser 14 usec
	[JSON]:		18727 bytes 		ser 3470 usec 		deser 1359 usec

PackedInt32Array - Size:4096
	[GUS]: 		16387 bytes 		ser 14 usec 		deser 5 usec
	[Native]: 	16392 bytes 		ser 40 usec 		deser 51 usec
	[JSON]:		45003 bytes 		ser 6925 usec 		deser 1402 usec

PackedInt64Array - Size:4096
	[GUS]: 		32771 bytes 		ser 17 usec 		deser 8 usec
	[Native]: 	32776 bytes 		ser 71 usec 		deser 90 usec
	[JSON]:		81490 bytes 		ser 8280 usec 		deser 1784 usec

PackedFloat32Array - Size:4096
	[GUS]: 		16387 bytes 		ser 15 usec 		deser 7 usec
	[Native]: 	16392 bytes 		ser 52 usec 		deser 58 usec
	[JSON]:		73706 bytes 		ser 10798 usec 		deser 1777 usec

PackedFloat64Array - Size:4096
	[GUS]: 		32771 bytes 		ser 21 usec 		deser 8 usec
	[Native]: 	32776 bytes 		ser 81 usec 		deser 102 usec
	[JSON]:		80060 bytes 		ser 11074 usec 		deser 1916 usec

PackedStringArray - Size:4096
	[GUS]: 		23472 bytes 		ser 11002 usec 		deser 1306 usec
	[Native]: 	48764 bytes 		ser 4673 usec 		deser 1933 usec
	[JSON]:		31662 bytes 		ser 14393 usec 		deser 6405 usec

PackedVector2Array - Size:4096
	[GUS]: 		32771 bytes 		ser 21 usec 		deser 54 usec
	[Native]: 	32776 bytes 		ser 115 usec 		deser 155 usec
	[JSON]:		89200 bytes 		ser 25615 usec 		deser 6728 usec

PackedVector3Array - Size:4096
	[GUS]: 		49155 bytes 		ser 22 usec 		deser 59 usec
	[Native]: 	49160 bytes 		ser 188 usec 		deser 204 usec
	[JSON]:		129617 bytes 		ser 35702 usec 		deser 9644 usec

PackedColorArray - Size:4096
	[GUS]: 		65539 bytes 		ser 24 usec 		deser 41 usec
	[Native]: 	65544 bytes 		ser 230 usec 		deser 233 usec
	[JSON]:		137434 bytes 		ser 45701 usec 		deser 10435 usec

Empty Array[bool] - Size:0
	[GUS]: 		1 bytes 		ser 13 usec 		deser 5 usec
	[Native]: 	8 bytes 		ser 4 usec 		deser 7 usec
	[JSON]:		2 bytes 		ser 20 usec 		deser 8 usec

Empty Array[int] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 5 usec

Empty Array[float] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 5 usec

Empty Array[String] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[StringName] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 19 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Vector2] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 11 usec 		deser 5 usec

Empty Array[Vector2i] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 4 usec

Empty Array[Vector3] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 19 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Vector3i] - Size:0
	[GUS]: 		1 bytes 		ser 28 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 25 usec 		deser 5 usec

Empty Array[Vector4] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 26 usec

Empty Array[Vector4i] - Size:0
	[GUS]: 		1 bytes 		ser 20 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Projection] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Rect2] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Rect2i] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 12 usec 		deser 6 usec

Empty Array[Transform2D] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Plane] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Quaternion] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 11 usec 		deser 5 usec

Empty Array[AABB] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Basis] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 7 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Transform3D] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Color] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[NodePath] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[Array] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 4 usec

Empty Array[Dictionary] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[PackedByteArray] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 4 usec

Empty Array[PackedInt32Array] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 9 usec 		deser 4 usec

Empty Array[PackedInt64Array] - Size:0
	[GUS]: 		1 bytes 		ser 7 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 11 usec 		deser 6 usec

Empty Array[PackedFloat32Array] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[PackedFloat64Array] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[PackedStringArray] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Empty Array[PackedVector2Array] - Size:0
	[GUS]: 		1 bytes 		ser 5 usec 		deser 3 usec
	[Native]: 	8 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 11 usec 		deser 5 usec

Empty Array[PackedVector3Array] - Size:0
	[GUS]: 		1 bytes 		ser 27 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 4 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 25 usec 		deser 5 usec

Empty Array[PackedColorArray] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 4 usec
	[Native]: 	8 bytes 		ser 3 usec 		deser 4 usec
	[JSON]:		2 bytes 		ser 10 usec 		deser 5 usec

Array[bool] - Size:4096
	[GUS]: 		4099 bytes 		ser 174 usec 		deser 483 usec
	[Native]: 	32776 bytes 		ser 371 usec 		deser 862 usec
	[JSON]:		22489 bytes 		ser 4573 usec 		deser 6673 usec

Array[int] - Size:4096
	[GUS]: 		20349 bytes 		ser 307 usec 		deser 733 usec
	[Native]: 	40916 bytes 		ser 396 usec 		deser 884 usec
	[JSON]:		44016 bytes 		ser 6665 usec 		deser 1415 usec

Array[float] - Size:4096
	[GUS]: 		32771 bytes 		ser 241 usec 		deser 682 usec
	[Native]: 	49160 bytes 		ser 395 usec 		deser 892 usec
	[JSON]:		71205 bytes 		ser 10624 usec 		deser 1776 usec

Array[String] - Size:4096
	[GUS]: 		60336 bytes 		ser 14328 usec 		deser 3574 usec
	[Native]: 	97916 bytes 		ser 6063 usec 		deser 4316 usec
	[JSON]:		68526 bytes 		ser 14677 usec 		deser 9917 usec

Array[StringName] - Size:4096
	[GUS]: 		80816 bytes 		ser 15728 usec 		deser 8368 usec
	[Native]: 	114660 bytes 		ser 7127 usec 		deser 7744 usec
	[JSON]:		89006 bytes 		ser 14859 usec 		deser 12014 usec

Array[Vector2] - Size:4096
	[GUS]: 		32771 bytes 		ser 275 usec 		deser 707 usec
	[Native]: 	49160 bytes 		ser 356 usec 		deser 993 usec
	[JSON]:		108871 bytes 		ser 30304 usec 		deser 13520 usec

Array[Vector2i] - Size:4096
	[GUS]: 		32771 bytes 		ser 279 usec 		deser 725 usec
	[Native]: 	49160 bytes 		ser 347 usec 		deser 1001 usec
	[JSON]:		110355 bytes 		ser 30188 usec 		deser 13701 usec

Array[Vector3] - Size:4096
	[GUS]: 		49155 bytes 		ser 323 usec 		deser 780 usec
	[Native]: 	65544 bytes 		ser 438 usec 		deser 1246 usec
	[JSON]:		157291 bytes 		ser 38420 usec 		deser 17914 usec

Array[Vector3i] - Size:4096
	[GUS]: 		49155 bytes 		ser 309 usec 		deser 765 usec
	[Native]: 	65544 bytes 		ser 409 usec 		deser 1349 usec
	[JSON]:		159449 bytes 		ser 39000 usec 		deser 17880 usec

Array[Vector4] - Size:4096
	[GUS]: 		65539 bytes 		ser 306 usec 		deser 753 usec
	[Native]: 	81928 bytes 		ser 447 usec 		deser 1147 usec
	[JSON]:		205632 bytes 		ser 46500 usec 		deser 21697 usec

Array[Vector4i] - Size:4096
	[GUS]: 		65539 bytes 		ser 308 usec 		deser 776 usec
	[Native]: 	81928 bytes 		ser 430 usec 		deser 1228 usec
	[JSON]:		208552 bytes 		ser 46064 usec 		deser 21370 usec

Array[Projection] - Size:4096
	[GUS]: 		262147 bytes 		ser 664 usec 		deser 2017 usec
	[Native]: 	278536 bytes 		ser 754 usec 		deser 4059 usec
	[JSON]:		787456 bytes 		ser 205315 usec 		deser 65828 usec

Array[Rect2] - Size:4096
	[GUS]: 		65539 bytes 		ser 319 usec 		deser 860 usec
	[Native]: 	81928 bytes 		ser 476 usec 		deser 1473 usec
	[JSON]:		246734 bytes 		ser 59359 usec 		deser 24957 usec

Array[Rect2i] - Size:4096
	[GUS]: 		65539 bytes 		ser 335 usec 		deser 783 usec
	[Native]: 	81928 bytes 		ser 443 usec 		deser 1328 usec
	[JSON]:		249476 bytes 		ser 58861 usec 		deser 25123 usec

Array[Transform2D] - Size:4096
	[GUS]: 		98307 bytes 		ser 369 usec 		deser 1785 usec
	[Native]: 	114696 bytes 		ser 455 usec 		deser 3035 usec
	[JSON]:		363907 bytes 		ser 82450 usec 		deser 34703 usec

Array[Plane] - Size:4096
	[GUS]: 		65539 bytes 		ser 323 usec 		deser 786 usec
	[Native]: 	81928 bytes 		ser 476 usec 		deser 1457 usec
	[JSON]:		238517 bytes 		ser 52076 usec 		deser 24104 usec

Array[Quaternion] - Size:4096
	[GUS]: 		65539 bytes 		ser 307 usec 		deser 749 usec
	[Native]: 	81928 bytes 		ser 450 usec 		deser 1133 usec
	[JSON]:		132285 bytes 		ser 42360 usec 		deser 15375 usec

Array[AABB] - Size:4096
	[GUS]: 		98307 bytes 		ser 336 usec 		deser 1683 usec
	[Native]: 	114696 bytes 		ser 555 usec 		deser 3063 usec
	[JSON]:		343269 bytes 		ser 75734 usec 		deser 32661 usec

Array[Basis] - Size:4096
	[GUS]: 		147459 bytes 		ser 399 usec 		deser 1700 usec
	[Native]: 	163848 bytes 		ser 504 usec 		deser 3277 usec
	[JSON]:		321826 bytes 		ser 111807 usec 		deser 30873 usec

Array[Transform3D] - Size:4096
	[GUS]: 		196611 bytes 		ser 430 usec 		deser 1910 usec
	[Native]: 	213000 bytes 		ser 620 usec 		deser 3261 usec
	[JSON]:		674307 bytes 		ser 140458 usec 		deser 75629 usec

Array[Color] - Size:4096
	[GUS]: 		65539 bytes 		ser 323 usec 		deser 794 usec
	[Native]: 	81928 bytes 		ser 466 usec 		deser 1239 usec
	[JSON]:		140650 bytes 		ser 56070 usec 		deser 16271 usec

Array[NodePath] - Size:4096
	[GUS]: 		196611 bytes 		ser 37758 usec 		deser 41295 usec
	[Native]: 	327688 bytes 		ser 33466 usec 		deser 30390 usec
	[JSON]:		204801 bytes 		ser 26301 usec 		deser 21529 usec

Array[Array] - Size:4096
	[GUS]: 		192218 bytes 		ser 73002 usec 		deser 29718 usec
	[Native]: 	319220 bytes 		ser 29915 usec 		deser 23864 usec
	[JSON]:		388428 bytes 		ser 129866 usec 		deser 50159 usec

Array[Dictionary] - Size:4096
	[GUS]: 		229045 bytes 		ser 91394 usec 		deser 32654 usec
	[Native]: 	401192 bytes 		ser 41261 usec 		deser 29649 usec
	[JSON]:		500707 bytes 		ser 178698 usec 		deser 71691 usec

Array[PackedByteArray] - Size:4096
	[GUS]: 		20467 bytes 		ser 1701 usec 		deser 6355 usec
	[Native]: 	49144 bytes 		ser 1125 usec 		deser 4015 usec
	[JSON]:		87026 bytes 		ser 32037 usec 		deser 11987 usec

Array[PackedInt32Array] - Size:4096
	[GUS]: 		135043 bytes 		ser 6099 usec 		deser 6396 usec
	[Native]: 	98248 bytes 		ser 1167 usec 		deser 4195 usec
	[JSON]:		188019 bytes 		ser 50042 usec 		deser 13243 usec

Array[PackedInt64Array] - Size:4096
	[GUS]: 		167779 bytes 		ser 2342 usec 		deser 6391 usec
	[Native]: 	196456 bytes 		ser 1554 usec 		deser 4490 usec
	[JSON]:		416463 bytes 		ser 65267 usec 		deser 16791 usec

Array[PackedFloat32Array] - Size:4096
	[GUS]: 		85939 bytes 		ser 1948 usec 		deser 6187 usec
	[Native]: 	114616 bytes 		ser 1132 usec 		deser 4500 usec
	[JSON]:		229645 bytes 		ser 77703 usec 		deser 14705 usec

Array[PackedFloat64Array] - Size:4096
	[GUS]: 		167779 bytes 		ser 2701 usec 		deser 6467 usec
	[Native]: 	196456 bytes 		ser 1650 usec 		deser 5091 usec
	[JSON]:		399069 bytes 		ser 78333 usec 		deser 16594 usec

Array[PackedStringArray] - Size:4096
	[GUS]: 		73663 bytes 		ser 34877 usec 		deser 11118 usec
	[Native]: 	196456 bytes 		ser 13536 usec 		deser 12134 usec
	[JSON]:		110497 bytes 		ser 74116 usec 		deser 24973 usec

Array[PackedVector2Array] - Size:4096
	[GUS]: 		135043 bytes 		ser 2321 usec 		deser 7095 usec
	[Native]: 	163720 bytes 		ser 1469 usec 		deser 5512 usec
	[JSON]:		431431 bytes 		ser 95646 usec 		deser 39718 usec

Array[PackedVector3Array] - Size:4096
	[GUS]: 		200515 bytes 		ser 2756 usec 		deser 6747 usec
	[Native]: 	229192 bytes 		ser 2164 usec 		deser 4958 usec
	[JSON]:		624635 bytes 		ser 128803 usec 		deser 54449 usec

Array[PackedColorArray] - Size:4096
	[GUS]: 		331459 bytes 		ser 2359 usec 		deser 6938 usec
	[Native]: 	360136 bytes 		ser 2229 usec 		deser 5728 usec
	[JSON]:		698894 bytes 		ser 248205 usec 		deser 59321 usec
```
