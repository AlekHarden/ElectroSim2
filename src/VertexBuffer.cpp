#define GLEW_STATIC
#include <ElectroSim/VertexBuffer.hpp>
#include <ElectroSim/Constants.hpp>
#include <GL/glew.h>
#include <stdlib.h>
#include <iostream>

VertexBuffer::VertexBuffer(void* data, unsigned int size){

	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ARRAY_BUFFER,mRendererID);
	glBufferData(GL_ARRAY_BUFFER, size,(const void*) data, GL_STATIC_DRAW);

	// std::cout << "Points: " << std::endl;
	// float* trash = static_cast <float*> (data);
	// for(int i = 0; i < size / (2 * sizeof(float)); i++){
	// 	std::cout <<"[" << i << "] "<< trash[i * 2] << ", " << trash[i * 2 + 1] << std::endl;
	// }
	// std::cout << std::endl;
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1,&mRendererID);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER,mRendererID);
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void VertexBuffer::setPoints(void* data, unsigned int size){
	glBindBuffer(GL_ARRAY_BUFFER,mRendererID);
	glBufferData(GL_ARRAY_BUFFER, size,(const void*) data, GL_STATIC_DRAW);
}
