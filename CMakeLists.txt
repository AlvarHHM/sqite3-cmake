cmake_minimum_required(VERSION 3.5)
project(sqlite3_cmake)

FILE(GLOB SRC src/*.*)
set(TSRC ${SRC})
list(REMOVE_ITEM TSRC ${CMAKE_CURRENT_SOURCE_DIR}/src/shell.c)
message("${TSRC}")

#_TCC += -DNDEBUG
#add_definitions(-DNDEBUG)
#add_definitions(-DSQLITE_DEBUG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/workspace/sqlite3-cmake/bin")

message(WARNING ${CMAKE_CXX_FLAGS_DEBUG})
add_executable(sqlite3 ${SRC})
add_executable(tsqlite3 ${TSRC} main.c)

