#pragma once
#if !defined(OGL)
#include <windows.h>
#else
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#endif

class Window
{
public: 
	Window();
	~Window();

	void Init();

#if !defined(OGL)
	LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
#else
	void processInputs(GLFWwindow* window);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
#endif

private:
#if !defined(OGL)
	HWND m_window;
#else
	GLFWwindow* m_window;
#endif
};

