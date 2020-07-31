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
#include <ElectroSim/Proton.hpp>

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

std::string ReadShaderFile(std::string filepath) {
	char buffer[100];
	char cbullshit;
	std::string shaderSource;
	FILE *file = fopen(filepath.c_str(), "r");

	// Error checking
	if (file == NULL) throw("ERROR: File \"" + filepath + "\" does not exist\n");

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

	Proton p(0, 0, 0.5);
	// for(int i = 0; i < CIRCLERESOLUTION - 2; i++) {
	// 	std::cout << p.mIndices[i * 3] << ", " << p.mIndices[i * 3 + 1] << ", " << p.mIndices[i * 3 + 2] << std::endl;
	// }
	// std::cout << std::endl;
	// for(int i = 0; i < RESOLUTION; i++){
    //     std::cout << p.mPoints[i * 2] << ", " << p.mPoints[i * 2 + 1] << std::endl;
    // }

	std::thread t1(test);
	t1.join();

	float positions[] = {
		-0.5, -0.5,
		0.5,  -0.5,
		0.5,  0.5,
		-0.5, 0.5
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER, CIRCLERESOLUTION * 2 * sizeof(float), p.mPoints, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (CIRCLERESOLUTION - 2) * sizeof(unsigned int), p.mIndices, GL_STATIC_DRAW);

	// std::thread th1(test);
	// th1.join();
	std::string vertexShader;
	std::string fragmentShader;

	// Error Handling for opening shader files
	try {
		vertexShader = ReadShaderFile("../res/shaders/basic/vertex.shader");
		fragmentShader = ReadShaderFile("../res/shaders/basic/fragment.shader");
	} catch (std::string e) {
		std::cout << e;
	}

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	glUniform4f(location, 1.0, 0.5, 0.0, 1.0);

	unsigned int frames = 0;
	unsigned int framerate;
	double elapsedTime;
	double timeStart = ns() / 1000000000.0;

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


	float l = 0.5;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glUniform4f(location, l, 0.5, 0.0, 1.0);

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glDrawElements(GL_TRIANGLES, 3 * (CIRCLERESOLUTION - 2), GL_UNSIGNED_INT, nullptr);

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
