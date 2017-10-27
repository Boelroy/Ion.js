cmake_minimum_required(VERSION 3.2)
project (Ion)


execute_process(COMMAND "python ${CMAKE_CURRENT_SOURCE_DIR}/js2c.py")

# set(ICU_INCLUDE_PATH ......) # -> if you have a custom ICU setup
# set(CHAKRACORE_PATH "................/Github/ChakraCore/") # -> location of the ChakraCore Repo
# set(CHAKRACORE_LIB_TYPE ....Release or Test or Debug) # -> by default Release

message ("-- Compiling ChakraCore..")
# Embed ChakraCore
set(CHAKRACORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/ChakraCore")
include(ChakraCoreEmbed.cmake)

message ("-- Compiling libuv...")
#Embed libuv
include(libuv.cmake)
include_directories(deps/libuv/include)

message ("-- Compiling ion...")
# main
set(ION_INCLUDE_PATH src)

add_executable(ion ${ION_INCLUDE_PATH}/ion.cc
  ${ION_INCLUDE_PATH}/env.h
  ${ION_INCLUDE_PATH}/ion.h
  ${ION_INCLUDE_PATH}/runtime.h
  ${ION_INCLUDE_PATH}/utils.h
  ${ION_INCLUDE_PATH}/vm.h
  ${ION_INCLUDE_PATH}/natives.h
  ${ION_INCLUDE_PATH}/timer.h
  ${ION_INCLUDE_PATH}/process.h
  ${ION_INCLUDE_PATH}/constants.h
  ${ION_INCLUDE_PATH}/fs.h
  ${ION_INCLUDE_PATH}/bootstrap_ion.js)

target_include_directories (ion
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  ${ION_INCLUDE_PATH}
  ${CHAKRACORE_INCLUDE_PATH} # add ChakraCore include path
  )
target_link_libraries (ion
  uv # add ChakraCore linker options
  )
target_link_libraries (ion
  ${CHAKRACORE_LINKER_OPTIONS} # add ChakraCore linker options
  )