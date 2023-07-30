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

    bin_dir = "bin/"  # "dist/addons/com.daylily_zeleen.godot_universal_serializer2/bin/"
    # Remove all last build files.
    for f in os.listdir(bin_dir):
        os.remove(path_join(bin_dir, f))

    # Buiild.
    if debug_and_relaese:
        print("Building debug version...")
        os.system(args + " target=template_debug")
        print("")
        print("Building release version...")
        os.system(args + " target=template_release")
    else:
        print("Building...")
        os.system(args)
    print("Build finished, post processiong...")

    # == Post process ==
    # Copy dynamic library to dist dir.
    print("Copy dynamic library to dist dir.")
    dynamic_lib_suffixs = [".so", ".dylib", ".wasm", ".dll"]
    dist_dir = "dist/addons/com.daylily_zeleen.godot_universal_serializer2/"
    dist_bin_dir = path_join(dist_dir, "bin")

    dev_libs = []
    for f in os.listdir("bin/"):
        for suffix in dynamic_lib_suffixs:
            if not f.endswith(suffix):
                continue
            path = path_join(bin_dir, f)
            dist_path = path_join(dist_bin_dir, f)
            if f.count(".dev."):
                dev_libs.append(f)
            else:
                if os.path.isfile(path):
                    shutil.copyfile(path, dist_path)
                else:
                    shutil.copytree(path, dist_path)

    # Try dev libs as debug libs.
    if len(dev_libs) > 0:
        print("Copy and replace dev libs to debug libs.")
        for f in dev_libs:
            path = path_join(bin_dir, f)
            dist_path = path_join(dist_bin_dir, f).replace(".dev.", ".")
            if os.path.isfile(path):
                shutil.copyfile(path, dist_path)
            else:
                shutil.copytree(path, dist_path)

    # Copy readme and license to dist path.
    print("Copy readme and license to dist path.")
    shutil.copyfile("README.md", path_join(dist_dir, "README.md"))
    shutil.copyfile("README_zh_cn.md", path_join(dist_dir, "README_zh_cn.md"))
    shutil.copyfile("LICENSE", path_join(dist_dir, "LICENSE"))

    # Copy plugins to demo for test.
    print("Copy plugins to demo for test.")
    if os.path.exists("demo/addons/"):
        shutil.rmtree("demo/addons/")
    shutil.copytree("dist/addons", "demo/addons")

    print("Done!")


if __name__ == "__main__":
    main()
