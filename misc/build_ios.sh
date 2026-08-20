#!/bin/sh
# Build the GUS2 extension for iOS as a single arm64 device dynamic library (.dylib).
# godot-cpp is linked into the extension at build time, so no separate
# libgodot-cpp xcframework is needed (matching the approach used by gode).
set -e

TARGET=template_release

# --- godot-cpp (static, used only as link input for the extension) ---
(cd godot-cpp && scons platform=ios arch=arm64 ios_simulator=no target=$TARGET generate_bindings=yes build_profile=../build_profile.json optimize=speed)

# --- extension (dynamic library, godot-cpp linked in) ---
scons platform=ios arch=arm64 ios_simulator=no target=$TARGET generate_bindings=no build_profile=build_profile.json optimize=speed

echo "iOS build finished: bin/libgus2.ios.dylib"
