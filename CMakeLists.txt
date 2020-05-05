# SETUP
cmake_minimum_required(VERSION 3.12)
project(Chess
        DESCRIPTION "Chess Engine & Chess AI using standard protocols"
        VERSION 0.1
        HOMEPAGE_URL "https://gitlab.cri.epita.fr/theo.lepage/chess"
        LANGUAGES CXX)

#[[
----- USAGE -----
1. Create and go to a directory of build (preference : debug or release) :
    - `mkdir build_debug` / `mkdir build_release`
    - `cd build_debug` / `cd build_release`
    (Use `build_debug` folder to use a Debug BUILD_TYPE)

2. Generate cmake files in build directory with the good options :
    - `cmake .. [OPTIONS]`
        - Set Compiler  : -DCMAKE_C_COMPILER=/usr/bin/g++
        - Debug build   : -DCMAKE_BUILD_TYPE=Debug
        - Release build : -DCMAKE_BUILD_TYPE=Release

3. Use make as usual in the wanted build_directory:
    - `make`
    - `make all`
    - `make clean`
    - `make check`
    - `make -B`
    - verbose option : `VERBOSE=1 make`

4. verbose tests
    - ctest --verbose
]]

# OPTIMISATION FLAGS
set(OPT_FLAGS
    #FIXME
    )

# SOURCES
set(MAIN_ENGINE
    src/main.cc)
set(SRC_ENGINE
    #src/...
    #FIXME
    )
include_directories(src
                    #FIXME
                    )

# TESTS
set(SRC_TESTS
    tests/unit_tests/example_test.cc
    tests/unit_tests/example2_test.cc
    #FIXME
    )

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -fsanitize=address")

# BUILD_TYPE (if not defined by the user)
if (NOT CMAKE_BUILD_TYPE)
    string(REGEX REPLACE ".*/build_" "" BUILD_NAME ${CMAKE_BINARY_DIR})
    # Default BUILD_TYPE is Release
    set(CMAKE_BUILD_TYPE Release)
    # If folder is named "build_debug" BUILD_TYPE is set to: Debug
    if(${BUILD_NAME} STREQUAL "Debug" OR ${BUILD_NAME} STREQUAL "debug")
        set(CMAKE_BUILD_TYPE Debug)
    endif()
    message(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
endif()

# LIBRARIES
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})

# CHESSENGINE
add_executable(chessengine ${MAIN_ENGINE})
set_target_properties(chessengine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} # binary destination
)
target_sources(chessengine PRIVATE ${SRC_ENGINE})
if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    target_link_libraries(chessengine PRIVATE ${LIBRARIES} -static)
else()
    target_link_libraries(chessengine PRIVATE ${LIBRARIES})
endif()

# TESTS
find_package(GTest)
find_package (Threads)
if (${GTEST_FOUND} AND ${THREADS_FOUND})
    enable_testing()
    # For each test file create an executable of test. Launched using ctest
    foreach(f IN LISTS SRC_TESTS)
        message(STATUS "TESTS : building file : ${f}")
        # Find the test_name by deleting the extension and the parent_dir
        string(REGEX REPLACE "\\.[^.]*$" "" F_WITHOUT_EXT ${f})
        string(REGEX REPLACE "[^/]*/" "" TEST_NAME ${F_WITHOUT_EXT})

        # Create an executable with all sources and lib needed
        add_executable(${TEST_NAME} ${f})
        target_sources(${TEST_NAME} PRIVATE ${SRC_ENGINE})
        target_link_libraries(${TEST_NAME} PRIVATE
            ${GTEST_BOTH_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
        )

        # Add the executable created to the test list of ctest
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        # Set timeout of the test to 3 seconds
        set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 3)
    endforeach()
else()
    message(WARNING "Gtest or ThreadLib not found... You will not be able to launch tests")
endif()