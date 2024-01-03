# CMakeList.txt : CMake project for aoc23, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.28)
project ("aoc23" CXX)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 23)


# Add source to this project's executable.
add_executable (aoc23 "aoc23.cpp")
target_sources(aoc23 PRIVATE FILE_SET CXX_MODULES FILES "Utils/Parser.ixx" "Solutions/Day1.ixx" "Utils/AoC.ixx" "Utils/Input.ixx" "Utils/Strings.ixx" "Solutions/Day2.ixx" "Solutions/Day3.ixx" "Solutions/Day4.ixx" "Solutions/Day5.ixx" "Solutions/Day6.ixx" "Solutions/Day7.ixx" "Solutions/Day8.ixx")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET aoc23 PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
