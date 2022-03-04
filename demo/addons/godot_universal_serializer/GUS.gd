@tool
## 包装  GodotUniversalSerializer 的节点，
## 用于作为全局单例 。
## 正常使用应通过该单例进行序列化和反序列化

extends Node

## Color 编码方式
enum ColorEncoding{
	## 精度最低，序列化长度最小
	RGBA32,
	## 精度一般，序列化长度稍长
	RGBA64,
	## 精度最高，序列化长度最长
	RGBA_FLOAT,
}

	
##  决定 Color 类型的编码方式（静态属性，影响所有 GodotUniversalSerializer 实例）。
##  默认 ColorEncoding.RGBA32, 采用 rgba32 进行编码，最小编码长度，最低的颜色表达精度； 
##  设为 ColorEncoding.RGBA64 时采用 rgba64 进行编码，稍大的编码长度，较好的颜色表达精度；
##  设为 ColorEncoding.RGBA_FLOAT 时采用 4个float对每个通道进行编码，最大的的编码长度，最好的颜色表达精度；
##  只有设为 ColorEncoding.RGBA_FLOAT 才能保证序列化前后的值相同（跨平台或者跨架构除外，因为浮点数的不确定性）。
var color_encoding :ColorEncoding = ColorEncoding.RGBA32:
	set(value): __gus.color_rgba64 = value
	get: return __gus.color_rgba64

##  决定 String 的编码方式（静态属性，影响所有 GodotUniversalSerializer 实例）。
##  请根据实际使用的字符集进行设定，
##  默认为  utf8
var string_encoding_utf8:bool = true:
	set(value): __gus.string_encoding_utf8 = value
	get: return __gus.string_encoding_utf8
	
##  决定 StringName 的编码方式（静态属性，影响所有 GodotUniversalSerializer 实例）。
##  请根据实际使用的字符集进行设定，
##  默认为  ascii
var string_name_encoding_utf8:bool = false:
	set(value): __gus.string_name_encoding_utf8 = value
	get: return __gus.string_name_encoding_utf8

##  决定 NodePath 的编码方式（静态属性，影响所有 GodotUniversalSerializer 实例）。
##  请根据实际使用的字符集进行设定，
##  默认为  ascii。
var node_path_encoding_utf8:bool = false:
	set(value): __gus.node_path_encoding_utf8 = value
	get: return __gus.node_path_encoding_utf8

## 将输入参数 序列化为 GUS 数据。
## 注意输入参数 不能是 Object, RID, Callable, Signal 类型，
## 如果输入参数是 容器类型 （Array、Dictionary), 也不能包含上诉类型，
## 否则将会推出错误。
## GUS 暂不支持与对象有关的序列化和反序列化、也暂无计划提供相关支持，
## 可以 通过 GUS.to_bytes(var2bytes_with_objects(variant)）以及 bytes2var_with_objects(GUS.to_var(bytes)) 进行代替。
## 当然这不是最优解，应尽量避免这种用法
func to_bytes(variant: Variant) -> PackedByteArray:
	return __gus.to_bytes(variant)
	
## 反序列化 GUS 数据。
## 如果不是 GUS 能识别的数据将会推出错误。
func to_var(bytes: PackedByteArray) -> Variant:
	return __gus.to_var(bytes)

# =======================================================
# =======================================================

var __gus := GodotUniversialSerializer.new()
func _notification(what: int) -> void:
	if what == NOTIFICATION_PREDELETE:
		__gus.free()
