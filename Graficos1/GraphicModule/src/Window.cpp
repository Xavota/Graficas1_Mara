#include "Window.h"

#include "RenderManager.h"

#if !defined(OGL)
#include "imgui_impl_win32.h"
#endif

#if !defined(OGL)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

LRESULT CALLBACK Window::WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{

	// Handle UI inputs
	if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
		return 1;

	// Handle Window inputs
	switch (_msg)
	{
	case WM_SIZE:
		//if (g_pd3dDevice != NULL && _wParam != SIZE_MINIMIZED)
	{
		static bool _first = true;
		if (!_first)
		{
			/*Update viewport*/
			int width = 0, height = 0;
			RECT rc;
			GetClientRect(GraphicsModule::GetManager()->GetWindow(), &rc);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			//Resize(width, height);
		}
		_first = false;
	}
	return 0;
	break;

	case WM_SYSCOMMAND:
		if ((_wParam & 0xfff0) == SC_KEYMENU)
		{
			return 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: {
		/*if (LOWORD(_wParam) == 'D')
			g_Cameras[g_activeCamera].move({ 1, 0, 0 });
		if (LOWORD(_wParam) == 'A')
			g_Cameras[g_activeCamera].move({ -1, 0, 0 });
		if (LOWORD(_wParam) == 'W')
			g_Cameras[g_activeCamera].move({ 0, 1, 0 });
		if (LOWORD(_wParam) == 'S')
			g_Cameras[g_activeCamera].move({ 0, -1, 0 });
		if (LOWORD(_wParam) == 'Q')
			g_Cameras[g_activeCamera].move({ 0, 0, 1 });
		if (LOWORD(_wParam) == 'E')
			g_Cameras[g_activeCamera].move({ 0, 0, -1 });
		if (LOWORD(_wParam) == 9)
			g_activeCamera = (g_activeCamera + 1) % g_Cameras.size();*/
		break;
	}

	case WM_RBUTTONDOWN:
		//GraphicsModule::Mouse::setPressed(true);
		break;

	case WM_RBUTTONUP:
		//GraphicsModule::Mouse::setPressed(false);
		break;
	}

	LPPOINT p = new POINT;
	GetCursorPos(p);
	//GraphicsModule::Mouse::setMousePos({ (float)p->x, -(float)p->y, 0 });
	delete p;

	return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

#else

void Window::processInputs(GLFWwindow* window)
{
	/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_Cameras[g_activeCamera].move({ 1, 0, 0 });
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_Cameras[g_activeCamera].move({ -1, 0, 0 });
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_Cameras[g_activeCamera].move({ 0, 1, 0 });
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_Cameras[g_activeCamera].move({ 0, -1, 0 });
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		g_Cameras[g_activeCamera].move({ 0, 0, 1 });
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		g_Cameras[g_activeCamera].move({ 0, 0, -1 });
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		g_activeCamera = (g_activeCamera + 1) % g_Cameras.size();

	if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		GraphicsModule::Mouse::setPressed(true);


	if (glfwGetMouseButton(window, 1) == GLFW_RELEASE)
		GraphicsModule::Mouse::setPressed(false);*/
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//GraphicsModule::Mouse::setMousePos({ (float)xpos, -(float)ypos, 0 });
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//Resize(width, height);
}

#endif

void Window::Init()
{
}
