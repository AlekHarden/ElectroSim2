#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#define GLEW_STATIC

//Dependencies
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Standard Libraries
#include <string>

//Local Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ElectroSim/Constants.hpp>






class Display {

private:
GLFWwindow* mWindow;
const GLFWvidmode* mDiplayMode;

std::string mTitle;
glm::vec2 mDimensions;

unsigned int mRefreshRate;

glm::mat4 proj;
glm::mat4 view;


public:
Display(std::string title);
~Display();

glm::vec2 getDimensions();
unsigned int getRefreshrate();
GLFWwindow* getWindow();

glm::mat4* getProj();
glm::mat4* getView();



};


#endif
