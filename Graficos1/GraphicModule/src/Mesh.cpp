#include "Mesh.h"
#include <iostream>

#include "RenderManager.h"

namespace GraphicsModule
{
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int numOfMesh)
{
	m_numOfMesh = numOfMesh;
	//m_vertices = nullptr;
	//m_indices = nullptr;
	setVertex(vertices);
	setIndices(indices);
}

Mesh::Mesh(unsigned int numOfMesh)
{
	m_numOfMesh = numOfMesh;
	//m_vertices = nullptr;
	//m_indices = nullptr;
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_indices.clear();
}

HRESULT Mesh::setVertex(vector<Vertex> vertices)
{
	if (m_vertices.size() != 0) {
		m_vertices.clear();
	}
	if (vertices.size() != 0)
	{
		//m_vertices = new Vertex[count];
		//memcpy(m_vertices, vertices, count * sizeof(Vertex));
		m_vertices = vertices;
		/*for (int i = 0; i < vertices.size(); ++i)
		{
			m_vertices.push_back(vertices[i]);
		}*/
		
#if defined(DX11)

		m_pVertexBuffer.Release();

		RenderManager* renderManager = GetManager();

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * m_vertices.size();
		bd.BindFlags = BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_vertices.data();
		HRESULT hr = renderManager->CreateBuffer(&bd, &InitData, m_pVertexBuffer);/**/
		if (FAILED(hr))
			return hr;

#endif
	}
}

HRESULT Mesh::setIndices(vector<unsigned int> indices)
{
	if (m_indices.size() != 0) {
		m_indices.clear();
	}
	if (indices.size() != 0)
	{
		//m_indices = new unsigned int[count];
		//memcpy(m_indices, indices, count * sizeof(unsigned int));
		m_indices = indices;
		m_indicesCount = m_indices.size();

		RenderManager* renderManager = GetManager();

#if defined(DX11)

		g_pIndexBuffer.Release();

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * m_indices.size();
		bd.BindFlags = BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_indices.data();
		HRESULT hr = renderManager->CreateBuffer(&bd, &InitData, g_pIndexBuffer);/**/
		if (FAILED(hr))
		 	return hr;

#endif
	}
}
}