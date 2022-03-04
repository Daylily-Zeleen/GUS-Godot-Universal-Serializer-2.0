
# GUS - Godot Universal Serializer 2.0.0 - Godot的通用序列化器 - V2.0.0

![Image text](icon.png)

[English doc](README.md) <- click here。

为序列化尺寸优化而构建的 Godot 4.x 通用序列化器。
支持除了 `Object`、 `RID`、 `Callable` 以及 `Signal` 以外的 Godot4.x 的所有内建数据类型。

# 特点:
  1. 不丢失类型信息。在跨端传送时不需要类型检测和类型强制转换，就像本地传参一般。
  2. 整形是变长序列化。
  3. 相比 Godot 的原生序列化方法 `var2bytes()` 和转换成`JSON`再转`UTF8`数据, `GUS`的序列化后数据尺寸有显著的减少。尤其适用于多人游戏中需要传输的小型数据结构的序列化。
  3. `GDExtension` 插件，具有较高的性能。

  可以通过运行名为`GUS_test.gd`的编辑器脚本来查看 `GUS`、`Native` 以及 `JSON` 这三种方式的剧烈话反序列化测试对比。

# 如何使用:
  1. 下载发布的插件包，像其他插件一样安装和启用即可。
  2. 根据你的需求代码修改名为`GUS`的Autoload的属性.
  3. 传递不包含`Object`、 `OID`、 `Callable`以及 `Signal`的变量给 `GUS.to_bytes()`并获取序列化后数据，然后发送给其他网络对等体。
  3. 远端的网络对等体获取到序列化后数据，传递给`GUS.to_var()`,即可获得与序列化前相同的变量。


# 注意:
  1. 不同端的`GUS`属性必须保持一致才能正确反序列化。
  2. `Array` 和 `Dictionary` 不能包含 `Object`、`RID`、`Callable`以及`Signal`。
  3. 当前不支持`typeof double real_t`的自定义构建Godot 4.x版本。
  4. 当前仅支持`windows`.
	
# TODO:
  1. 如果我找到绑定静态方法给`GDScript`的办法，我将抛弃自动加载`GUS`, 改为静态方法的方式去实现功能。
  2. 编译`linux`和`osx`版本(但我碰上了我无法处理的编译错误)。
  3. 编译`typeof double real_t`版本以支持自定义构建Godot 4.x版本(但我碰上了我无法处理的编译错误)。

# 如何编译:
  1. 根据[官方文档](https://docs.godotengine.org/zh_CN/stable/development/compiling/index.html)搭建你的编译环境。
  1. 克隆这个仓库(以递归的方式)。
  3. 导航到`godot-cpp`文件夹，运行下方命令:

    cd godot-cpp
    scons target=debug
  4. 导航到仓库根目录,再次运行构建命令 :
    
    scons target=debug
  5. 现在,你能再`\demo\addons\godot_universal_serializer`获得该插件.

# 其他:
我只是一个c++新手，我无法处理上面提到的编译错误。如果有人能帮我解决这些问题，我将非常感谢。