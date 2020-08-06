#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <iostream>
#include <GLFW/glfw3.h>
#include <ElectroSim/Handler.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static double MouseX;
static double MouseY;
static double InitX;
static double InitY;
static double InitSX;
static double InitSY;
static Handler* mHandler;
static float* mScale;
static float* mPanX;
static float* mPanY;
static float mWidth;
static float mHeight;
static bool mPanning;
static glm::mat4* mOG;

class InputHandler {
private:
GLFWwindow* mWindow;

public:
InputHandler(GLFWwindow* window, Handler* handler, float* s, float* px, float* py, float w, float h,glm::mat4* og);
void getKeyPressed();
void getSelectedArea(float* area);
static void mousePosition(GLFWwindow* window, double xPos, double yPos);
static void mousePressed(GLFWwindow* window, int button, int action, int mods);
static void mouseScrolled(GLFWwindow * window, double xoffset, double yoffset);
static void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
