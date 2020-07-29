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

void test() {
	std::cout << "yo from thread" << std::endl;
}

std::string ReadShaderFile(std::string filepath) {
	char buffer[100];
	char cbullshit;
	std::string shaderSource;
	FILE *file = fopen(filepath.c_str(), "r");

	if (file == NULL) {
		std::cout << "works";
		return "hello bithch";
	}

	//fgetc eats first character so you have to set c bullshit and append it later
	while ((cbullshit = fgetc(file)) != EOF) {
		fgets(buffer, 100, file);
		shaderSource.push_back(cbullshit);
		shaderSource.append(buffer);
	}
	return shaderSource;
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;




	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
};


int main(void) {
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit()) return -1;

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

	std::cout << glGetString(GL_VERSION) << std::endl;
	/* Loop until the user closes the window */

	float positions[] = {
		-1.0, -0.5,
		0.0,  1.0,
		0.5,  -0.5,
		-1.0, -1.0
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// std::thread th1(test);
	// th1.join();

	std::string vertexShader = ReadShaderFile("../res/shaders/basic/vertex.shader");
	std::string fragmentShader = ReadShaderFile("../res/shaders/basic/fragment.shader");


	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	glUniform4f(location, 1.0, 0.5, 0.0, 1.0);

	unsigned int frames = 0;
	unsigned int framerate;
	double elapsedTime;
	double timeStart = ns() / 1000000000.0;

	float l;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform4f(location, l, 0.5, 0.0, 1.0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
