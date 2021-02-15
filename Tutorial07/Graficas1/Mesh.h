#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
struct Vector3 {
	float x;
	float y;
	float z;
};
struct Vector2 {
	float x;
	float y;
};
struct Vertex 
{
	Vector3 Pos;
	Vector2 Tex;
};

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

private:
	Vertex* m_vertices;
	int m_vertexCount;
	unsigned short* m_indices;
	int m_indicesCount;
};

