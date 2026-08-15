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


# Build a suffix that does NOT include the target (template_debug/template_release),
# so the produced library name is the same regardless of which target the user builds.
suffix = ".{}".format(env["platform"])
if env["dev_build"]:
    suffix += ".dev"
if env["precision"] == "double":
    suffix += ".double"
suffix += ".{}".format(env["arch"])
if env["ios_simulator"]:
    suffix += ".simulator"
if not env["threads"]:
    suffix += ".nothreads"

if env["platform"] == "macos":
    framework_path = "bin/libgus2.macos.framework"
    library = env.SharedLibrary(
        framework_path + "/libgus2.macos",
        source=sources,
    )

    # Build the framework with -dynamiclib so the macOS linker automatically
    # generates the required Resources/Info.plist (this is how godot-cpp's
    # example framework gets its Info.plist - no manual plist is written).
    env.Append(LINKFLAGS=["-dynamiclib"])
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "bin/libgus2.ios.simulator.a",
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "bin/libgus2.ios.a",
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "bin/libgus2{}{}".format(suffix, env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
