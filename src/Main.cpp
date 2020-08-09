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

void update(double deltaTimeS){

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
		glfwWindowHint(GLFW_RESIZABLE,false);
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



	Handler handler;

	//Spawn Particles in random Locations w/ Random Charges
	for(int i = 0; i <100; i++ ) {
		//( rand()%2 == 0 ? 1 : -1 )
		handler.addParticle(Particle(randNum() * Width * SCALE,randNum() * Height * SCALE, 20,( rand()%2 == 0 ? 1 : -1 ) * 0.01));
	}

	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 mvp;

	//left                //right                  //bottom            //top      	//front //near
	proj = glm::ortho(-Width/2.0f, Width/2.0f,-Height/2.0f, Height/2.0f, -1.0f, 1.0f);
	view = glm::scale(glm::mat4(1.0f),glm::vec3(SCALE,SCALE,1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-PANX,-PANY,0));



	InputHandler inHandler(window, &handler, &proj, &view);




	//------Circle Batch-------
	VertexBuffer vb(nullptr,sizeof(float) * 6 * handler.getNumPoints());
	IndexBuffer ib(nullptr,handler.getNumInd());
	VertexArray va;
	VertexBufferLayout layout;
	//Position Vec 2
	layout.Push(GL_FLOAT,2);
	//Color Vec 4
	layout.Push(GL_FLOAT,4);
	va.AddBuffer(vb,layout);
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	//--------------------------


	//------Select Box-------
	unsigned int square[6] = {0,1,2,0,2,3};
	VertexBuffer Svb(nullptr,sizeof(float) * 6 * 4);
	IndexBuffer Sib(nullptr,6);
	VertexArray Sva;
	VertexBufferLayout Slayout;
	//Position Vec 2
	Slayout.Push(GL_FLOAT,2);
	//Color Vec 4
	Slayout.Push(GL_FLOAT,4);
	Sva.AddBuffer(Svb,Slayout);
	Sva.Unbind();
	Svb.Unbind();
	Sib.Unbind();
	//--------------------------


	Renderer renderer;
	Shader shader("../res/shaders/basic/vertex.vsh","../res/shaders/basic/fragment.fsh");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP",mvp);

	unsigned int frames = 0;
	unsigned int framerate;
	double elapsedTime;
	double timeStart = ns() / 1000000000.0;
	double frameStart;
	double frameEnd;
	double deltaTimeS;



	// Spent 3 hours on this one line of code
	glClearColor(0.1,0.1,0.1,1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	float* points;
	unsigned int* indices;
	while (!glfwWindowShouldClose(window)) {
		frameStart = ns() / 1000000000.0;

		if(deltaTimeS > 0.001  )

			mvp = proj * view;
		shader.Bind();
		shader.SetUniformMat4f("u_MVP",mvp);


		// Clear with Color We set earlier
		renderer.Clear();

		//Draw Particles if they exist
		if(handler.getNumPoints() != 0) {
			handler.tick(deltaTimeS);
			points = handler.getPoints();
			indices = handler.getIndices();
			ib.SetIndices(indices,handler.getNumInd());
			vb.SetPoints((void*)points,sizeof(float) * 6 * handler.getNumPoints());
			renderer.Draw(va,ib,shader);
			free(points);
			free(indices);
		}

		//Draw Select Box
		if(inHandler.isSelecting()) {
			points = inHandler.getSelectedArea();
			Sib.SetIndices(square,6);
			Svb.SetPoints((void*)points,sizeof(float) * 6 * 4);
			renderer.Draw(Sva,Sib,shader);

			free(points);
		}


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();




		elapsedTime = ns() / 1000000000.0 - timeStart;

		if (elapsedTime >= 1) {
			//handler.addVelall();
			framerate = frames;
			timeStart = ns() / 1000000000.0;
			frames = 0;
			std::cout << "FPS: " << framerate << std::endl;
			std::cout << "dt =  " << deltaTimeS << std::endl;

		}

		frames++;

		frameEnd = ns() / 1000000000.0;
		deltaTimeS = (frameEnd - frameStart);
	}
	glfwTerminate();
	return 0;
}
