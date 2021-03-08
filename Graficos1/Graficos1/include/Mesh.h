#pragma once
#include "ShaderResourceView.h"
#include "Descriptors.h"
#include "Buffer.h"


class Mesh
{
public:
	Mesh(Vertex* vertices, int verticesCount, unsigned short* indices, int indicesCount);
	Mesh();
	~Mesh();

	void setVertex(Vertex* vertices, int count);
	Vertex* getVertex() { return m_vertices; }
	int getVertexCount() { return m_vertexCount; }

	void setIndices(unsigned short* indices, int count);
	unsigned short* getIndices() { return m_indices; }
	int getIndexCount() { return m_indicesCount; }

	Color getColor() { return m_color; }
	void setColor(Color color) { m_color = color; }

	Buffer& getVertexBuffer() { return m_pVertexBuffer; }
	//void setVertexBuffer(Buffer vertexBuffer) { m_pVertexBuffer = vertexBuffer; }

	Buffer& getIndexBuffer() { return g_pIndexBuffer; }

private:
	Vertex* m_vertices;
	Buffer m_pVertexBuffer;
	Buffer g_pIndexBuffer;
	int m_vertexCount;
	unsigned short* m_indices;
	int m_indicesCount;

	Color m_color;
};
