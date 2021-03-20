#pragma once
#include "ShaderResourceView.h"
#include "Descriptors.h"
#include "Buffer.h"
#include <vector>

using std::vector;

namespace GraphicsModule
{
class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int numOfMesh);
	Mesh(unsigned int numOfMesh);
	~Mesh();

	HRESULT setVertex(vector<Vertex> vertices);
	Vertex* getVertex() { return m_vertices.data(); }
	int getVertexCount() { return m_vertices.size(); }

	HRESULT setIndices(vector<unsigned int> indices);
	unsigned int* getIndices() { return m_indices.data(); }
	int getIndexCount() { return m_indices.size(); }

	Color getColor() { return m_color; }
	void setColor(Color color) { m_color = color; }

	Buffer& getVertexBuffer() { return m_pVertexBuffer; }
	//void setVertexBuffer(Buffer vertexBuffer) { m_pVertexBuffer = vertexBuffer; }

	Buffer& getIndexBuffer() { return g_pIndexBuffer; }

	unsigned int getNumOfMesh() { return m_numOfMesh; }

private:
	vector<Vertex> m_vertices;
	Buffer m_pVertexBuffer;
	Buffer g_pIndexBuffer;
	int m_vertexCount;
	vector<unsigned int> m_indices;
	int m_indicesCount;

	unsigned int m_numOfMesh = 0;

	Color m_color;
};
}
