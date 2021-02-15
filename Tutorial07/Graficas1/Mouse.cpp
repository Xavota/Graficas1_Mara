#include "Mouse.h"

Vector Mouse::m_pos;
Vector Mouse::m_movement;
Mouse::~Mouse()
{
}

void Mouse::setMousePos(Vector pos)
{
	if (m_pos == Vector(0, 0, 0))
		m_movement = { 0,0,0 };
	else
		m_movement = pos - m_pos;
	m_pos = pos;
}

Vector Mouse::getMousePos()
{
	return m_pos;
}

Vector Mouse::getMouseMovement()
{
	return m_movement;
}

Mouse::Mouse()
{
}
