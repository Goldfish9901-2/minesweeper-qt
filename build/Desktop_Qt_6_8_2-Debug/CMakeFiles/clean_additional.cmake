# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MineSweeper_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MineSweeper_autogen.dir/ParseCache.txt"
  "MineSweeper_autogen"
  )
endif()
