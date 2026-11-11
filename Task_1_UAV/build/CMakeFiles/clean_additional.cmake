# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Control_Work_02_Polymorphism_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Control_Work_02_Polymorphism_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\model_tests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\model_tests_autogen.dir\\ParseCache.txt"
  "Control_Work_02_Polymorphism_autogen"
  "model_tests_autogen"
  )
endif()
