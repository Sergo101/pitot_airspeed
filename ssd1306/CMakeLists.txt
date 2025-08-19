cmake_minimum_required(VERSION 3.20)

set(LIB_NAME ssd1306)

file(GLOB sources
  ${CMAKE_CURRENT_LIST_DIR}/Src/*.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/*.cpp
)

file(GLOB headers
  ${CMAKE_CURRENT_LIST_DIR}/Inc/*.*
)

set(includes
  ${CMAKE_CURRENT_LIST_DIR}/Inc
)

add_library(${LIB_NAME} INTERFACE)
add_library(lib::ssd1306 ALIAS ${LIB_NAME})

target_sources(${LIB_NAME} INTERFACE ${sources} ${headers})
target_include_directories(${LIB_NAME} INTERFACE ${includes})

target_compile_definitions(${LIB_NAME} INTERFACE
)
