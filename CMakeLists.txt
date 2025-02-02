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

#setting our project name as a defenition so it can be used in our source files
set(PROJECT_NAME "${PROJECT_NAME}")
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

#adding libraries
#glad library
add_library(glad src/glad/glad.c)
target_include_directories(glad PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#platform-specific GLFW library handling
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_library(glfw3 STATIC IMPORTED)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/GLFW/glfw3.lib"
    )
    target_include_directories(glfw3 INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW"
    )
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_library(glfw3 STATIC IMPORTED)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/GLFW/glfw3.a"
    )
    target_include_directories(glfw3 INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW"
    )
endif()

#stb_image library
add_library(stb_image src/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#imgui library
add_library(imgui
    src/imgui/imgui.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/imgui/imgui_impl_opengl3.cpp
    src/imgui/imgui_tables.cpp
)
target_include_directories(imgui PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
#target_link_libraries(imgui PUBLIC glfw3 ${OPENGL_LIBRARIES})

#Math library
add_library(Math src/computer_graphics/Math.cpp)
target_include_directories(Math PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#Mesh library
add_library(Mesh src/computer_graphics/Mesh.cpp)
target_include_directories(Mesh PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#Shader library
add_library(Shader src/computer_graphics/Shader.cpp)
target_include_directories(Shader PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

#UI library
add_library(UI src/computer_graphics/UI.cpp)
target_include_directories(UI PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)

install(TARGETS 
    glad
    stb_image
    imgui
    Math 
    Mesh
    Shader
    UI
    ARCHIVE DESTINATION lib/${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    RUNTIME DESTINATION bin/${PROJECT_NAME}
)

#adding our main executable
add_executable(${PROJECT_NAME} src/computer_graphics/Main.cpp)
target_link_libraries(${PROJECT_NAME} glad glfw3 stb_image imgui Math Mesh Shader UI)
install(TARGETS
  ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)
