#define GLEW_STATIC



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ElectroSim/Shader.hpp>
#include <ElectroSim/Renderer.hpp>
#include <ElectroSim/Handler.hpp>
#include <ElectroSim/InputHandler.hpp>
#include <glm/glm.hpp>
#include <Utils.hpp>
#include <Display.hpp>










int main(void) {
	utils::seed();

	Display display("ElectroSim2");
	Handler handler;
	InputHandler inHandler(display.getWindow(),&handler,display.getProj(),display.getView());

	GLFWwindow* window = display.getWindow();
	glm::mat4* proj = display.getProj();
	glm::mat4* view = display.getView();
	glm::mat4 mvp;




	//Spawn Particles in random Locations w/ Random Charges
	for(int i = 0; i <100; i++ ) {
		handler.addParticle(Particle(utils::randNum() * display.getDimensions().x * DEFAULT_SCALE,utils::randNum() * display.getDimensions().y * DEFAULT_SCALE, 20,( rand()%2 == 0 ? 1 : -1 ) * CHARGE));
	}










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





	float* points;
	unsigned int* indices;
	while (!glfwWindowShouldClose(window)) {
		frameStart = ns() / 1000000000.0;


		mvp = (*proj) * (*view);
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
		}

		frames++;

		frameEnd = ns() / 1000000000.0;
		deltaTimeS = (frameEnd - frameStart);
	}
	glfwTerminate();
	return 0;
}
