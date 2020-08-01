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
#include <Electrosim/VertexBuffer.hpp>
#include <Electrosim/IndexBuffer.hpp>
#include <Electrosim/Shader.hpp>
#include <ElectroSim/Handler.hpp>


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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

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

	std::cout << glGetString(GL_VERSION) << std::endl;
	/* Loop until the user closes the window */

	Proton p(0.5,0.5,0.5);
	Proton p2(-0.5, -0.5, 0.5);
	Handler h;
	h.addParticle(p);
	h.addParticle(p2);

	// for(int i = 0; i < CIRCLERESOLUTION - 2; i++) {
	// 	std::cout << p.mIndices[i * 3] << ", " << p.mIndices[i * 3 + 1] << ", " << p.mIndices[i * 3 + 2] << std::endl;
	// }
	// std::cout << std::endl;
	// for(int i = 0; i < RESOLUTION; i++){
    //     std::cout << p.mPoints[i * 2] << ", " << p.mPoints[i * 2 + 1] << std::endl;
    // }

	std::thread t1(test);
	t1.join();

	unsigned int vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

    VertexBuffer vb((const void*)h.getPoints(),2 * CIRCLERESOLUTION * 2 * sizeof(float));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	IndexBuffer ib(h.getIndices(),2* 3 * (CIRCLERESOLUTION - 2));

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
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();



	float l = 0.5;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window)) {



		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		shader.SetUniform4f("u_Color", l, 0.5, 0.0, 1.0);

		glBindVertexArray(vao);
		ib.Bind();

		glDrawElements(GL_TRIANGLES,2*  3 * (CIRCLERESOLUTION - 2), GL_UNSIGNED_INT, nullptr);

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
			std::cout << framerate << " " << elapsedTime << std::endl;
		}

		frames++;
	}
	glfwTerminate();
	return 0;
}
