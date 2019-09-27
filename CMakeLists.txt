cmake_minimum_required(VERSION 3.9)
project(covscript_4)

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

#### Check C11
check_c_compiler_flag("-std=c11" COMPILER_SUPPORTS_C11)
if (COMPILER_SUPPORTS_C11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
else ()
    message(STATUS "The compiler ${CMAKE_C_COMPILER} has no C11 support. Please use a different C compiler.")
endif ()
set(CMAKE_C_STANDARD 11)

find_package(Java COMPONENTS Runtime)

set(ANTLR_JAR_LOCATION ${PROJECT_SOURCE_DIR}/compiler/antlr/antlr-4.7.2-complete.jar)

set(compiler-GENERATED_SRC
        ${PROJECT_SOURCE_DIR}/compiler/generated/CovScriptLexer.cpp
        ${PROJECT_SOURCE_DIR}/compiler/generated/CovScriptParser.cpp
        ${PROJECT_SOURCE_DIR}/compiler/generated/CovScriptBaseListener.cpp
        ${PROJECT_SOURCE_DIR}/compiler/generated/CovScriptBaseVisitor.cpp
        ${PROJECT_SOURCE_DIR}/compiler/generated/CovScriptListener.cpp
        ${PROJECT_SOURCE_DIR}/compiler/generated/CovScriptVisitor.cpp
        )

foreach(src_file ${compiler-GENERATED_SRC})
    set_source_files_properties(
            ${src_file}
            PROPERTIES
            GENERATED TRUE
    )
endforeach(src_file ${compiler-GENERATED_SRC})

add_custom_target(GenerateParser DEPENDS ${compiler-GENERATED_SRC})
add_custom_command(OUTPUT ${compiler-GENERATED_SRC}
        COMMAND
        ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/compiler/generated/
        COMMAND
        "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror -Dlanguage=Cpp -listener -visitor -o ${PROJECT_SOURCE_DIR}/compiler/generated/ -package "cs_compiler_antlr_gen" ${PROJECT_SOURCE_DIR}/compiler/grammar/CovScript.g4
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        DEPENDS ${PROJECT_SOURCE_DIR}/compiler/grammar/CovScript.g4
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src
        ${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/misc
        ${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/atn
        ${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/dfa
        ${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/tree
        ${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/support
        ${PROJECT_SOURCE_DIR}/compiler/generated
        ${PROJECT_SOURCE_DIR}/compiler/include
        ${PROJECT_SOURCE_DIR}/runtime/include
        ${PROJECT_SOURCE_DIR}/shared/include
        )

aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src antlr4_SRC_1)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/misc antlr4_SRC_2)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/atn antlr4_SRC_3)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/dfa antlr4_SRC_4)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/tree antlr4_SRC_5)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/support antlr4_SRC_6)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/tree/pattern antlr4_SRC_7)
aux_source_directory(${PROJECT_SOURCE_DIR}/compiler/antlr/runtime/src/tree/xpath antlr4_SRC_8)
set(antlr4_SRC ${antlr4_SRC_1} ${antlr4_SRC_2} ${antlr4_SRC_3} ${antlr4_SRC_4} ${antlr4_SRC_5} ${antlr4_SRC_6} ${antlr4_SRC_7} ${antlr4_SRC_8})

add_library(antlr4_runtime STATIC
        ${antlr4_SRC})

