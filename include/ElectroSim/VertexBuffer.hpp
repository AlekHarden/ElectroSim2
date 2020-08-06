#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP
class VertexBuffer {
private:
unsigned int mRendererID;
public:

VertexBuffer(void* data, unsigned int size );         //'size' in bytes
VertexBuffer(VertexBuffer& vb);
VertexBuffer();
~VertexBuffer();

void Bind() const;
void Unbind() const;

void SetPoints(void* data, unsigned int size);

};
#endif
