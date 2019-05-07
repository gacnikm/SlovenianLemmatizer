cmake_minimum_required(VERSION 3.12)
project(LemmaGen VERSION 1.3.0
                 DESCRIPTION "LemmaGen lemmatizer"
                 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

# JAVA HEADLESS
if (UNIX)
 set(JAVA_AWT_LIBRARY NotNeeded)
 set(JAVA_AWT_INCLUDE_PATH NotNeeded)
endif(UNIX)

# ========================
# RDRLEMMATIZER LIBRARY
# ========================

add_library(rdrlemmatizer STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/RdrLemmatizer.cpp)

# ========================
# LEMMAGEN LIBRARY
# ========================

add_library(lemmagen SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/lemmagen.cpp)
target_link_libraries(lemmagen rdrlemmatizer)


# ========================
# SOLR LEMMATIZER JNI INTERFACE LIBRARY
# ========================

FIND_PACKAGE(JNI REQUIRED lemmatizer)

add_library(lemmatizer SHARED  ${CMAKE_CURRENT_SOURCE_DIR}/src/sl_lemmatizer_java.cpp)
target_include_directories(lemmatizer PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
target_link_libraries(lemmatizer rdrlemmatizer)

# ========================
# BINARY
# ========================

add_executable(lemmagen_bin ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(lemmagen_bin lemmagen)

# ========================
# TESTS
# ========================
include(FetchContent)
include(GoogleTest)

if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(WIN32)


# "warning C4616: #pragma warning : warning number '5046' not a valid compiler warning" error:
# https://github.com/google/googletest/commit/a2a15f2cefcfceaa6cee5b674a9b447359268ee9
#
# "MSVC < 2010 is not supported": https://github.com/google/googletest/issues/1848
#
# "The 1.8.x is the last release supporting pre-C++11 compilers.": https://github.com/google/googletest/releases/tag/release-1.8.1
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        a2a15f2cefcfceaa6cee5b674a9b447359268ee9
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

enable_testing()
file(GLOB lemmagen_test_files ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)

add_executable(lemmagen_tests ${lemmagen_test_files})
target_link_libraries(lemmagen_tests lemmagen gtest gtest_main)
gtest_add_tests(TARGET lemmagen_tests AUTO)


# ========================
# Set OS specific compiler flags/defs
# ========================

if (UNIX)
  set(COMPILER_FLAGS -O3 -ffunction-sections -fdata-sections -fstack-protector -fvisibility-inlines-hidden -static -std=c++11 -fPIC)
  set(COMPILER_FLAGS_WARNINGS -Wall -Wno-c++98-compat -Wno-old-style-cast -Wno-c++98-compat-pedantic -Wno-cast-align -Wno-padded)
  set(COMPILER_FLAGS ${COMPILER_FLAGS_WARNINGS} ${COMPILER_FLAGS})
  set(COMPILER_DEFS _FORTIFY_SOURCE=2)
endif(UNIX)

if (WIN32)
  set(COMPILER_FLAGS /EHsc)
endif(WIN32)

set_target_properties(gmock_main lemmagen_tests lemmagen rdrlemmatizer lemmatizer PROPERTIES COMPILE_OPTIONS "${COMPILER_FLAGS}")
set_target_properties(lemmagen_tests lemmagen rdrlemmatizer lemmatizer PROPERTIES COMPILE_DEFINITIONS "${COMPILER_DEFS}")


# ========================
# Set OS specific linker flags
# ========================

if (UNIX)
  set(LINKER_FLAGS "-Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro -Wl,--gc-sections")
endif(UNIX)

if (APPLE)
 set(LINKER_FLAGS "-Wl,-dead_strip")
endif(APPLE)

set_target_properties(lemmagen_tests lemmagen rdrlemmatizer lemmatizer PROPERTIES LINK_FLAGS "${LINKER_FLAGS}")
