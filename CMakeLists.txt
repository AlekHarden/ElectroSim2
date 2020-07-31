cmake_minimum_required(VERSION 2.8.9)
project(ElectroSim2)

# Include libraries
include_directories(include)
file(GLOB LIB "lib/*")

# Set Sources
file(GLOB SOURCES "src/*.cpp")


# ifeq ($(shell uname), Linux)
# 	LIBS = -lpthread -lglfw -lGLEW -lGL
# else
# 	LIBS = -lglfw3dll -lglew32s -lopengl32

add_executable(ElectroSim2 ${SOURCES})

if(CMAKE_HOST_UNIX)
    target_link_libraries(ElectroSim2 pthread glfw GLEW libGLEW.so libGL.so)
else()
    target_link_libraries(ElectroSim2 ${LIB})
endif()
