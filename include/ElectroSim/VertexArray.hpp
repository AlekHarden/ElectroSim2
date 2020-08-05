#define GLEW_STATIC
#include <GL/glew.h>
#include <ElectroSim/VertexBufferLayout.hpp>
#include <ElectroSim/VertexBuffer.hpp>


class VertexArray {
private:
unsigned int mRendererID;
public:

VertexArray();
~VertexArray();

void Bind() const;
void Unbind() const;

void AddBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);
};
