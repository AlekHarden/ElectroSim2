#define GLEW_STATIC
#include <ElectroSim/IndexBuffer.hpp>
#include <ElectroSim/Constants.hpp>
#include <GL/glew.h>
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : mCount(count) {
	// std::cout << "Indicies: "<< std::endl;
	//
	// for(int i = 0; i < count/3; i++) {
	// 	std::cout << data[i * 3] << ", " << data[i * 3 + 1] << ", " << data[i * 3 + 2] << std::endl;
	// }
	// std::cout << std::endl;


	glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer(){
	glDeleteBuffers(1,&mRendererID);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mRendererID);
}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
