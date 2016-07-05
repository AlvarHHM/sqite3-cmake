cmake_minimum_required(VERSION 3.5)
project(sqlite3_cmake)

FILE(GLOB SRC src/*.*)

#_TCC += -DNDEBUG
#add_definitions(-DNDEBUG)
#add_definitions(-DSQLITE_DEBUG)
add_definitions(-o3)

add_executable(sqlite3_cmake ${SRC})

