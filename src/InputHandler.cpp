#include <ElectroSim/InputHandler.hpp>

InputHandler::InputHandler(GLFWwindow* window) : mWindow(window){
	glfwSetCursorPosCallback(mWindow, mousePosition);
	glfwSetKeyCallback(mWindow, pressed);
}

void InputHandler::getSelectedArea(float* area){

}

void InputHandler::mousePosition(GLFWwindow* window, double xPos, double yPos){
	MouseX = xPos;
	MouseY = yPos;
	//std::cout << "xPos = " << MouseX << " : yPos = " << MouseY << std::endl;
}

void InputHandler::pressed(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE: glfwDestroyWindow(window); break;
		}
	}
}
