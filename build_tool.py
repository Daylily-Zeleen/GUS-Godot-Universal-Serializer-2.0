import os
import sys
import shutil
from os.path import join as path_join


build_debug = True
build_release = True

arg1 = ""
arg2 = ""


def main():
    args = "scons"
    debug_and_relaese = True
    for arg in sys.argv:
        if arg == "-h" or arg == "--help":
            os.system("scons -h")
            print("\nIf you have not specify \"target\" argument, this tool will build both debug and relaese. ")
            return

        if arg.startswith("python"):
            continue
        if arg.count("build_tool.py") > 0:
            continue
        if arg.count("scons") > 0:
            print("Should not use \"scons\" argument, skip it.")
            continue

        args += " " + arg

        if arg.startswith("target"):
            debug_and_relaese = False

        if debug_and_relaese:
            print("Building debug version...")
            os.system(args + " target=template_debug")

            print("Building release version...")
            os.system(args + " target=template_release")
        else:
            print("Building...")
            os.system(args)
        print("Build finished!")

        # Post process
        bin_dir = "bin"
        plugin_dir = "demo/addons/com.daylily_zeleen.godot_universal_serializer"
        dynamic_lib_suffixs = [".so", ".dylib", ".wasm", ".dll"]

        # Copy dynamic liberary.
        dst_dir = path_join(plugin_dir, "bin")
        for f in os.listdir("bin"):
            for suffix in dynamic_lib_suffixs:
                if not f.endswith(suffix):
                    continue
                shutil.copyfile(path_join(bin_dir, f), path_join(dst_dir, f))

        # Copy readme and license.
        shutil.copyfile("README.md", path_join(plugin_dir, "README.md"))
        shutil.copyfile("README_zh_cn.md", path_join(plugin_dir, "README_zh_cn.md"))
        shutil.copyfile("LICENSE", path_join(plugin_dir, "LICENSE"))

        shutil.copyfile("README.md", path_join("demo", "README.md"))
        shutil.copyfile("README_zh_cn.md", path_join("demo", "README_zh_cn.md"))
        shutil.copyfile("LICENSE", path_join("demo", "LICENSE"))


"com.daylily_zeleen.godot_universal_serializer/bin"
if __name__ == '__main__':
    main()
