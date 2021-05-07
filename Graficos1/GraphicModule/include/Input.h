#pragma once
#include <vector>
class Input
{
public:
	enum class Keyboard
	{
		NULLKEY = -1,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		TAB, BLOCK_MAYUS, LSHIFT, LCTR, ESC, LATL, SPACE, BACKSPACE, RETURN, DEL, 
		RSHIFT, RCTRL, RALT, WIN, UP, DOWN, LEFT, RIGHT, COMA, POINT, 
		NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0, 
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};

	Input() = default;

	static bool GetKeyPressed(Keyboard key);
	static bool GetKey(Keyboard key);
	static bool GetKeyReleased(Keyboard key);
	static bool GetMouseButtonDown(char button);
	static bool GetMouseButton(char button);
	static bool GetMouseButtonUp(char button);
private:
	static void HandleInputs(Keyboard key, bool isPressed);
	static void HandleMouseInputs(char mouseBtn, bool isPressed);
	static void Update();

private:
	struct KEY
	{
		Keyboard keyType = Keyboard::NULLKEY;
		bool wasPressed = false;
		bool isPressed = false;
	};
	struct MOUSEBTN
	{
		char btnType = -1;
		bool wasPressed = false;
		bool isPressed = false;
	};
	
	static std::vector<KEY> m_keys;
	static std::vector<MOUSEBTN> m_mouseBtns;
};

