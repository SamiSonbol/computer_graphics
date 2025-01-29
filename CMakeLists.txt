cmake_minimum_required(VERSION 3.8)
project(computer_graphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#specify output directories for libraries and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")#for executables (.exe)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")#for static libraries (.lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")#for shared libraries (.dll or .so)

#setting the relative path of our resources folder (contains our various files such as .pngs) to be easily used in code
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
add_definitions(-DRESOURCES_DIR="${RESOURCES_DIR}")

#adding libraries
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#platform-specific GLFW handling
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_library(glfw3 STATIC IMPORTED)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/glfw3.lib"
    )
    target_include_directories(glfw3 INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW"
    )
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(glfw3 REQUIRED)
endif()

add_library(stb_image src/stb_image.cpp)
target_include_directories(stb_image PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

add_library(Math src/Math.cpp)
target_include_directories(Math PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

add_library(Mesh src/Mesh.cpp)
target_include_directories(Mesh PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

add_library(Shader src/Shader.cpp)
target_include_directories(Shader PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

install(TARGETS 
    glad
    stb_image
    Math 
    Mesh
    Shader
    ARCHIVE DESTINATION lib/${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    RUNTIME DESTINATION bin/${PROJECT_NAME}
)

#adding our main executable
add_executable(${PROJECT_NAME} src/Source.cpp)
target_link_libraries(${PROJECT_NAME} glad glfw3 stb_image Math Mesh Shader)
install(TARGETS
  ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)
