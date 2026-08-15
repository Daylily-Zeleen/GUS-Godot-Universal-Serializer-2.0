#!/bin/sh
# Build the GUS2 extension for iOS, matching the godot-cpp test example packaging:
# build an arm64 device static library and a universal simulator static library,
# then combine them into a single .xcframework (for both the extension and godot-cpp).
set -e

TARGET=template_release

# --- godot-cpp ---
# arm64 device
(cd godot-cpp && scons platform=ios arch=arm64 ios_simulator=no target=$TARGET generate_bindings=yes build_profile=../build_profile.json optimize=speed)
# universal simulator
(cd godot-cpp && scons platform=ios arch=universal ios_simulator=yes target=$TARGET generate_bindings=no build_profile=../build_profile.json optimize=speed)

# --- extension ---
# arm64 device
scons platform=ios arch=arm64 ios_simulator=no target=$TARGET generate_bindings=no build_profile=build_profile.json optimize=speed
# universal simulator
scons platform=ios arch=universal ios_simulator=yes target=$TARGET generate_bindings=no build_profile=build_profile.json optimize=speed

# --- xcframeworks ---
xcodebuild -create-xcframework \
	-library bin/libgus2.ios.a \
	-library bin/libgus2.ios.simulator.a \
	-output bin/libgus2.ios.xcframework

xcodebuild -create-xcframework \
	-library godot-cpp/bin/libgodot-cpp.ios.$TARGET.arm64.a \
	-library godot-cpp/bin/libgodot-cpp.ios.$TARGET.universal.simulator.a \
	-output bin/libgodot-cpp.ios.xcframework

# Clean up intermediate static libraries.
rm -f bin/libgus2.ios.a bin/libgus2.ios.simulator.a \
	godot-cpp/bin/libgodot-cpp.ios.$TARGET.arm64.a \
	godot-cpp/bin/libgodot-cpp.ios.$TARGET.universal.simulator.a

echo "iOS build finished: bin/libgus2.ios.xcframework, bin/libgodot-cpp.ios.xcframework"
