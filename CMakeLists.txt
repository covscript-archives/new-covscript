cmake_minimum_required(VERSION 3.4)
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

#### Check C99
check_c_compiler_flag("-std=c99" COMPILER_SUPPORTS_C99)
if (COMPILER_SUPPORTS_C99)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
else ()
    message(STATUS "The compiler ${CMAKE_C_COMPILER} has no C99 support. Please use a different C compiler.")
endif ()
set(CMAKE_C_STANDARD 11)

find_package(Java COMPONENTS Runtime)

set(ANTLR_JAR_LOCATION ${PROJECT_SOURCE_DIR}/parser/antlr/antlr-4.7.2-complete.jar)

set(covscript-GENERATED_SRC
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptLexer.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptParser.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptBaseListener.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptBaseVisitor.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptListener.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptVisitor.cpp
        )

foreach(src_file ${covscript-GENERATED_SRC})
    set_source_files_properties(
            ${src_file}
            PROPERTIES
            GENERATED TRUE
    )
endforeach(src_file ${covscript-GENERATED_SRC})

add_custom_target(GenerateParser DEPENDS ${covscript-GENERATED_SRC})
add_custom_command(OUTPUT ${covscript-GENERATED_SRC}
        COMMAND
        ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/parser/generated/
        COMMAND
        "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror -Dlanguage=Cpp -listener -visitor -o ${PROJECT_SOURCE_DIR}/parser/generated/ -package "cs::compiler" ${PROJECT_SOURCE_DIR}/parser/grammar/CovScript.g4
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        DEPENDS ${PROJECT_SOURCE_DIR}/parser/grammar/CovScript.g4
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/parser/runtime/src
        ${PROJECT_SOURCE_DIR}/parser/runtime/src/misc
        ${PROJECT_SOURCE_DIR}/parser/runtime/src/atn
        ${PROJECT_SOURCE_DIR}/parser/runtime/src/dfa
        ${PROJECT_SOURCE_DIR}/parser/runtime/src/tree
        ${PROJECT_SOURCE_DIR}/parser/runtime/src/support
        ${PROJECT_SOURCE_DIR}/parser/generated
        ${PROJECT_SOURCE_DIR}/parser/include
        )

aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src antlr4_SRC_1)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/misc antlr4_SRC_2)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/atn antlr4_SRC_3)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/dfa antlr4_SRC_4)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/tree antlr4_SRC_5)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/support antlr4_SRC_6)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/tree/pattern antlr4_SRC_7)
aux_source_directory(${PROJECT_SOURCE_DIR}/parser/runtime/src/tree/xpath antlr4_SRC_8)
set(antlr4_SRC ${antlr4_SRC_1} ${antlr4_SRC_2} ${antlr4_SRC_3} ${antlr4_SRC_4} ${antlr4_SRC_5} ${antlr4_SRC_6} ${antlr4_SRC_7} ${antlr4_SRC_8})

add_library(antlr4_runtime STATIC
        ${antlr4_SRC})

set(parser_SRC
        parser/include/covscript/compiler/parser.h
        parser/src/covscript/compiler/parser.cpp
        parser/include/covscript/compiler/file.h
        parser/include/covscript/compiler/unicode.h
        parser/src/covscript/compiler/unicode.cpp)

set(covscript_SRC
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${parser_SRC}
        ${covscript-GENERATED_SRC}
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

add_executable(covscript4
        ${covscript_SRC}
        )

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(covscript4 PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()

add_dependencies(covscript4 GenerateParser)
target_link_libraries(covscript4 antlr4_runtime)


