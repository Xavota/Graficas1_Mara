#include <windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#if defined(DX11)
#include "imgui_impl_dx11.h"
#endif
#include "GraphicModule.h"

#include "Camara.h"
#include "Mouse.h"
#include "Mesh.h"
#include "OBJInstance.h"
#include "TextureManager.h"

#include "Descriptors.h"


#include <chrono>
//using namespace chrono;
//using namespace chrono;

// -----------------Global var-----------------------------------------------------------------
HWND g_hwnd;
GraphicsModule::test g_Test;

GraphicsModule::Camara* g_Cameras;
char                                g_activeCamera = 0;
char                                g_CameraCount = 2;

GraphicsModule::Mesh g_Mesh;
GraphicsModule::OBJInstance* g_ObjInstances;

/**
 * @brief   Forward declare message handler from imgui_impl_win32.cpp
 * @param   #HWND: A handle to the window.
 * @param   #UINT: The message.
 * @param   #WPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @param   #LPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @return  #LRESULT: The return value is the result of the message processing and depends on the message sent..
 * @bug     No know Bugs.
 * @return  #LRESULT: Status code.
 */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

/**
 * @brief   Message bomb.
 * @param   #HWND: A handle to the window.
 * @param   #UINT: The message.
 * @param   #WPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @param   #LPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
 * @return  #LRESULT: The return value is the result of the message processing and depends on the message sent..
 */
LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
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
            RECT rc;
            GetClientRect(_hwnd, &rc);

            UINT width = rc.right - rc.left;
			UINT height = rc.bottom - rc.top;

			for (int i = 0; i < g_CameraCount; i++)
			{
				g_Cameras[i].setViewWidth(width);
				g_Cameras[i].setViewHeight(height);
			}

            g_Test.Resize(width, height);
        }
        _first = !_first;
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
		if (LOWORD(_wParam) == 'D')
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
            g_activeCamera = (g_activeCamera + 1) % g_CameraCount;

		break;
        }
    }
    return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