#### CovScript SDK library target
set(compiler_SRC
        ${compiler-GENERATED_SRC}
        compiler/include/covscript/compiler/parser.h
        compiler/src/covscript/compiler/parser.cpp
        compiler/include/covscript/compiler/utils/unicode.h
        compiler/src/covscript/compiler/utils/unicode.cpp
        compiler/include/covscript/compiler/utils/bytebuffer.h
        compiler/src/covscript/compiler/utils/bytebuffer.cpp
        compiler/include/covscript/compiler/scope.h
        compiler/include/covscript/compiler/variable.h
        compiler/src/covscript/compiler/scope.cpp
        compiler/include/covscript/compiler/callState.h
        compiler/include/covscript/compiler/compiler.h
        compiler/include/covscript/compiler/sharedTypes.h
        compiler/src/covscript/compiler/compiler.cpp
        compiler/include/covscript/compiler/phase/assembleScan.h
        compiler/src/covscript/compiler/phase/assembleScan.cpp
        compiler/include/covscript/compiler/phase/assembleDefine.h
        compiler/src/covscript/compiler/phase/assembleDefine.cpp
        compiler/src/covscript/compiler/variable.cpp compiler/include/covscript/compiler/asm/CodeWriter.h)

set(runtime_SRC
        runtime/include/covscript/runtime/vm_instance.hpp
        )

set(shared_SRC
        shared/include/covscript/shared/any.hpp
        shared/include/covscript/shared/exception.hpp
        shared/include/covscript/shared/memory.hpp
        shared/include/covscript/shared/utility.hpp
        shared/include/covscript/shared/shared_types.hpp
        shared/src/definitions.cpp)

set(covscript_SRC
        ${compiler_SRC}
        ${runtime_SRC}
        ${shared_SRC}
        )

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set (flags_1 "-Wno-overloaded-virtual")
else()
    set (flags_1 "-MP /wd4251")
endif()

foreach(src_file ${covscript_SRC})
    set_source_files_properties(
            ${src_file}
            PROPERTIES
            COMPILE_FLAGS "${COMPILE_FLAGS} ${flags_1}"
    )
endforeach(src_file ${covscript_SRC})

add_library(covscript4 SHARED ${covscript_SRC})

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(covscript4 PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()

add_dependencies(covscript4 GenerateParser)
target_link_libraries(covscript4 antlr4_runtime)

#### CovScript Compiler interface target
add_executable(compiler
        compiler/src/main.cpp)

target_link_libraries(compiler covscript4)

#### Compiler Benchmarks and Tests
macro(add_compiler_benchmark_target name)
    add_executable(compiler-bench-${name}
            compiler/benchmarks/bench-${name}.cpp
            shared/benchmarks/BenchmarkHelper.h)
    target_link_libraries(compiler-bench-${name} covscript4)
    add_test(${name} compiler-bench-${name})
endmacro()

macro(add_compiler_test_target name)
    add_executable(compiler-test-${name} compiler/tests/test-${name}.cpp)
    target_link_libraries(compiler-test-${name} covscript4)
    add_test(${name} compiler-test-${name})
endmacro()

#### Runtime Benchmarks and Tests
macro(add_runtime_benchmark_target name)
    add_executable(runtime-bench-${name}
            runtime/benchmarks/bench-${name}.cpp
            shared/benchmarks/BenchmarkHelper.h)
    target_link_libraries(runtime-bench-${name} covscript4)
    add_test(${name} runtime-bench-${name})
endmacro()

macro(add_runtime_test_target name)
    add_executable(runtime-test-${name} runtime/tests/test-${name}.cpp)
    target_link_libraries(runtime-test-${name} covscript4)
    add_test(${name} runtime-test-${name})
endmacro()

#### Shared Benchmarks and Tests
macro(add_shared_benchmark_target name)
    add_executable(shared-bench-${name}
            shared/benchmarks/bench-${name}.cpp
            shared/benchmarks/BenchmarkHelper.h)
    target_link_libraries(shared-bench-${name} covscript4)
    add_test(${name} shared-bench-${name})
endmacro()

macro(add_shared_test_target name)
    add_executable(shared-test-${name} shared/tests/test-${name}.cpp)
    target_link_libraries(shared-test-${name} covscript4)
    add_test(${name} shared-test-${name})
endmacro()

add_compiler_benchmark_target(copy)
add_compiler_benchmark_target(bytebuffer)
add_compiler_test_target(typeflags)

add_shared_benchmark_target(any)
target_compile_features(shared-bench-any PRIVATE cxx_std_17)

add_shared_test_target(any)
