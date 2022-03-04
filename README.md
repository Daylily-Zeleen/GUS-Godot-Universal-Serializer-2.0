
# GUS - Godot Universal Serializer 2.00 - Godot的通用序列化器 - V2.00

![Image text](icon.png)

[中文文档](README_zh_cn.md) <- 点击这里。

A Godot's universal serializer for size optimization.    
Support all data types except `Object`, `RID`, `Callable` and `Signal` in Godot 4.x.

# Feature:
  1. Without losing type information. Need not type detection and forced typce conversion in cross end transfer，just like local parameter transfer.
  2. Integer are variable length serialization.
  3. The serialized data size is smaller than JSON text to utf8 and the native serialization method `var2bytes()`.GUS is especially suitable for the serialization of structured small data transfer in multiplayer games.
  4. GDExtension plugin, high performance. 

  You can run the `EditorScript` which named `GUS_test.gd` to view the contrast between `GUS`, `Native`, and `JSON`.

# How to use:
  1. Download release plugin, install and enable this plugin just like other plugin.
  2. Modify properties of autoload which named `GUS` as you require.
  3. Pass variable which without `Object`, `OID`, `Callable`, `Signal` into `GUS.to_bytes()` and get the serialized data, than send to network peer.
  3. After the remote network peer obtains the serialized data, passe it into `GUS.to_var()` to get the same variable as before serialization.


# Be careful:
  1. The properties of autoload `GUS` between all network peers should keep the same.
  2. `Array` and `Dictionary` should not contain `Object`, `RID`, `Callable` and `Signal`.
  3. Currently unsupport Godot 4.x which compiled with `typeof double real_t`.
  4. Currently just support platform `windows`.
	
# TODO:
  1. If I find the way to bind static method for `GDScript`, I will get rid of the autoload `GUS`.
  2. Compile `linux` and `osx` version( but I encounter error which I can't handle it).
  3. Compile `typeof double real_t` version to support custom Godot( but I encounter error which I can't handle it, too).

# How to compile:
  1. Follow [offical tutorial](https://docs.godotengine.org/zh_CN/stable/development/compiling/index.html) to set up you develop enviroment.
  1. Clone this repository (recursive).
  3. Navigate to `godot-cpp` folder, and run command as below :

    cd godot-cpp
    scons target=debug
  4. Navigate to root folder, and run command as below :
    
    scons target=debug
  5. Now, you can get the addon which be located at `\demo\addons\godot_universal_serializer`.

# Other:
I just a c++ beginner, I can't handle above compile error. I will be very grateful to those who deal these problems.