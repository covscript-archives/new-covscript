cmake_minimum_required(VERSION 3.14)
project(covscript_4)

set(CMAKE_MODULE_PATH "${CMCMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/parser/antlr-runtime/cmake")

include(CheckIncludeFiles)
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(CheckCSourceCompiles)
enable_testing()

#### Check C++14
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)
if (COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else ()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif ()
set(CMAKE_CXX_STANDARD 14)

#### Check C99
check_c_compiler_flag("-std=c99" COMPILER_SUPPORTS_C99)
if (COMPILER_SUPPORTS_C99)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
else ()
    message(STATUS "The compiler ${CMAKE_C_COMPILER} has no C99 support. Please use a different C compiler.")
endif ()
set(CMAKE_C_STANDARD 11)

# Bring in the required packages
find_package(antlr4-runtime REQUIRED)
find_package(antlr4-generator REQUIRED)

# Set path to generator
set(ANTLR4_JAR_LOCATION parser/antlr/antlr-4.7.2-complete.jar)

# generate lexer and parser
antlr4_generate(
    covscript_parser
    parser/grammar/CovScript.g4
    BOTH
    TRUE
    TRUE
    "cs"
)

# add directories for generated include files
include_directories(${PROJECT_BINARY_DIR} ${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_covscript_parser})

# add generated source files
add_executable(covscript4
        ${ANTLR4_SRC_FILES_covscript_parser}
        main.cpp
        )

# add required runtime library
add_dependencies(covscript4 antlr4_shared)

target_link_libraries(covscript4 PRIVATE antlr4_shared)
