@tool
extends EditorPlugin

func _enter_tree() -> void:
	add_autoload_singleton("GUS","res://addons/godot_universal_serializer/GUS.gd")

func _exit_tree() -> void:
	remove_autoload_singleton("GUS")
