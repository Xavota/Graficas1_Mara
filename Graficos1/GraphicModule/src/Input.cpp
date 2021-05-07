#include "Input.h"

std::vector<Input::KEY> Input::m_keys;
std::vector<Input::MOUSEBTN> Input::m_mouseBtns;


bool Input::GetKeyPressed(Keyboard key)
{
	for (KEY& k : m_keys)
	{
		if (k.keyType == key)
			return k.isPressed && !k.wasPressed;
	}
	return false;
}

bool Input::GetKey(Keyboard key)
{
	for (KEY& k : m_keys)
	{
		if (k.keyType == key)
			return k.isPressed;
	}
	return false;
}

bool Input::GetKeyReleased(Keyboard key)
{
	for (KEY& k : m_keys)
	{
		if (k.keyType == key)
			return !k.isPressed && k.wasPressed;
	}
	return false;
}

bool Input::GetMouseButtonDown(char button)
{
	for (MOUSEBTN& k : m_mouseBtns)
	{
		if (k.btnType == button)
			return k.isPressed && !k.wasPressed;
	}
	return false;
}

bool Input::GetMouseButton(char button)
{
	for (MOUSEBTN& k : m_mouseBtns)
	{
		if (k.btnType == button)
			return k.isPressed;
	}
	return false;
}

bool Input::GetMouseButtonUp(char button)
{
	for (MOUSEBTN& k : m_mouseBtns)
	{
		if (k.btnType == button)
			return !k.isPressed && k.wasPressed;
	}
	return false;
}

void Input::HandleInputs(Keyboard key, bool isPressed)
{
	for (KEY& k : m_keys) {
		if (k.keyType == key) {
			k.isPressed = isPressed;
			return;
		}
	}

	m_keys.push_back(KEY{key, isPressed, false});
}

void Input::HandleMouseInputs(char mouseBtn, bool isPressed)
{
	for (MOUSEBTN& k : m_mouseBtns) {
		if (k.btnType == mouseBtn) {
			k.isPressed = isPressed;
			return;
		}
	}

	m_mouseBtns.push_back(MOUSEBTN{mouseBtn, isPressed, false});
}

void Input::Update()
{
	for (KEY& k : m_keys) {
		k.wasPressed = k.isPressed;
	}

	for (MOUSEBTN& button : m_mouseBtns) {
		button.wasPressed = button.isPressed;
	}
}