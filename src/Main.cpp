#define GLEW_STATIC
#if defined(_WIN32)
	#define _WIN32_WINNT 0x0A00
	#include <mingw/mingw.thread.h>
#else // if defined(_WIN32)
	#include <thread>
#endif // if defined(_WIN32)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <nano/nano.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <ElectroSim/Shader.hpp>
#include <ElectroSim/Renderer.hpp>
#include <ElectroSim/Handler.hpp>
#include <ElectroSim/InputHandler.hpp>
#include <ElectroSim/OpenGLError.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


static int Width;
static int Height;


// return random number from -0.5 to 0.5
float randNum(){
	return (((float)rand() - (float)RAND_MAX/2) / (float)RAND_MAX);
}



void test() {
	std::cout << "Thread Test" << std::endl;
}




int main(void) {
	srand(ns());
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	// ------- ENABLE DEBUG MODE -------
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


	if(FULLSCREEN) {
		/* Create a fullscreen mode window and its OpenGL context */
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		//BitDepth
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		//RefreshRate
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window = glfwCreateWindow(mode->width, mode->height, "ElectroSim2",glfwGetPrimaryMonitor(), NULL);
		Width = (int)mode->width;
		Height = (int)mode->height;

	}
	else{
		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(WIDTH,HEIGHT, "ElectroSim2",NULL, NULL);
		Width = WIDTH;
		Height = HEIGHT;
	}

	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);



	if (glewInit() != GLEW_OK) {
		std::cout << "glewInit Error" << std::endl;
	}

	// Let user know if its in debug mode.
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "** DEBUG MODE **" << std::endl;
	}

	// Enable DebugMessageCallback
	glEnable( GL_DEBUG_OUTPUT );
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback( MessageCallback, 0 );

	// Print OpenGL Version
	std::cout << "\nOpenGL "<< glGetString(GL_VERSION) << std::endl;



	Handler h;

	// Spawn Particles in random Locations w/ Random Charges
	for(int i = 0; i <200; i++ ) {
		h.addParticle(*new Particle(randNum() * Width,randNum() * Height,20,( rand()%2 == 0 ? 1 : -1 ) * 0.0001));
	}

	float scale = 0.5;
	float panx = 0;
	float pany = 0;
	//glm::mat4 proj = glm::ortho(-Width/2,Width/2,-Height/2,Height/2,1,1);
	glm::mat4 proj = glm::ortho(scale * -Width/2.0f, scale * Width/2.0f, scale * -Height/2.0f, scale * Height/2.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-panx,-pany,0));
	glm::mat4 mvp = proj * view;

	unsigned int* indices = h.getIndices();
	IndexBuffer ib(indices,h.getNumInd());

	VertexArray va;
	VertexBuffer vb(nullptr,sizeof(float) * 6 * h.getNumPoints());
	VertexBufferLayout layout;

	//Position Vec 2
	layout.Push(GL_FLOAT,2);
	//Color Vec 4
	layout.Push(GL_FLOAT,4);
	va.AddBuffer(vb,layout);

	Shader shader("../res/shaders/basic/vertex.shader","../res/shaders/basic/fragment.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP",mvp);
	Renderer renderer;

	unsigned int frames = 0;
	unsigned int framerate;
	double elapsedTime;
	double timeStart = ns() / 1000000000.0;

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	// Spent 3 hours on this one line of code
	glClearColor(0.1,0.1,0.1,1);

	// Make InputHandler
	InputHandler inHandler(window);

	while (!glfwWindowShouldClose(window)) {

		h.tick();
		float* points = h.getPoints();
		vb.setPoints((void*)points,sizeof(float) * 6 * h.getNumPoints());


		// Clear with Color We set earlier
		renderer.Clear();
		renderer.Draw(va,ib,shader);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();




		elapsedTime = ns() / 1000000000.0 - timeStart;

		if (elapsedTime >= 1) {
			//h.addVelall();
			framerate = frames;
			timeStart = ns() / 1000000000.0;
			frames = 0;
			std::cout << "FPS: " << framerate << std::endl;
		}

		frames++;

		free(points);
	}
	free(indices);
	glfwTerminate();
	return 0;
}
