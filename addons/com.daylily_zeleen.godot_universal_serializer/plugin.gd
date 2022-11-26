@tool
extends EditorPlugin


func _enable_plugin() -> void:
	print_rich("""[b]Godot Universal Serizalizer[/b] is a GDExtension plugin, not need to enable this editor plugin.
You can use [b]GUS.var_to_bytes()[/b] and [b]GUS.bytes_to_var()[/b] instead of [b]var_to_bytes()[/b] and [b]bytes_to_var()[/b].
You can use [b]GUS.set_color_encode_type()[/b] to compress Color and PackedColorArray further.""")
