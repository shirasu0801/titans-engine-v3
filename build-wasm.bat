@echo off
REM Build script for WebAssembly version of Chess Titans
REM Requires Emscripten SDK to be installed at C:\emsdk

echo Activating Emscripten environment...
call C:\emsdk\emsdk_env.bat

echo Building Chess Titans for WebAssembly...

REM Create build directory
if not exist wasm-cmake-build mkdir wasm-cmake-build
cd wasm-cmake-build

REM Configure with Emscripten
call emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    exit /b %ERRORLEVEL%
)

REM Build
call emmake cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

cd ..

echo.
echo Build complete! Files generated in wasm-build/
echo.
echo To test, run:
echo   serve.bat
echo Then open http://localhost:3000/web/ in your browser
