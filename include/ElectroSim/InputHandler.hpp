#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <iostream>
#include <GLFW/glfw3.h>
#include <ElectroSim/Handler.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static glm::vec2 MousePos;
static glm::vec2 InitScenePos;
static glm::vec2 selectStartPos;
static float dx;
static float dy;
static Handler* mHandler;
static bool mPanning;
static bool mSelecting;
static glm::mat4* mView;
static glm::mat4* mProj;

class InputHandler {
private:
GLFWwindow* mWindow;

public:
InputHandler(GLFWwindow* window, Handler* handler, glm::mat4* proj, glm::mat4* view);
void getKeyPressed();
float* getSelectedArea();
bool isSelecting();
static void mousePosition(GLFWwindow* window, double xPos, double yPos);
static void mousePressed(GLFWwindow* window, int button, int action, int mods);
static void mouseScrolled(GLFWwindow * window, double xoffset, double yoffset);
static void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
