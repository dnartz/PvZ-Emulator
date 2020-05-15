project(pvz-emulator)
cmake_minimum_required(VERSION 3.16)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(CMAKE_CXX_STANDARD 17)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/object OBJ_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system/zombie ZOMB_SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system/projectile PROJ_SYS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/system/plant PLANT_SYS_SRC)

add_library(pvzemu STATIC
    ${OBJ_SRC}
    ${SYS_SRC}
    ${ZOMB_SYS_SRC}
    ${PROJ_SYS_SRC}
    ${PLANT_SYS_SRC}
    world.cpp)

target_include_directories(pvzemu 
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/system
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/object)

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
