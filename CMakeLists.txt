cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
project (objectWrapper)
set(CMAKE_C_STANDARD 99)


set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.h)

set(PLATFORM "Windows")
set(ARCH "x64")

#check platform
if(WIN32)
  set(PLATFORM "Windows")
else()
  set(PLATFORM "Linux")
endif()

#check architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH "x64")
else()
  set(ARCH "x86")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${PLATFORM}/${ARCH})
# set the output directories based on the build type
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/build/${PLATFORM}/${ARCH}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/build/${PLATFORM}/${ARCH}/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MinSizeRel ${CMAKE_CURRENT_SOURCE_DIR}/build/${PLATFORM}/${ARCH}/minsizerel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RelWithDebInfo ${CMAKE_CURRENT_SOURCE_DIR}/build/${PLATFORM}/${ARCH}/RelWithDebInfo)

if(${PLATFORM} STREQUAL "Windows")
  if(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -s")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_MinSizeRel})
  endif()

  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
    #set preprocessor definition _DEBUG
    add_definitions(-D_DEBUG)
  endif()

  if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
  endif()

  if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RelWithDebInfo})
  endif()
endif()

# Add compiler flags for GCC
if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
endif()

set(SOURCE_FILES 
${SOURCE_DIR}/main.c
${SOURCE_DIR}/socketipc.c
${SOURCE_DIR}/packer.c
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )
target_link_libraries(${PROJECT_NAME} wsock32 ws2_32 comctl32)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})



if(MSVC)
  if(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup")
  endif()
  if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup")
  endif()
else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
      set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "-mconsole")
    endif()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
      set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "-mconsole")
    endif()
endif()

#add_custom_command(
#  TARGET ${PROJECT_NAME} POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/assets
#    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)