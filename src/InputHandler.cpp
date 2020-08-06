#include <ElectroSim/InputHandler.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

InputHandler::InputHandler(GLFWwindow* window, Handler* handler, glm::mat4* proj, glm::mat4* view) : mWindow(window){
	mHandler = handler;
	mProj = proj;
	mView = view;
	mPanning = 0;
	glfwSetCursorPosCallback(mWindow, mousePosition);
	glfwSetMouseButtonCallback(mWindow, mousePressed);
	glfwSetScrollCallback(mWindow, mouseScrolled);
	glfwSetKeyCallback(mWindow, keyPressed);
}

void InputHandler::getSelectedArea(float* area){

}

void InputHandler::mousePosition(GLFWwindow* window, double xPos, double yPos){








    // (*mView)[3][0] * (*mView)[0][0]
	// (*mView)[3][1] * (*mView)[1][1]

	// Top Left Corner of proj matrix
	// mProj[0][0] = 2 / (right - left)

	float width = 1 / ((*mProj)[0][0] / 2);
	float height = 1 / ((*mProj)[1][1] / 2);
	float panx = (*mView)[3][0];
	float pany = (*mView)[3][1];
	float scale = 1 / (*mView)[0][0];

	std::cout << "W H S-> " << width << " : " << height << " : " << scale << std::endl;
	std::cout << "px py -> " << panx << " : " << pany << std::endl;

							//(Width) 				//scale
	MousePos.x = scale * (xPos - width/2) + panx ;
	MousePos.y = -1 * (scale * (yPos - height/2) + pany);

	std::cout << "Mouse: " << MousePos.x << " : " << MousePos.y << std::endl;
	if(mPanning == true) {
		//uh pan
		*mView = glm::translate(glm::mat4(1.0f), glm::vec3(MousePos.x - InitScenePos.x, MousePos.y - InitScenePos.y, 0)) * (*mView);
		InitScenePos = MousePos;
	}
	if(mHandler->mHolding) {
		mHandler->setDelta(MousePos.x - InitX, MousePos.y - InitY);
	}


	//std::cout <<"Mouse: " << MousePos.x << " : " << MousePos.y << std::endl;

}

void InputHandler::mousePressed(GLFWwindow* window, int button, int action, int mods){
	if(action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			//std::cout <<"pressed" << std::endl;
			InitScenePos = MousePos;
			if(!mHandler->grabParticles(MousePos.x, MousePos.y)) {
				/*do selection box*/
			}
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			InitScenePos = MousePos;
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
	float scale = 1 / (*mView)[0][0];
	float panx = (*mView)[3][0];
	float pany = (*mView)[3][1];

	glm::vec3 offset (panx - MousePos.x, pany - MousePos.y , 0);

	*mView = glm::translate(glm::mat4(1.0f), -1.0f * offset) * glm::scale(glm::mat4(1.0f), glm::vec3( (yoffset == -1 ? 0.9 : 1/0.9 ), (yoffset == -1 ? 0.9 : 1/0.9 ), 0)) * glm::translate(glm::mat4(1.0f), offset) * (*mView);

	//*mView =  glm::translate(glm::mat4(1.0f), glm::vec3(-((*mView)[3][0] - MousePos.x), -((*mView)[3][1] - MousePos.y), 0)) * glm::scale(glm::mat4(1.0f), glm::vec3( (yoffset == -1 ? 0.9 : 1/0.9 ), (yoffset == -1 ? 0.9 : 1/0.9 ), 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(((*mView)[3][0] - MousePos.x), ((*mView)[3][1] - MousePos.y), 0)) *(*mView);
}

void InputHandler::keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS) {
		switch(key) {
		case GLFW_KEY_ESCAPE: glfwDestroyWindow(window); break;
		case GLFW_KEY_DELETE: mHandler->removeParticle(); break;
		case GLFW_KEY_MINUS: mHandler->addParticle(Particle(MousePos.x, MousePos.y, 20, -CHARGE)); break;
		case GLFW_KEY_EQUAL: mHandler->addParticle(Particle(MousePos.x, MousePos.y, 20, CHARGE)); break;
		}
	}
}
