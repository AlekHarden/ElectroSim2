cmake_minimum_required(VERSION 2.8.9)
project(ElectroSim2)

# Include libraries
include_directories(include)

# Set Sources
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ElectroSim2 ${SOURCES})

if(CMAKE_HOST_UNIX)
    target_link_libraries(ElectroSim2 pthread glfw GLEW libGLEW.so libGL.so)
    # add_custom_target(
    #     ElectroSimRun ALL
    #     COMMAND cd ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} && ./ElectroSim2
    #     VERBATIM
    # )
else()
    file(GLOB LIB "lib/windows/*")
    target_link_libraries(ElectroSim2 ${LIB} -lgdi32 -static-libgcc -static-libstdc++)
    add_custom_target(
        ElectroSimRun ALL
        COMMAND cd ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} && ElectroSim2.exe
        VERBATIM
    )
endif()
