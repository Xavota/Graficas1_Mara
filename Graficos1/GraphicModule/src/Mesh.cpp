#include "Mesh.h"
#include <iostream>

#include "RenderManager.h"

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
	if (vertices != nullptr)
	{
		m_vertices = new Vertex[count];
		memcpy(m_vertices, vertices, count * sizeof(Vertex));
		m_vertexCount = count; 
		
#if defined(DX11)

		RenderManager* renderManager = GetManager();

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * count;
		bd.BindFlags = BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		HRESULT hr = renderManager->CreateBuffer(&bd, &InitData, m_pVertexBuffer);/**/
		if (FAILED(hr))
			throw std::runtime_error("No se pudo crear el vertex buffer");

#endif
	}
}

void Mesh::setIndices(unsigned short* indices, int count)
{
	if (m_indices != nullptr) {
		delete m_indices;
		m_indices = nullptr;
		m_indicesCount = 0;
	}
	if (indices != nullptr)
	{
		m_indices = new unsigned short[count];
		memcpy(m_indices, indices, count * sizeof(unsigned short));
		m_indicesCount = count;

		RenderManager* renderManager = GetManager();

#if defined(DX11)

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * count;
		bd.BindFlags = BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices;
		HRESULT hr = renderManager->CreateBuffer(&bd, &InitData, g_pIndexBuffer);/**/
		if (FAILED(hr))
		 	throw std::runtime_error("No se pudo crear el index buffer");

#endif
	}
}