/**
 * @brief   Set the style for the main window and init it.
 * @param   #unsigned int: First window width.
 * @param   #unsigned int: First window height.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
HRESULT InitWindow(LONG _width, LONG _height)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = nullptr;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "TutorialWindowClass";
    wcex.hIconSm = nullptr;
    if (!RegisterClassEx(&wcex))
    {
        return E_FAIL;
    }

    // Create window
    RECT rc = { 0, 0, _width, _height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hwnd = CreateWindow("TutorialWindowClass", "Graficos 1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, NULL, NULL);
    if (!g_hwnd)
    {
        return E_FAIL;
    }
	ShowWindow(g_hwnd, SW_SHOWNORMAL);

    return S_OK;
}

/**
 * @brief   Init the UI.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
HRESULT InitImgUI()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer back ends
    ImGui_ImplWin32_Init(g_hwnd);
#if defined(DX11)

    ImGui_ImplDX11_Init(g_Test.GetDevice(), g_Test.GetDeviceContext());
#endif

    return S_OK;
}

HRESULT Init(unsigned int width, unsigned int height)
{
	g_Cameras = new GraphicsModule::Camara[2];
	g_Cameras[0].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, true, PIDIV4);
	g_Cameras[1].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, false, PIDIV4);
    
    HRESULT hr;

	hr = g_Mesh.setVertex(new Vertex[24]{
			{ Vector3{-1.0f, 1.0f, -1.0f},  Vector2{0.0f, 0.0f},    Vector3{0.0f, 1.0f, 0.0f}  },
			{ Vector3{1.0f, 1.0f, -1.0f},   Vector2{1.0f, 0.0f},    Vector3{0.0f, 1.0f, 0.0f}  },
			{ Vector3{1.0f, 1.0f, 1.0f},    Vector2{1.0f, 1.0f},    Vector3{0.0f, 1.0f, 0.0f}  },
			{ Vector3{-1.0f, 1.0f, 1.0f},   Vector2{0.0f, 1.0f},    Vector3{0.0f, 1.0f, 0.0f}  },

			{ Vector3{-1.0f, -1.0f, -1.0f}, Vector2{0.0f, 0.0f},    Vector3{0.0f, -1.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, -1.0f},  Vector2{1.0f, 0.0f},    Vector3{0.0f, -1.0f, 0.0f} },
			{ Vector3{1.0f, -1.0f, 1.0f},   Vector2{1.0f, 1.0f},    Vector3{0.0f, -1.0f, 0.0f} },
			{ Vector3{-1.0f, -1.0f, 1.0f},  Vector2{0.0f, 1.0f},    Vector3{0.0f, -1.0f, 0.0f} },

			{ Vector3{-1.0f, -1.0f, 1.0f},  Vector2{0.0f, 0.0f},    Vector3{-1.0f, 0.0f, 0.0f} },
			{ Vector3{-1.0f, -1.0f, -1.0f}, Vector2{1.0f, 0.0f},    Vector3{-1.0f, 0.0f, 0.0f} },
			{ Vector3{-1.0f, 1.0f, -1.0f},  Vector2{1.0f, 1.0f},    Vector3{-1.0f, 0.0f, 0.0f} },
			{ Vector3{-1.0f, 1.0f, 1.0f},   Vector2{0.0f, 1.0f},    Vector3{-1.0f, 0.0f, 0.0f} },

			{ Vector3{1.0f, -1.0f, 1.0f},   Vector2{0.0f, 0.0f},    Vector3{1.0f, 0.0f, 0.0f}  },
			{ Vector3{1.0f, -1.0f, -1.0f},  Vector2{1.0f, 0.0f},    Vector3{1.0f, 0.0f, 0.0f}  },
			{ Vector3{1.0f, 1.0f, -1.0f},   Vector2{1.0f, 1.0f},    Vector3{1.0f, 0.0f, 0.0f}  },
			{ Vector3{1.0f, 1.0f, 1.0f},    Vector2{0.0f, 1.0f},    Vector3{1.0f, 0.0f, 0.0f}  },

			{ Vector3{-1.0f, -1.0f, -1.0f}, Vector2{0.0f, 0.0f},    Vector3{0.0f, 0.0f, -1.0f} },
			{ Vector3{1.0f, -1.0f, -1.0f},  Vector2{1.0f, 0.0f},    Vector3{0.0f, 0.0f, -1.0f} },
			{ Vector3{1.0f, 1.0f, -1.0f},   Vector2{1.0f, 1.0f},    Vector3{0.0f, 0.0f, -1.0f} },
			{ Vector3{-1.0f, 1.0f, -1.0f},  Vector2{0.0f, 1.0f},    Vector3{0.0f, 0.0f, -1.0f} },

			{ Vector3{-1.0f, -1.0f, 1.0f},  Vector2{0.0f, 0.0f},    Vector3{0.0f, 0.0f, 1.0f}  },
			{ Vector3{1.0f, -1.0f, 1.0f},   Vector2{1.0f, 0.0f},    Vector3{0.0f, 0.0f, 1.0f}  },
			{ Vector3{1.0f, 1.0f, 1.0f},    Vector2{1.0f, 1.0f},    Vector3{0.0f, 0.0f, 1.0f}  },
			{ Vector3{-1.0f, 1.0f, 1.0f},   Vector2{0.0f, 1.0f},    Vector3{0.0f, 0.0f, 1.0f}  } }, 24);

	if (FAILED(hr))
		return hr;




	// Create index buffer
	hr = g_Mesh.setIndices(new unsigned short[36]{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
		}, 36
	);

	if (FAILED(hr))
		return hr;

	g_ObjInstances = new GraphicsModule::OBJInstance[1];

	g_ObjInstances[0].setMesh(&g_Mesh);
	g_ObjInstances[0].setPosition({ 0,0,2 });
	g_ObjInstances[0].setRotation({ 0,0,3.14159265 });
	g_ObjInstances[0].setSize({ 1,1,1 });

	TextureManager::CreateTextureFromFile("pepe.dds", "Pepe");
	g_ObjInstances[0].setTexture(TextureManager::GetTexture("Pepe"));

    return S_OK;
}

void UIRender()
{
    // Start the Dear ImGui frame
#if defined(DX11)
    ImGui_ImplDX11_NewFrame();
#endif
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // example window
    if (ImGui::Begin("Another Window", nullptr))
	{
		static float dir[3]{};
		if (ImGui::DragFloat3("Direccion de la luz :)", dir, 0.001f, -1.0f, 1.0f))
		{
			g_Test.SetDirLight(XMFLOAT4(dir[0], dir[1], dir[2], 0));
		}
    }
    ImGui::End();

    // render UI
    ImGui::Render();
#if defined(DX11)
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Update(float dt)
{
    g_Test.Update(dt);

    /*Update the mouse position*/
    LPPOINT p = new POINT;
    GetCursorPos(p);
    GraphicsModule::Mouse::setMousePos({ (float)p->x, -(float)p->y, 0 });
    delete p;


    /*Update the active camera*/
    g_Cameras[g_activeCamera].Update();


    /*Set the new view and projection matrices*/
    g_Test.GetRenderManager()->UpdateViewMatrix(g_Cameras[g_activeCamera].getViewMatrix());
    g_Test.GetRenderManager()->UpdateProjectionMatrix(g_Cameras[g_activeCamera].getProjectionMatrix());


    static float dtt = 0;
    dtt += dt;


    // Modify the color
    /*(sinf(t * 1.0f) + 1.0f) * 0.5f;
      (cosf(t * 3.0f) + 1.0f) * 0.5f;
      (sinf(t * 5.0f) + 1.0f) * 0.5f;/**/
    Color g_vMeshColor;
    g_vMeshColor.r = .7;
    g_vMeshColor.g = .7;
    g_vMeshColor.b = .7;/**/

    g_ObjInstances[0].getMesh()->setColor(g_vMeshColor);

    static GraphicsModule::Vector rotation_c{ 0,0,0 };
    rotation_c.setVector(0, dtt, 3.14159265);

    g_ObjInstances[0].setRotation(rotation_c);
}

