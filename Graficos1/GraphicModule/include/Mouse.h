#pragma once
#include "Vector.h"

namespace GraphicsModule
{
class Mouse
{
public:
	~Mouse();
	static void   setMousePos(Vector pos);
	static Vector getMousePos();
	static Vector getMouseMovement();

	static void setPressed(bool pr) { m_pressed = pr; }

	static bool isPressed() { return m_pressed; }
private:
	Mouse();

	static Vector m_pos;
	static Vector m_movement;

	static bool m_pressed;
};
}