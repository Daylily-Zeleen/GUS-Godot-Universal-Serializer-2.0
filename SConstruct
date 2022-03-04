#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
# env["MSVC_VERSION"] = "11.0"
# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# env["CPPDEFINES"].append("UNICODE")

default_project_path = "demo/"

addon_path = "addons/godot_universal_serializer/bin/"
bin_path = default_project_path + addon_path


# # If REAL_T_IS_DOUBLE
# bin_path += "real_t_is_double/"
# env['CPPDEFINES'].append('REAL_T_IS_DOUBLE')
# # Else

bin_path += "real_t_is_float/"



if env["platform"] == "osx":
    library = env.SharedLibrary(bin_path + "libgus.{}.{}.framework/libgus.{}.{}".format(env["platform"], env["target"], env["platform"], env["target"]), source=sources)
else:
    library = env.SharedLibrary(bin_path + "libgus.{}.{}.{}{}".format(env["platform"], env["target"], env["arch_suffix"], env["SHLIBSUFFIX"]), source=sources)

Default(library)
