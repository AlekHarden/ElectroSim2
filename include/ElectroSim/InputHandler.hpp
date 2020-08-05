#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <iostream>
#include <GLFW/glfw3.h>

static double MouseX;
static double MouseY;

class InputHandler {
private:
GLFWwindow* mWindow;
public:
InputHandler(GLFWwindow * window);
void getKeyPressed();
void getSelectedArea(float* area);
static void mousePosition(GLFWwindow* window, double xPos, double yPos);
static void pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
