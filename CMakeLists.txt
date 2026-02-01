cmake_minimum_required(VERSION 3.16)
project(ChessTitans LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC UTF-8 support for Unicode chess symbols
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Core source files (no SFML dependency)
set(CORE_SOURCES
    src/Board.cpp
    src/Piece.cpp
    src/Move.cpp
    src/AI.cpp
    src/Pieces/King.cpp
    src/Pieces/Queen.cpp
    src/Pieces/Rook.cpp
    src/Pieces/Bishop.cpp
    src/Pieces/Knight.cpp
    src/Pieces/Pawn.cpp
)

# Core header files
set(CORE_HEADERS
    src/Board.h
    src/Piece.h
    src/Move.h
    src/AI.h
    src/Pieces/King.h
    src/Pieces/Queen.h
    src/Pieces/Rook.h
    src/Pieces/Bishop.h
    src/Pieces/Knight.h
    src/Pieces/Pawn.h
)

# Check if building with Emscripten for WebAssembly
if(EMSCRIPTEN)
    # WASM build - no SFML, no renderer
    set(WASM_SOURCES
        ${CORE_SOURCES}
        src/wasm/ChessEngine.cpp
    )

    add_executable(chess ${WASM_SOURCES} ${CORE_HEADERS})

    # Emscripten-specific flags
    set_target_properties(chess PROPERTIES
        SUFFIX ".js"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/wasm-build"
    )

    target_compile_options(chess PRIVATE
        -O3
        -fno-exceptions
    )

    # Emscripten linker flags - use "SHELL:" prefix to prevent CMake from splitting arguments
    target_link_options(chess PRIVATE
        --bind
        "SHELL:-s WASM=1"
        "SHELL:-s MODULARIZE=1"
        "SHELL:-s EXPORT_ES6=1"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
        "SHELL:-s ENVIRONMENT=web"
        -O3
    )

    target_include_directories(chess PRIVATE ${CMAKE_SOURCE_DIR}/src)

else()
    # Native build with SFML

    # Find SFML 3.0
    find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

    # Full source files including SFML-dependent ones
    set(SOURCES
        ${CORE_SOURCES}
        src/main.cpp
        src/Game.cpp
        src/Renderer.cpp
    )

    # Full header files
    set(HEADERS
        ${CORE_HEADERS}
        src/Game.h
        src/Renderer.h
    )

    # Create executable
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

    # Link SFML
    target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System)

    # Include directories
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

    # Copy assets to build directory
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

    # Windows-specific: Copy SFML DLLs to output directory
    if(WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SFML::Graphics>
                $<TARGET_FILE:SFML::Window>
                $<TARGET_FILE:SFML::System>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
endif()
