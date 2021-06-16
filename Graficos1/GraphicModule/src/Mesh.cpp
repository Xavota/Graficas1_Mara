#include "Mesh.h"
#include <iostream>

#include "RenderManager.h"

namespace GraphicsModule
{
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
	m_ID = getNewID();
	//m_vertices = nullptr;
	//m_indices = nullptr;
	setVertex(vertices);
	setIndices(indices);
}

Mesh::Mesh()
{
	m_ID = getNewID();
	//m_vertices = nullptr;
	//m_indices = nullptr;
}

Mesh::~Mesh()
{
	//m_vertices.clear();
	//m_indices.clear();
}

unsigned int Mesh::getNewID()
{
	static unsigned int ID = -1;
	return ++ID;
}

HRESULT Mesh::setVertex(vector<Vertex> vertices)
{
	/*if (m_vertices.size() != 0) {
		m_vertices.clear();
	}*/
	if (vertices.size() != 0)
	{
		//m_vertices = new Vertex[count];
		//memcpy(m_vertices, vertices, count * sizeof(Vertex));
		//m_vertices = vertices;
		/*for (int i = 0; i < vertices.size(); ++i)
		{
			m_vertices.push_back(vertices[i]);
		}*/

		RenderManager* renderManager = GetManager();
		
#if defined(DX11)

		m_pVertexBuffer.Release();

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * vertices.size();
		bd.BindFlags = BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices.data();
		HRESULT hr = renderManager->CreateBuffer(&bd, &InitData, m_pVertexBuffer);/**/
		if (FAILED(hr))
			return hr;

#elif defined(OGL)

		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);


		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
#endif
	}
	return S_OK;
}

HRESULT Mesh::setIndices(vector<unsigned int> indices)
{
	/*if (m_indices.size() != 0) {
		m_indices.clear();
	}*/
	if (indices.size() != 0)
	{
		//m_indices = new unsigned int[count];
		//memcpy(m_indices, indices, count * sizeof(unsigned int));
		//m_indices = indices;

		m_indicesCount = indices.size();

		RenderManager* renderManager = GetManager();

#if defined(DX11)

		g_pIndexBuffer.Release();

		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * indices.size();
		bd.BindFlags = BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices.data();
		HRESULT hr = renderManager->CreateBuffer(&bd, &InitData, g_pIndexBuffer);/**/
		if (FAILED(hr))
		 	return hr;

#elif defined(OGL)
		glBindVertexArray(VAO);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
#endif
	}
	return S_OK;
}
void Mesh::Draw(RenderManager* renderManager)
{
#if defined(DX11)/*
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	renderManager->IASetVertexBuffers(0, 1, m_pVertexBuffer, &stride, &offset);
	renderManager->IASetIndexBuffer(g_pIndexBuffer, FORMAT_R32_UINT, offset);*/

	//renderManager->getShader().Use();
	//renderManager->getShader().Draw(m_indicesCount);
	// TODO: Hacer que funcione lo de que tengas los modelos dentro de los pases y les digan que se dibujen.

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	renderManager->IASetVertexBuffers(0, 1, m_pVertexBuffer, &stride, &offset);
	renderManager->IASetIndexBuffer(g_pIndexBuffer, FORMAT_R32_UINT, offset);

	renderManager->DrawIndexed(m_indicesCount, 0, 0);

#elif defined(OGL)
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, (const void*)0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
#endif           
}
void Mesh::SetResources(RenderManager* renderManager)
{
#if defined(DX11)
#elif defined(OGL)
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, (const void*)0);
#endif
}
}