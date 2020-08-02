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
#include <nano/nano.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <ElectroSim/VertexBuffer.hpp>
#include <ElectroSim/IndexBuffer.hpp>
#include <ElectroSim/Shader.hpp>
#include <ElectroSim/Handler.hpp>


float* pixelToScreen(float *points,int count){
	for(int i = 0; i < count ; i++){
		std::cout << "Points[" << i << "] = " << points[i*2] << ", " <<  points[i*2+1] << std::endl;


		points[i*2] /= (float)WIDTH/2;
		points[i*2 + 1] /= (float)HEIGHT/2;

		std::cout << "Points[" << i << "] (scaled) = " << points[i*2] << ", " <<  points[i*2+1] << std::endl;

	}
	return points;
}





void GLAPIENTRY MessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam ){

		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION){
			fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
		}


}

void test() {
	std::cout << "yo from thread" << std::endl;
}




int main(void) {
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);



	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
	}

	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );

	std::cout << glGetString(GL_VERSION) << std::endl << std::endl;
	/* Loop until the user closes the window */

	Particle p1(-50,0, 10,-0.00001);
 	Particle p2(50, 0, 10, 0.00001);

	Handler h;
	h.addParticle(p1);
	h.addParticle(p2);





	std::thread t1(test);
	t1.join();

	unsigned int vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);


	std::cout << "NumPoints: " << h.getNumPoints() << std::endl;
	std::cout << "NumInd: " << h.getNumInd() << std::endl;
	std::cout << "SizePoints: " << sizeof(float) * 2 * h.getNumPoints() << std::endl;
    VertexBuffer vb((void*)pixelToScreen(h.getPoints(),h.getNumPoints()),sizeof(float) * 2 * h.getNumPoints());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	IndexBuffer ib(h.getIndices(),h.getNumInd());

	//Create Shader
	std::string vertexShader;
	std::string fragmentShader;
	// Error Handling for opening shader files
	try {
		vertexShader = ReadShaderFile("../res/shaders/basic/vertex.shader");
		fragmentShader = ReadShaderFile("../res/shaders/basic/fragment.shader");
	} catch (std::string e) {
		std::cout << e;
	}
	Shader shader(vertexShader,fragmentShader);
	shader.Bind();
	shader.SetUniform4f("u_Color", 1.0, 0.5, 0.0, 1.0);

	unsigned int frames = 0;
	unsigned int framerate;
	double elapsedTime;
	double timeStart = ns() / 1000000000.0;

	glBindVertexArray(0);
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();



	float l = 0.5;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window)) {

		h.tick();

		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		shader.SetUniform4f("u_Color", l, 0.5, 0.0, 1.0);

		glBindVertexArray(vao);
		ib.Bind();
		vb.setPoints((void*)pixelToScreen(h.getPoints(),h.getNumPoints()),sizeof(float) * 2 * h.getNumPoints());


		glDrawElements(GL_TRIANGLES,h.getNumInd(), GL_UNSIGNED_INT, nullptr);

		if (l < 0 || l > 1) {
			increment *= -1;
		}

		l += increment;


		/* Swap front and back buffers */
		glfwSwapBuffers(window);


		/* Poll for and process events */
		glfwPollEvents();


		elapsedTime = ns() / 1000000000.0 - timeStart;

		if (elapsedTime >= 1) {
			framerate = frames;
			timeStart = ns() / 1000000000.0;
			frames = 0;
			//std::cout << framerate << " " << elapsedTime << std::endl;
		}

		frames++;
	}
	glfwTerminate();
	return 0;
}
