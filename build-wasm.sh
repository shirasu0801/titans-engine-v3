#!/bin/bash
# Build script for WebAssembly version of Chess Titans
# Requires Emscripten SDK to be installed and activated

set -e

echo "Building Chess Titans for WebAssembly..."

# Create build directory
mkdir -p wasm-cmake-build
cd wasm-cmake-build

# Configure with Emscripten
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
emmake make -j$(nproc 2>/dev/null || echo 4)

echo ""
echo "Build complete! Files generated in wasm-build/"
echo ""
echo "To test, run:"
echo "  cd web"
echo "  python -m http.server 8080"
echo "Then open http://localhost:8080 in your browser"
