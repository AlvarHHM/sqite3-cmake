#cmake_minimum_required(VERSION 3.5)
project(sqlite3_cmake)

FILE(GLOB SRC src/sqlite3_src/*.*)
set(TSRC ${SRC})
list(REMOVE_ITEM TSRC ${CMAKE_CURRENT_SOURCE_DIR}/src/sqlite3_src/shell.c)

#_TCC += -DNDEBUG
#add_definitions(-DNDEBUG)
#add_definitions(-DSQLITE_DEBUG)
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -ldl")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(sqlite3 ${SRC})
add_executable(tsqlite3 ${TSRC} src/main.c)
add_executable(trend ${TSRC} src/trend.c)
