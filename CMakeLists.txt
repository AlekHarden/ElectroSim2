cmake_minimum_required(VERSION 3.29)
project(ElectroSim2)

# Include libraries
include_directories(include)

# Set Sources
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall")

add_executable(ElectroSim2 ${SOURCES})

if(CMAKE_HOST_UNIX)
    target_link_libraries(ElectroSim2 pthread glfw GLEW libGLEW.so libGL.so)
else()
    target_link_libraries(ElectroSim2 mingw32 glew32 glu32 opengl32 glfw3 gdi32 -static-libgcc -static-libstdc++)
endif()

# Install Shaders (Move shaders to bin dir to be used by executable)
set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/res/shaders/basic) # Use Basic Shader

#Copy Fragment Shader
set(IN_FILE ${SHADER_SOURCE_DIR}/fragment.fsh)
set(OUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/bin/fragment.fsh)
add_custom_command(OUTPUT ${OUT_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy
        ${IN_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/bin/
    DEPENDS ${IN_FILE}
    COMMAND_EXPAND_LISTS)
add_custom_target(fragment_shader
    DEPENDS ${OUT_FILE})
add_dependencies(ElectroSim2 fragment_shader)

#Copy Vertex Shader
set(IN_FILE ${SHADER_SOURCE_DIR}/vertex.vsh)
set(OUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/bin/vertex.vsh)
add_custom_command(OUTPUT ${OUT_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy
        ${IN_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/bin/
    DEPENDS ${IN_FILE}
    COMMAND_EXPAND_LISTS)
add_custom_target(vertex_shader
    DEPENDS ${OUT_FILE})
add_dependencies(ElectroSim2 vertex_shader)