void Render()
{
	g_Test.Render();
	GraphicsModule::GetManager()->DrawObject(&g_ObjInstances[0]);
#if defined(DX11) || defined(OGL)
    UIRender();
#endif
#if defined(DX11) || defined(OGL)
    g_Test.GetSwapChain()->Present(0, 0);
#endif
}

/**
 * @brief   Entry point.
 * @bug     No know Bugs.
 * @return  #int: Status code.
 */
int main()
{
    // create the window and console
    if (FAILED(InitWindow(1280, 720)))
    {
        DestroyWindow(g_hwnd);
        return 0;
	}

	g_Test = GraphicsModule::GetTestObj(g_hwnd);

    // create Graphic API interface
    //if (FAILED(g_Test.InitDevice(g_hwnd)))
    //{
       // g_Test.CleanupDevice();
        //return 0;
    //}

    // create UI
    if (FAILED(InitImgUI()))
    {
#if defined(DX11)
        ImGui_ImplDX11_Shutdown();
#endif
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        return 0;
    }

    if (FAILED(Init(1280, 720)))
	{
		DestroyWindow(g_hwnd);
#if defined(DX11)
		ImGui_ImplDX11_Shutdown();
#endif
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		return 0;
        
    }

    // main loop
    MSG msg = { 0 };
    //auto start = chrono::high_resolution_clock::now();
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
            //auto end = high_resolution_clock::now();
			//g_Test.Update(duration<double>(end - start).count());
			Update(.003f);
            //start = high_resolution_clock::now();
        }
    }

    // clean resources
#if defined(DX11)
    ImGui_ImplDX11_Shutdown(),
#endif
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    g_Test.CleanupDevice();
    DestroyWindow(g_hwnd);
    return (int)msg.wParam;
}