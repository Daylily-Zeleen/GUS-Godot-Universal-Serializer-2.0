
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

## Others

If this plugin can help you, please consider to [buy me a coffee](https://afdian.com/a/Daylily-Zeleen).

# Benchmark 2.2.0:
```
null - value:<null>
	[GUS]: 		1 bytes 		ser 4 usec 		deser 1 usec
	[Native]: 	4 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		4 bytes 		ser 5 usec 		deser 3 usec

bool - value:true
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 3 usec 		deser 2 usec

bool - value:false
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		5 bytes 		ser 3 usec 		deser 2 usec

int1 - value:122
	[GUS]: 		3 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		3 bytes 		ser 3 usec 		deser 1 usec

int2 - value:23611
	[GUS]: 		4 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		5 bytes 		ser 4 usec 		deser 1 usec

int3 - value:-8016784
	[GUS]: 		5 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		8 bytes 		ser 30 usec 		deser 1 usec

int4 - value:1783875844
	[GUS]: 		6 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 0 usec
	[JSON]:		10 bytes 		ser 3 usec 		deser 1 usec

int5 - value:-533449183297
	[GUS]: 		7 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		13 bytes 		ser 3 usec 		deser 1 usec

int6 - value:99315901321300
	[GUS]: 		8 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 0 usec 		deser 0 usec
	[JSON]:		14 bytes 		ser 3 usec 		deser 1 usec

int7 - value:17047537962766401
	[GUS]: 		9 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		17 bytes 		ser 6 usec 		deser 1 usec

int8 - value:-8838969341549745360
	[GUS]: 		11 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 0 usec 		deser 0 usec
	[JSON]:		20 bytes 		ser 3 usec 		deser 2 usec

float - value:-105.0532
	[GUS]: 		9 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		9 bytes 		ser 10 usec 		deser 1 usec

String - value:test 测试
	[GUS]: 		13 bytes 		ser 4 usec 		deser 2 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		13 bytes 		ser 5 usec 		deser 2 usec

StringName - value:TestStringName
	[GUS]: 		16 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	24 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		16 bytes 		ser 20 usec 		deser 2 usec

Vector2 - value:(-423, 46.8005)
	[GUS]: 		9 bytes 		ser 1 usec 		deser 0 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		17 bytes 		ser 9 usec 		deser 2 usec

Vector2i - value:(-423, 23611)
	[GUS]: 		9 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		15 bytes 		ser 7 usec 		deser 2 usec

Vector3 - value:(52011, -541.3327, 77441)
	[GUS]: 		13 bytes 		ser 1 usec 		deser 0 usec
	[Native]: 	16 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		27 bytes 		ser 11 usec 		deser 3 usec

Vector3i - value:(-423, 23611, 52011)
	[GUS]: 		13 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	16 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		22 bytes 		ser 7 usec 		deser 2 usec

Vector4 - value:(5211, -541.327, 7441, 1.23)
	[GUS]: 		17 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		30 bytes 		ser 12 usec 		deser 2 usec

Vector4i - value:(-42, 49947, 5211, 445)
	[GUS]: 		17 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		25 bytes 		ser 8 usec 		deser 2 usec

Projection - value:
8740.2373046875, 0, 0, 0
0, 441.044006347656, 0, 0
0, 0, 1, 0
56.6220016479492, -77.8499984741211, 0, 1
	[GUS]: 		65 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	68 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		110 bytes 		ser 40 usec 		deser 28 usec

Rect2 - value:[P: (56.622, -77.85), S: (8740.237, 441.044)]
	[GUS]: 		17 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		47 bytes 		ser 15 usec 		deser 3 usec

Rect2i - value:[P: (-495747255, 323484462), S: (1855859115, 474812983)]
	[GUS]: 		17 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		58 bytes 		ser 11 usec 		deser 2 usec

Transform2D - value:[X: (1, 0), Y: (0, -1), O: (513, 88.5)]
	[GUS]: 		25 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	28 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		41 bytes 		ser 13 usec 		deser 3 usec

Plane - value:[N: (-0.022075, 0.354495, 0.934797), D: 0.301078]
	[GUS]: 		17 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		51 bytes 		ser 30 usec 		deser 3 usec

Quaternion - value:(-54.55, 876, 2310, 744)
	[GUS]: 		17 bytes 		ser 15 usec 		deser 1 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		26 bytes 		ser 13 usec 		deser 3 usec

AABB - value:[P: (-77.85, 8740.237, 441.044), S: (105, 50, 68)]
	[GUS]: 		25 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	28 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		52 bytes 		ser 14 usec 		deser 3 usec

Basis - value:[X: (1, 0, 0), Y: (0, -1, 0), Z: (0, 0, 1)]
	[GUS]: 		37 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	40 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		45 bytes 		ser 13 usec 		deser 2 usec

Transform3D - value:[X: (1, 0, 0), Y: (0, -1, 0), Z: (0, 0, 1), O: (513, -88.5, 105)]
	[GUS]: 		49 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	52 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		67 bytes 		ser 16 usec 		deser 3 usec

Color - value:(1, 1, 1, 1)
	[GUS]: 		33 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	20 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		14 bytes 		ser 25 usec 		deser 1 usec

NodePath - value:/root/@EditorNode@16886/@Panel@13
	[GUS]: 		35 bytes 		ser 5 usec 		deser 4 usec
	[Native]: 	64 bytes 		ser 3 usec 		deser 3 usec
	[JSON]:		35 bytes 		ser 7 usec 		deser 3 usec

Array - value:[574, -21, (-423, 46.8005), (0, 1, 1, 1), "acxx"]
	[GUS]: 		62 bytes 		ser 8 usec 		deser 6 usec
	[Native]: 	68 bytes 		ser 3 usec 		deser 5 usec
	[JSON]:		49 bytes 		ser 60 usec 		deser 7 usec

Dictionary - value:{ "k_str": "value1", "k_color": (0.9412, 0.9725, 1, 1), "k_int": 123123, "k_float": 44.886, "k_vec3": (52011, -541.3327, 77441), 532: [574, -21, (-423, 46.8005), (0, 1, 1, 1), "acxx"] }
	[GUS]: 		174 bytes 		ser 17 usec 		deser 7 usec
	[Native]: 	236 bytes 		ser 7 usec 		deser 6 usec
	[JSON]:		178 bytes 		ser 45 usec 		deser 12 usec

Empty Array - value:[]
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Dictionary - value:{  }
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 5 usec 		deser 1 usec

Empty PackedByteArray - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 5 usec 		deser 1 usec

Empty PackedInt32Array - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 4 usec 		deser 1 usec

Empty PackedInt64Array - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty PackedFloat32Array - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty PackedFloat64Array - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty PackedStringArray - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty PackedVector2Array - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 0 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 5 usec 		deser 1 usec

Empty PackedVector3Array - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 5 usec 		deser 1 usec

Empty PackedVector4Array - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 0 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 4 usec 		deser 2 usec

Empty PackedColorArray - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	8 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		4 bytes 		ser 5 usec 		deser 1 usec

PackedByteArray - Size:4096
	[GUS]: 		4099 bytes 		ser 5 usec 		deser 2 usec
	[Native]: 	4104 bytes 		ser 1 usec 		deser 4 usec
	[JSON]:		18695 bytes 		ser 890 usec 		deser 286 usec

PackedInt32Array - Size:4096
	[GUS]: 		16387 bytes 		ser 8 usec 		deser 3 usec
	[Native]: 	16392 bytes 		ser 9 usec 		deser 14 usec
	[JSON]:		44955 bytes 		ser 2028 usec 		deser 389 usec

PackedInt64Array - Size:4096
	[GUS]: 		32771 bytes 		ser 9 usec 		deser 4 usec
	[Native]: 	32776 bytes 		ser 14 usec 		deser 23 usec
	[JSON]:		81571 bytes 		ser 3407 usec 		deser 562 usec

PackedFloat32Array - Size:4096
	[GUS]: 		16387 bytes 		ser 10 usec 		deser 4 usec
	[Native]: 	16392 bytes 		ser 9 usec 		deser 15 usec
	[JSON]:		73718 bytes 		ser 4150 usec 		deser 529 usec

PackedFloat64Array - Size:4096
	[GUS]: 		32771 bytes 		ser 9 usec 		deser 5 usec
	[Native]: 	32776 bytes 		ser 13 usec 		deser 22 usec
	[JSON]:		80128 bytes 		ser 5368 usec 		deser 474 usec

PackedStringArray - Size:4096
	[GUS]: 		23472 bytes 		ser 2895 usec 		deser 418 usec
	[Native]: 	48764 bytes 		ser 988 usec 		deser 600 usec
	[JSON]:		31662 bytes 		ser 3788 usec 		deser 1740 usec

PackedVector2Array - Size:4096
	[GUS]: 		32771 bytes 		ser 10 usec 		deser 5 usec
	[Native]: 	32776 bytes 		ser 17 usec 		deser 31 usec
	[JSON]:		89182 bytes 		ser 9463 usec 		deser 1346 usec

PackedVector3Array - Size:4096
	[GUS]: 		49155 bytes 		ser 17 usec 		deser 11 usec
	[Native]: 	49160 bytes 		ser 26 usec 		deser 40 usec
	[JSON]:		129652 bytes 		ser 13254 usec 		deser 1829 usec

PackedVector4Array - Size:4096
	[GUS]: 		65539 bytes 		ser 14 usec 		deser 7 usec
	[Native]: 	65544 bytes 		ser 35 usec 		deser 46 usec
	[JSON]:		170203 bytes 		ser 18130 usec 		deser 2678 usec

PackedColorArray - Size:4096
	[GUS]: 		65539 bytes 		ser 12 usec 		deser 7 usec
	[Native]: 	65544 bytes 		ser 33 usec 		deser 46 usec
	[JSON]:		137407 bytes 		ser 15698 usec 		deser 2166 usec

Empty Array[bool] - Size:0
	[GUS]: 		1 bytes 		ser 6 usec 		deser 3 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 5 usec 		deser 3 usec

Empty Array[int] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 2 usec

Empty Array[float] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 2 usec

Empty Array[String] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 2 usec

Empty Array[StringName] - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Vector2] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Vector2i] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Vector3] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Vector3i] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Vector4] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Vector4i] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 2 usec

Empty Array[Projection] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec

Empty Array[Rect2] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Rect2i] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 2 usec

Empty Array[Transform2D] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Plane] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Quaternion] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[AABB] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Basis] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Transform3D] - Size:0
	[GUS]: 		1 bytes 		ser 3 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 3 usec
	[JSON]:		2 bytes 		ser 4 usec 		deser 2 usec

Empty Array[Color] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[NodePath] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 29 usec

Empty Array[Dictionary] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[PackedByteArray] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 2 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 4 usec 		deser 1 usec

Empty Array[PackedInt32Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 2 usec

Empty Array[PackedInt64Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[PackedFloat32Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[PackedFloat64Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 2 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 2 usec

Empty Array[PackedStringArray] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[PackedVector2Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 1 usec

Empty Array[PackedVector3Array] - Size:0
	[GUS]: 		1 bytes 		ser 1 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 2 usec 		deser 2 usec

Empty Array[PackedVector4Array] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 1 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Empty Array[PackedColorArray] - Size:0
	[GUS]: 		1 bytes 		ser 2 usec 		deser 1 usec
	[Native]: 	12 bytes 		ser 1 usec 		deser 2 usec
	[JSON]:		2 bytes 		ser 3 usec 		deser 1 usec

Array[bool] - Size:4096
	[GUS]: 		4099 bytes 		ser 33 usec 		deser 114 usec
	[Native]: 	32780 bytes 		ser 58 usec 		deser 210 usec
	[JSON]:		22551 bytes 		ser 984 usec 		deser 1644 usec

Array[int] - Size:4096
	[GUS]: 		20348 bytes 		ser 92 usec 		deser 146 usec
	[Native]: 	41088 bytes 		ser 109 usec 		deser 183 usec
	[JSON]:		43977 bytes 		ser 2496 usec 		deser 336 usec

Array[float] - Size:4096
	[GUS]: 		32771 bytes 		ser 47 usec 		deser 143 usec
	[Native]: 	49164 bytes 		ser 79 usec 		deser 179 usec
	[JSON]:		71202 bytes 		ser 4660 usec 		deser 468 usec

Array[String] - Size:4096
	[GUS]: 		60336 bytes 		ser 3188 usec 		deser 837 usec
	[Native]: 	97920 bytes 		ser 1171 usec 		deser 988 usec
	[JSON]:		68526 bytes 		ser 3977 usec 		deser 2668 usec

Array[StringName] - Size:4096
	[GUS]: 		80816 bytes 		ser 3742 usec 		deser 1328 usec
	[Native]: 	114664 bytes 		ser 1273 usec 		deser 1371 usec
	[JSON]:		89006 bytes 		ser 3939 usec 		deser 3662 usec

Array[Vector2] - Size:4096
	[GUS]: 		32771 bytes 		ser 72 usec 		deser 142 usec
	[Native]: 	49164 bytes 		ser 67 usec 		deser 187 usec
	[JSON]:		108976 bytes 		ser 8969 usec 		deser 3717 usec

Array[Vector2i] - Size:4096
	[GUS]: 		32771 bytes 		ser 69 usec 		deser 144 usec
	[Native]: 	49164 bytes 		ser 68 usec 		deser 181 usec
	[JSON]:		110456 bytes 		ser 9709 usec 		deser 3760 usec

Array[Vector3] - Size:4096
	[GUS]: 		49155 bytes 		ser 67 usec 		deser 144 usec
	[Native]: 	65548 bytes 		ser 80 usec 		deser 194 usec
	[JSON]:		157311 bytes 		ser 12737 usec 		deser 4907 usec

Array[Vector3i] - Size:4096
	[GUS]: 		49155 bytes 		ser 71 usec 		deser 146 usec
	[Native]: 	65548 bytes 		ser 78 usec 		deser 195 usec
	[JSON]:		159468 bytes 		ser 12224 usec 		deser 4942 usec

Array[Vector4] - Size:4096
	[GUS]: 		65539 bytes 		ser 80 usec 		deser 143 usec
	[Native]: 	81932 bytes 		ser 83 usec 		deser 201 usec
	[JSON]:		205795 bytes 		ser 15415 usec 		deser 5652 usec

Array[Vector4i] - Size:4096
	[GUS]: 		65539 bytes 		ser 79 usec 		deser 152 usec
	[Native]: 	81932 bytes 		ser 79 usec 		deser 202 usec
	[JSON]:		208572 bytes 		ser 15012 usec 		deser 5801 usec

Array[Projection] - Size:4096
	[GUS]: 		262147 bytes 		ser 173 usec 		deser 348 usec
	[Native]: 	278540 bytes 		ser 88 usec 		deser 643 usec
	[JSON]:		787250 bytes 		ser 91675 usec 		deser 14968 usec

Array[Rect2] - Size:4096
	[GUS]: 		65539 bytes 		ser 62 usec 		deser 143 usec
	[Native]: 	81932 bytes 		ser 78 usec 		deser 199 usec
	[JSON]:		246778 bytes 		ser 18537 usec 		deser 6139 usec

Array[Rect2i] - Size:4096
	[GUS]: 		65539 bytes 		ser 64 usec 		deser 157 usec
	[Native]: 	81932 bytes 		ser 81 usec 		deser 205 usec
	[JSON]:		249525 bytes 		ser 18878 usec 		deser 6342 usec

Array[Transform2D] - Size:4096
	[GUS]: 		98307 bytes 		ser 85 usec 		deser 312 usec
	[Native]: 	114700 bytes 		ser 64 usec 		deser 517 usec
	[JSON]:		363715 bytes 		ser 26511 usec 		deser 8389 usec

Array[Plane] - Size:4096
	[GUS]: 		65539 bytes 		ser 63 usec 		deser 147 usec
	[Native]: 	81932 bytes 		ser 75 usec 		deser 201 usec
	[JSON]:		238430 bytes 		ser 16990 usec 		deser 6046 usec

Array[Quaternion] - Size:4096
	[GUS]: 		65539 bytes 		ser 66 usec 		deser 144 usec
	[Native]: 	81932 bytes 		ser 77 usec 		deser 206 usec
	[JSON]:		133186 bytes 		ser 13225 usec 		deser 4151 usec

Array[AABB] - Size:4096
	[GUS]: 		98307 bytes 		ser 91 usec 		deser 317 usec
	[Native]: 	114700 bytes 		ser 91 usec 		deser 527 usec
	[JSON]:		343170 bytes 		ser 26878 usec 		deser 7971 usec

Array[Basis] - Size:4096
	[GUS]: 		147459 bytes 		ser 121 usec 		deser 322 usec
	[Native]: 	163852 bytes 		ser 69 usec 		deser 558 usec
	[JSON]:		325096 bytes 		ser 35881 usec 		deser 8314 usec

Array[Transform3D] - Size:4096
	[GUS]: 		196611 bytes 		ser 163 usec 		deser 328 usec
	[Native]: 	213004 bytes 		ser 87 usec 		deser 596 usec
	[JSON]:		674187 bytes 		ser 47383 usec 		deser 13641 usec

Array[Color] - Size:4096
	[GUS]: 		65539 bytes 		ser 83 usec 		deser 145 usec
	[Native]: 	81932 bytes 		ser 78 usec 		deser 200 usec
	[JSON]:		140842 bytes 		ser 18388 usec 		deser 4508 usec

Array[NodePath] - Size:4096
	[GUS]: 		139267 bytes 		ser 7720 usec 		deser 4620 usec
	[Native]: 	262156 bytes 		ser 4442 usec 		deser 4665 usec
	[JSON]:		147457 bytes 		ser 7161 usec 		deser 4762 usec

Array[Array] - Size:4096
	[GUS]: 		188115 bytes 		ser 13756 usec 		deser 5799 usec
	[Native]: 	319164 bytes 		ser 6413 usec 		deser 5254 usec
	[JSON]:		388614 bytes 		ser 42421 usec 		deser 12610 usec

Array[Dictionary] - Size:4096
	[GUS]: 		224920 bytes 		ser 16998 usec 		deser 6697 usec
	[Native]: 	401012 bytes 		ser 8713 usec 		deser 6562 usec
	[JSON]:		500670 bytes 		ser 51079 usec 		deser 19330 usec

Array[PackedByteArray] - Size:4096
	[GUS]: 		20467 bytes 		ser 346 usec 		deser 1047 usec
	[Native]: 	49148 bytes 		ser 188 usec 		deser 800 usec
	[JSON]:		87180 bytes 		ser 8138 usec 		deser 3279 usec

Array[PackedInt32Array] - Size:4096
	[GUS]: 		135043 bytes 		ser 968 usec 		deser 1023 usec
	[Native]: 	98252 bytes 		ser 236 usec 		deser 841 usec
	[JSON]:		187799 bytes 		ser 13042 usec 		deser 3384 usec

Array[PackedInt64Array] - Size:4096
	[GUS]: 		167779 bytes 		ser 702 usec 		deser 1160 usec
	[Native]: 	196460 bytes 		ser 293 usec 		deser 1023 usec
	[JSON]:		416358 bytes 		ser 20462 usec 		deser 4530 usec

Array[PackedFloat32Array] - Size:4096
	[GUS]: 		85939 bytes 		ser 755 usec 		deser 1160 usec
	[Native]: 	114620 bytes 		ser 383 usec 		deser 951 usec
	[JSON]:		229848 bytes 		ser 28507 usec 		deser 3958 usec

Array[PackedFloat64Array] - Size:4096
	[GUS]: 		167779 bytes 		ser 725 usec 		deser 1168 usec
	[Native]: 	196460 bytes 		ser 297 usec 		deser 1041 usec
	[JSON]:		399132 bytes 		ser 31626 usec 		deser 4510 usec

Array[PackedStringArray] - Size:4096
	[GUS]: 		73663 bytes 		ser 8261 usec 		deser 2041 usec
	[Native]: 	196460 bytes 		ser 2526 usec 		deser 2817 usec
	[JSON]:		110497 bytes 		ser 18976 usec 		deser 6421 usec

Array[PackedVector2Array] - Size:4096
	[GUS]: 		135043 bytes 		ser 648 usec 		deser 1098 usec
	[Native]: 	163724 bytes 		ser 292 usec 		deser 1047 usec
	[JSON]:		431435 bytes 		ser 29954 usec 		deser 9408 usec

Array[PackedVector3Array] - Size:4096
	[GUS]: 		200515 bytes 		ser 775 usec 		deser 1165 usec
	[Native]: 	229196 bytes 		ser 327 usec 		deser 1069 usec
	[JSON]:		624610 bytes 		ser 42741 usec 		deser 12976 usec

Array[PackedVector4Array] - Size:4096
	[GUS]: 		265987 bytes 		ser 876 usec 		deser 1052 usec
	[Native]: 	294668 bytes 		ser 419 usec 		deser 1095 usec
	[JSON]:		817512 bytes 		ser 52371 usec 		deser 16088 usec

Array[PackedColorArray] - Size:4096
	[GUS]: 		331459 bytes 		ser 830 usec 		deser 1172 usec
	[Native]: 	360140 bytes 		ser 386 usec 		deser 1089 usec
	[JSON]:		698858 bytes 		ser 82079 usec 		deser 15118 usec
```
