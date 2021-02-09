#include "Mesh.h"
#include <iostream>

Mesh::Mesh(Vertex* vertices, int verticesCount, unsigned short* indices, int indicesCount)
{
	m_vertices = nullptr;
	m_indices = nullptr;
	setVertex(vertices, verticesCount);
	setIndices(indices, indicesCount);
}

Mesh::Mesh()
{
	m_vertices = nullptr;
	m_indices = nullptr;
	setVertex(nullptr, 0);
	setIndices(nullptr, 0);
}

Mesh::~Mesh()
{
	if (m_vertices != nullptr) {
		delete m_vertices;
		m_vertices = nullptr;
		m_vertexCount = 0;
	}
	if (m_indices != nullptr) {
		delete m_indices;
		m_indices = nullptr;
		m_indicesCount = 0;
	}
}

void Mesh::setVertex(Vertex* vertices, int count)
{
	if (m_vertices != nullptr) {
		delete m_vertices;
		m_vertices = nullptr;
		m_vertexCount = 0;
	}
	m_vertices = new Vertex[count];
	memcpy(m_vertices, vertices, count * sizeof(Vertex));
	m_vertexCount = count;
}

void Mesh::setIndices(unsigned short* indices, int count)
{
	if (m_indices != nullptr) {
		delete m_indices;
		m_indices = nullptr;
		m_indicesCount = 0;
	}
	m_indices = new unsigned short[count];
	memcpy(m_indices, indices, count * sizeof(unsigned short));
	m_indicesCount = count;
}
