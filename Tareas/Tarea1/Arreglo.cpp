#include "Arreglo.h"

Arreglo::Arreglo()
{
	m_size = sizeof(char);
	m_x = 1;
	m_y = 1;
	setP();
	setStart();
}

Arreglo::Arreglo(int size, int y, int x)
{
	m_size = size;
	m_x = x;
	m_y = y;
	setP();
	setStart();
}

Arreglo::~Arreglo()
{
	if (m_start != nullptr) {
		delete m_start;
		m_start = nullptr;
	}
}

void Arreglo::setVal(void*** val, int sizey, int sizex)
{
	if (sizey > 0) {
		m_y = sizey;
	}
	if (sizex > 0) {
		m_x = sizex;
	}
	for (int i = 0; i < m_y; i++) 
	{
		for (int j = 0; j < m_x; j++) 
		{
			for (int k = 0; k < m_size; k++)
			{
				*(char*)((char*)m_start + i * m_p + j * m_size + k) = *(char*)((char*)val[i][j] + k);
			}
		}
	}
}

void Arreglo::setVal(void* val, int y, int x)
{
	for (int k = 0; k < m_size; k++)
	{
		*(char*)((char*)m_start + y * m_p + x * m_size + k) = *(char*)((char*)val + k);
	}
}

void*** Arreglo::getVal()
{
	return (void***)m_start;
}

void* Arreglo::getVal(int y, int x)
{
	return ((char*)m_start + y * m_p + x * m_size);
}

void Arreglo::setP()
{
	m_p = m_size * m_x;
}

void Arreglo::setStart()
{
	m_start = (void*)new char[m_x * m_y * m_size];
	std::memset(m_start, 0, m_x * m_y * m_size * sizeof(char));
}
