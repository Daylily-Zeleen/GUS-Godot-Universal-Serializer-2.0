#!/usr/bin/env python

import sys
import os
import zipfile
import shutil
from os.path import join as path_join


def main():
    args = "scons"
    debug_and_relaese = True
    for arg in sys.argv:
        if arg == "-h" or arg == "--help":
            os.system("scons -h")
            print('\nIf you have not specify "target" argument, this tool will build both debug and relaese.')
            return

        if arg.startswith("python"):
            continue
        if arg == sys.argv[0]:
            continue
        if arg.count("scons") > 0:
            print('Should not use "scons" argument, skip it.')
            continue

        args += " " + arg

        if arg.startswith("target"):
            debug_and_relaese = False

    bin_dir = "bin"
    # Remove all last build files.
    for f in os.listdir(bin_dir):
        os.remove(path_join(bin_dir, f))

    # Buiild.
    if debug_and_relaese:
        print("Building debug version...")
        os.system(args + " target=template_debug")

        print("Building release version...")
        os.system(args + " target=template_release")
    else:
        print("Building...")
        os.system(args)
    print("Build finished, post processiong...")

    # Post process
    plugin_dir = "demo/addons/com.daylily_zeleen.godot_universal_serializer"
    dynamic_lib_suffixs = [".so", ".dylib", ".wasm", ".dll"]

    # Copy dynamic library.
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

    # Zip files.
    zip_file_path = "bin\com.daylily_zeleen.godot_universal_serializer.zip"
    if os.path.exists(zip_file_path):
        os.remove(zip_file_path)
    zip_file = zipfile.ZipFile(zip_file_path, "w")
    zip_files_recursively(zip_file, "demo/addons")
    zip_file.close()

    print("Done!")


def zip_files_recursively(zip_file: zipfile.ZipFile, dir: str):
    for f in os.listdir(dir):
        path = path_join(dir, f)
        if os.path.isdir(path):
            zip_files_recursively(zip_file, path)
        else:
            src_path = path
            if src_path.startswith("demo/"):
                src_path = src_path.replace("demo/", "", 1)
            zip_file.write(path, src_path)


if __name__ == "__main__":
    main()
