#include <ElectroSim/InputHandler.hpp>

InputHandler::InputHandler(GLFWwindow* window, Handler* handler, glm::mat4* proj, glm::mat4* view) : mWindow(window){
	mHandler = handler;
	mProj = proj;
	mView = view;
	mPanning = 0;
	mSelecting = 0;
	dx = 0;
	dy = 0;
	glfwSetCursorPosCallback(mWindow, mousePosition);
	glfwSetMouseButtonCallback(mWindow, mousePressed);
	glfwSetScrollCallback(mWindow, mouseScrolled);
	glfwSetKeyCallback(mWindow, keyPressed);
}

float* InputHandler::getSelectedArea(){
	float* points = (float *)malloc(sizeof(float) * 24);
	//each vertex
	for(int i = 0; i < 4; i++) {
		points[6 * i + 0] = (i < 2 ? selectStartPos.x : MousePos.x);
		points[6 * i + 1] = (i == 1 || i == 2 ? MousePos.y : selectStartPos.y);
		points[6 * i + 2] = 0.5f;
		points[6 * i + 3] = 0.5f;
		points[6 * i + 4] = 0.5f;
		points[6 * i + 5] = 0.3f;
	}

	return points;
}

bool InputHandler::isSelecting(){
	return mSelecting;
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

			if(mHandler->grabParticles(MousePos.x, MousePos.y)) break;
			else if(!mSelecting) {
				std::cout << "selecting" <<std::endl;
				mSelecting = true;
				selectStartPos = MousePos;
			}

			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			mPanning = true;
			break;

		}
	}
	else if(action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			if(mSelecting) {
				mHandler->selectArea(selectStartPos, MousePos);
				mHandler->setDelta(0, 0);
				mSelecting = false;
				break;
			}
			mHandler->setDelta(0, 0);
			if(mHandler->mHolding) mHandler->releaseParticles();
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mPanning = false;
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
