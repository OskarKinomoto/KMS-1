CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT("kms1")

SET(CMAKE_CXX_FLAGS_PROFILING "-O0 -g -pg")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -g")
SET(CMAKE_CXX_FLAGS_CCOV "-O2 -g --coverage")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -msse -msse2 -msse3 -march=native -mfpmath=sse -mavx")

SET(CMAKE_BUILD_TYPE release)

# Set compiler warning flags
# ADD_DEFINITIONS("-Werror") # Make all warnings into errors.
ADD_DEFINITIONS("-Wall") # Generate all warnings
ADD_DEFINITIONS("-Wextra") # Generate even more extra warnings

SET(TARGET_MAIN "main")
SET(TARGET_GL "gl")

configure_file("parm.conf" "bin/parm.conf" COPYONLY)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )


ADD_SUBDIRECTORY(src)

