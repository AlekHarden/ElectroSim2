#include <ElectroSim/InputHandler.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

InputHandler::InputHandler(GLFWwindow* window, Handler* handler, float* s, float* px, float* py, float w, float h,glm::mat4* og) : mWindow(window){
	mOG = og;
	mHandler = handler;
	mScale = s;
	mPanX = px;
	mPanY = py;
	mWidth = w;
	mHeight = h;
	mPanning = 0;
	glfwSetCursorPosCallback(mWindow, mousePosition);
	glfwSetMouseButtonCallback(mWindow, mousePressed);
	glfwSetScrollCallback(mWindow, mouseScrolled);
	glfwSetKeyCallback(mWindow, keyPressed);
}

void InputHandler::getSelectedArea(float* area){

}

void InputHandler::mousePosition(GLFWwindow* window, double xPos, double yPos){

	MouseX = (xPos - mWidth/2) * (*mScale) + (*mPanX);
	MouseY = (-1* (yPos - mHeight/2)) * (*mScale) + (*mPanY);
	if(mPanning == true) {
		//uh pan

		*mOG = glm::translate(glm::mat4(1.0f), glm::vec3(MouseX - InitSX,MouseY - InitSY, 0)) * (*mOG);
		InitSX = MouseX;
		InitSY = MouseY;
	}
	if(mHandler->mHolding) {
		mHandler->setDelta(MouseX - InitX, MouseY - InitY);
	}


	//std::cout <<"Mouse: " << MouseX << " : " << MouseY << std::endl;

}

void InputHandler::mousePressed(GLFWwindow* window, int button, int action, int mods){
	if(action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			//std::cout <<"pressed" << std::endl;
			InitX = MouseX;
			InitY = MouseY;
			if(!mHandler->grabParticles(MouseX, MouseY)) {
				/*do selection box*/
			}
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			InitSX = MouseX;
			InitSY = MouseY;
			mPanning = 1;
			break;
		}
	}
	else if(action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			//std::cout << "released" << std::endl;
			mHandler->setDelta(0, 0);
			if(mHandler->mHolding) mHandler->releaseParticles();
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mPanning = 0;
			break;
		}
	}
}

void InputHandler::mouseScrolled(GLFWwindow * window, double xoffset, double yoffset){
	//*mOG = glm::translate(glm::mat4(1.0f), glm::vec3(-(*mPanX - MouseX), -(*mPanX - MouseX), 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(*mScale,  *mScale,0)) * glm::translate(glm::mat4(1.0f), glm::vec3((*mPanX - MouseX), (*mPanX - MouseX), 0)) * (*mOG);

	*mOG =  glm::translate(glm::mat4(1.0f), glm::vec3(-(*mPanX - MouseX), -(*mPanY - MouseY), 0)) * glm::scale(glm::mat4(1.0f), glm::vec3( (yoffset == -1 ? 0.9 : 1/0.9 ), (yoffset == -1 ? 0.9 : 1/0.9 ), 0)) * glm::translate(glm::mat4(1.0f), glm::vec3((*mPanX - MouseX), (*mPanY - MouseY), 0)) *(*mOG);
}

void InputHandler::keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE: glfwDestroyWindow(window); break;
		case GLFW_KEY_DELETE: mHandler->removeParticle(); break;
		case GLFW_KEY_MINUS: mHandler->addParticle(Particle(MouseX, MouseY, 20, -CHARGE)); break;
		case GLFW_KEY_EQUAL: mHandler->addParticle(Particle(MouseX, MouseY, 20, CHARGE)); break;
		}
	}
}
