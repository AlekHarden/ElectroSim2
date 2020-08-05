#include <ElectroSim/InputHandler.hpp>

InputHandler::InputHandler(GLFWwindow* window, Handler* handler) : mWindow(window){
    mHandler = handler;
    glfwSetCursorPosCallback(mWindow, mousePosition);
    glfwSetMouseButtonCallback(mWindow, mousePressed);
    glfwSetKeyCallback(mWindow, keyPressed);
}

void InputHandler::getSelectedArea(float* area){

}

void InputHandler::mousePosition(GLFWwindow* window, double xPos, double yPos){
    if(mHandler->mHolding){
        mHandler->setDelta(MouseX - InitX, MouseY - InitY);
    }
    MouseX = xPos;
    MouseY = yPos;
}

void InputHandler::mousePressed(GLFWwindow* window, int button, int action, int mods){
    if(action == GLFW_PRESS){
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                std::cout <<"pressed\n";
                InitX = MouseX;
                InitY = MouseY;
                if(!mHandler->grabParticles(MouseX, MouseY))break;
            case GLFW_MOUSE_BUTTON_MIDDLE: break;
        }
    }
    else if(action == GLFW_RELEASE){
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: std::cout << "released\n"; if(mHandler->mHolding) mHandler->releaseParticles(); break;
        }
    }
}

void InputHandler::keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_ESCAPE: glfwDestroyWindow(window); break;
        }
    }
}
