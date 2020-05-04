# OPTIMISATION FLAGS : FIXME

set(OPT_FLAGS)

# END OPTIMIZATION FLAGS
# SOURCES : FIXME

set(SRC_ENGINE)
include_directories()

# END SOURCES


# SETUP
cmake_minimum_required(VERSION 3.12)
project(Chess)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -0g -g3 -fsanitize=address")

# LIBRARIES
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})

# CHESSENGINE
add_executable(chessengine)
target_sources(chessengine PRIVATE ${SRC_ENGINE})
target_link_libraries(chessengine PRIVATE ${LIBRARIES} -static)