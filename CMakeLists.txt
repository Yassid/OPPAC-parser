cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
project(OParser)

find_package(HDF5 REQUIRED COMPONENTS CXX HL)
find_package(Boost COMPONENTS system filesystem regex REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")


include_directories(${CMAKE_SOURCE_DIR} ${HDF5_INCLUDE_DIRS} ${HDF5_HL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} )

message(STATUS "> ${HDF5_INCLUDE_DIRS}")
message(STATUS "> ${HDF5_CXX_LIBRARIES}")

add_library(OParserLib SHARED OParser.cxx)
target_link_libraries(
  OParserLib ${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES} ${HDF5_HL_LIBRARIES} ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY}
)

add_executable(OParserExe OParser.cxx)

target_link_libraries(
  OParserExe
  OParserLib
)
