#!/usr/bin/env python

import os
import sys

env = SConscript("godot-cpp/SConstruct")


# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
# sources = Glob("src/*.cpp")
sources = Glob("src/*.cpp")

# Optionally C++20
# if env.get("is_msvc", False):
#     env["CXXFLAGS"] = ["/std:c++20"]
# else:
#     env["CXXFLAGS"] = ["-std=c++2a"]


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "dist/addons/com.daylily_zeleen.godot_universal_serializer2/bin/libgus2.{}.{}.framework/libguss.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "bin/libgus2{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
