#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <iostream>
#include <GLFW/glfw3.h>
#include <ElectroSim/Handler.hpp>

static double MouseX;
static double MouseY;
static double InitX;
static double InitY;
static Handler* mHandler;

class InputHandler {
private:
    GLFWwindow* mWindow;

public:
    InputHandler(GLFWwindow* window, Handler* h);
    void getKeyPressed();
    void getSelectedArea(float* area);
    static void mousePosition(GLFWwindow* window, double xPos, double yPos);
    static void mousePressed(GLFWwindow* window, int button, int action, int mods);
    static void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
