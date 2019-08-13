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

set(ANTLR_JAR_LOCATION ${PROJECT_SOURCE_DIR}/parser/antlr/antlr-4.7.2-complete.jar)
set(covscript-GENERATED_SRC
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptLexer.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptLexer.h
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptParser.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptParser.h
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptBaseListener.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptBaseListener.h
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptBaseVisitor.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptBaseVisitor.h
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptListener.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptListener.h
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptVisitor.cpp
        ${PROJECT_SOURCE_DIR}/parser/generated/CovScriptVisitor.h
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
        "${Java_JAVA_EXECUTABLE}" -jar ${ANTLR_JAR_LOCATION} -Werror -Dlanguage=Cpp -listener -visitor -o ${PROJECT_SOURCE_DIR}/parser/generated/ -package "cs::parser" ${PROJECT_SOURCE_DIR}/parser/grammar/CovScript.g4
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
        )

aux_source_directory(parser/runtime/src antlr4_SRC)
add_library(antlr4_runtime STATIC
        ${antlr4_SRC})

set(covscript_SRC
        ${PROJECT_SOURCE_DIR}/main.cpp
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


