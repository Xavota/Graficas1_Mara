#pragma once
#include "Descriptors.h"
#include "Buffer.h"
#include <vector>

#if defined(OGL)
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#endif

using std::vector;

namespace GraphicsModule
{
class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
	Mesh();
	~Mesh();

	static unsigned int getNewID();

	HRESULT setVertex(vector<Vertex> vertices);
	//Vertex* getVertex() { return m_vertices.data(); }
	//int getVertexCount() { return m_vertices.size(); }

	HRESULT setIndices(vector<unsigned int> indices);
	//unsigned int* getIndices() { return m_indices.data(); }
	int getIndexCount() { return m_indicesCount; }

	Color getColor() { return m_color; }
	void setColor(Color color) { m_color = color; }

	Buffer& getVertexBuffer() { return m_pVertexBuffer; }
	//void setVertexBuffer(Buffer vertexBuffer) { m_pVertexBuffer = vertexBuffer; }

	Buffer& getIndexBuffer() { return g_pIndexBuffer; }

	unsigned int getMeshID() { return m_ID; }

	void setMaterialID(unsigned int materialID) { m_materialID = materialID; }
	unsigned int getMaterialID() { return m_materialID; }

	void Draw(class RenderManager* renderManager);
	void SetResources(RenderManager* renderManager);

public:
	//vector<Vertex> m_vertices;
	Buffer m_pVertexBuffer;
	Buffer g_pIndexBuffer;
	int m_indicesCount;

	unsigned int m_ID = 0;

	Color m_color;

	unsigned int m_materialID = 0;

#if defined(OGL)
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
#endif
};
}
