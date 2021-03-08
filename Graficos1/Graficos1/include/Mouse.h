#pragma once
#include "Vector.h"

class Mouse
{
public:
	~Mouse();
	static void   setMousePos(Vector pos);
	static Vector getMousePos();
	static Vector getMouseMovement();
private:
	Mouse();

	static Vector m_pos;
	static Vector m_movement;
};
