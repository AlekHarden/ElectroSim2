#include <Display.hpp>
#include <ElectroSim/OpenGLError.hpp>




Display::Display(std::string title) : mTitle(title){

	/* Initialize the library */
	if (!glfwInit()) throw "Error: GLFW could not initialize.";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	// ------- Enable Verbose OpenGL MessageCallback Logging -------
	if(DEBUG) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	mDiplayMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	mRefreshRate = (unsigned int)mDiplayMode->refreshRate;

	if(FULLSCREEN) {
		/* Create a fullscreen mode window and its OpenGL context */

		glfwWindowHint(GLFW_REFRESH_RATE, mRefreshRate);

		mDimensions.x = (unsigned int)mDiplayMode->width;
		mDimensions.y = (unsigned int)mDiplayMode->height;
		mWindow = glfwCreateWindow(mDimensions.x, mDimensions.y, mTitle.c_str(), glfwGetPrimaryMonitor(), NULL);


	}
	else{
		/* Create a windowed mode window and its OpenGL context */
		glfwWindowHint(GLFW_RESIZABLE,false);

		mDimensions.x = DEFAULT_WIDTH;
		mDimensions.y = DEFAULT_HEIGHT;
		mWindow = glfwCreateWindow(mDimensions.x, mDimensions.y, mTitle.c_str(), NULL, NULL);

	}

	if (!mWindow) {
		glfwTerminate();
		throw "Error: GLFW window could not be created.";
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(0);



	if (glewInit() != GLEW_OK) throw "Error: GLEW could not Initialize";

	// Print OpenGL Version
	std::cout << std::endl << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	// Let user know if debug mode is enabled.
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "** DEBUG MODE **" << std::endl;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	//MessageCallback is included in OpenGLError
	glDebugMessageCallback(MessageCallback, 0);



	// Spent 3 hours on this one line of code
	glClearColor(0.1,0.1,0.1,1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	proj = glm::ortho(-mDimensions.x/2.0f, mDimensions.x/2.0f,-mDimensions.y/2.0f, mDimensions.y/2.0f, -1.0f, 1.0f);
	view = glm::scale(glm::mat4(1.0f),glm::vec3(DEFAULT_SCALE,DEFAULT_SCALE,1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-DEFAULT_PANX,-DEFAULT_PANY,0));


}


Display::~Display(){
}

GLFWwindow* Display::getWindow(){
	return mWindow;

}

glm::vec2 Display::getDimensions(){
	return mDimensions;
}


unsigned int Display::getRefreshrate(){
	return mRefreshRate;
}

glm::mat4* Display::getProj(){
	return &proj;
}
glm::mat4* Display::getView(){
	return &view;
}
