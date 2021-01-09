#pragma once
#include <iostream>
class Arreglo
{
public:
	Arreglo();
	Arreglo(int size, int y = 0, int x = 0);
	~Arreglo();

	void setX(int x) { m_x = x; setP(); setStart(); }
	void setY(int y) { m_y = y; setStart(); }
	void setValueSize(int size) { m_size = size; setP(); setStart(); }
	void setVal(void*** val, int sizey = NULL, int sizex = NULL);
	void setVal(void* val, int y, int x);

	int getX() { return m_x; }
	int getY() { return m_y; }
	int getP() { return m_p; }
	int getSize() { return m_size; }
	void* getPointer(int x, int y) { return m_start; }
	void*** getVal();
	void* getVal(int y, int x);

private:
	void setP();
	void setStart();

private:
	int m_x;
	int m_y;
	int m_p;
	int m_size;
	void* m_start;
};

