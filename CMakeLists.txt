project(pvz-emulator)
cmake_minimum_required(VERSION 3.10)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(CMAKE_CXX_STANDARD 17)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/object OBJ_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system/zombie ZOMB_SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system/projectile PROJ_SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system/plant PLANT_SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/learning LEARNING_SRC)

add_library(pvzemu STATIC
    ${OBJ_SRC}
    ${SYS_SRC}
    ${ZOMB_SYS_SRC}
    ${PROJ_SYS_SRC}
    ${PLANT_SYS_SRC}
    ${LEARNING_SRC}
    world.cpp)

SET(LIB_INCLUDE
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/lib
   ${CMAKE_CURRENT_SOURCE_DIR}/system
   ${CMAKE_CURRENT_SOURCE_DIR}/object
   ${CMAKE_CURRENT_SOURCE_DIR}/learning)

target_include_directories(pvzemu PUBLIC ${LIB_INCLUDE})

if(PVZEMU_BUILD_PYBIND)
    add_definitions(-DPVZEMU_BUILD_PYBIND)

    add_subdirectory(lib/pybind11)
    pybind11_add_module(pvzemu-py SHARED
        ${OBJ_SRC}
        ${SYS_SRC}
        ${ZOMB_SYS_SRC}
        ${PROJ_SYS_SRC}
        ${PLANT_SYS_SRC}
        ${LEARNING_SRC}
        world.cpp
        pybind.cpp)

    target_include_directories(pvzemu-py PUBLIC ${LIB_INCLUDE})
endif(PVZEMU_BUILD_PYBIND)

if(PVZEMU_BUILD_DEBUGGER)
    add_definitions(-DPVZEMU_BUILD_DEBUGGER)

    ExternalProject_Add(debugger-ui
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/debugger
        CONFIGURE_COMMAND npm i
        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        BUILD_COMMAND npm run build
        INSTALL_COMMAND "")

    add_executable(debug-server ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
    add_dependencies(debug-server debugger-ui)
    target_link_libraries(debug-server pvzemu)
endif(PVZEMU_BUILD_DEBUGGER)
