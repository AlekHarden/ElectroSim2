#include <ElectroSim/VertexArray.hpp>
#include <ElectroSim/IndexBuffer.hpp>
#include <ElectroSim/Shader.hpp>


class Renderer{


public:
	void Draw(const VertexArray& va, const IndexBuffer& ib,const Shader& shader) const;
	void Clear();

};
