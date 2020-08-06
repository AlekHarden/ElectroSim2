#include <ElectroSim/InputHandler.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

InputHandler::InputHandler(GLFWwindow* window, Handler* handler, glm::mat4* proj, glm::mat4* view) : mWindow(window){
	mHandler = handler;
	mProj = proj;
	mView = view;
	mPanning = 0;
	dx = 0;
	dy = 0;
	glfwSetCursorPosCallback(mWindow, mousePosition);
	glfwSetMouseButtonCallback(mWindow, mousePressed);
	glfwSetScrollCallback(mWindow, mouseScrolled);
	glfwSetKeyCallback(mWindow, keyPressed);
}

void InputHandler::getSelectedArea(float* area){

}

void InputHandler::mousePosition(GLFWwindow* window, double xPos, double yPos){

	float width = 1 / ((*mProj)[0][0] / 2);
	float height = 1 / ((*mProj)[1][1] / 2);
	float panx = (*mView)[3][0];
	float pany = (*mView)[3][1];
	float scale = 1 / (*mView)[0][0];

	/************************NEVER CHANGE FIXED************************/
	MousePos.x = scale * ((xPos - width/2) - panx);
	MousePos.y = -1 * (scale * ((yPos - height/2) + pany));
	/******************************************************************/
	//std::cout << "Mouse: " << MousePos.x << " : " << MousePos.y << std::endl;




	if(mPanning == true) {

		dx = xPos - dx;
		dy = yPos - dy;

		*mView = glm::translate(glm::mat4(1.0f), glm::vec3(dx,-dy,0.0f)) * (*mView);
	}
	if(mHandler->mHolding) {
		mHandler->setDelta(MousePos.x - InitScenePos.x, MousePos.y - InitScenePos.y);
	}

	dx = xPos;
	dy = yPos;


}

void InputHandler::mousePressed(GLFWwindow* window, int button, int action, int mods){
	if(action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			InitScenePos = MousePos;
			if(!mHandler->grabParticles(MousePos.x, MousePos.y)) {
				/*do selection box*/
			}
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:

			mPanning = 1;
			break;
		}
	}
	else if(action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
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

	glm::vec3 offset (panx + MousePos.x/scale, pany + MousePos.y/scale, 0);

	*mView = glm::translate(glm::mat4(1.0f), offset) * glm::scale(glm::mat4(1.0f), glm::vec3( (yoffset == -1 ? 0.9 : 1/0.9 ), (yoffset == -1 ? 0.9 : 1/0.9 ), 0)) * glm::translate(glm::mat4(1.0f),-1.0f * offset) * (*mView);
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
