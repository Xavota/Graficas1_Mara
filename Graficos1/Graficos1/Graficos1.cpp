#pragma comment(lib, "ComDlg32.lib")

#include <windows.h>

#include "imgui.h"
#if !defined(OGL)
#include "imgui_impl_win32.h"
#endif
#if defined(DX11)
#include "imgui_impl_dx11.h"
#elif defined(OGL)
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#endif


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GraphicModule.h"

#include "Camara.h"
#include "Mouse.h"
#include "Mesh.h"
#include "OBJInstance.h"
#include "TextureManager.h"

#include "Descriptors.h"

#include <string.h>
#include <iostream>
#include <vector>
#include <list>

#include <chrono>
using namespace std;
//using namespace chrono;

#include "Shader.h"
GraphicsModule::Shader shader;
#if defined(OGL)
glm::mat4 model(1.0f);
#endif
// -----------------Global var-----------------------------------------------------------------
HWND g_hwnd;
GraphicsModule::test g_Test;

GraphicsModule::Camara* g_Cameras;
char                                g_activeCamera = 0;
char                                g_CameraCount = 2;

vector<GraphicsModule::Mesh> g_Mesh;
vector<GraphicsModule::OBJInstance> g_ObjInstances;

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
 #if !defined(OGL)
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

    case WM_LBUTTONDOWN:
        GraphicsModule::Mouse::setPressed(true);
		break;

	case WM_LBUTTONUP:
		GraphicsModule::Mouse::setPressed(false);
		break;
    }
    return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}
#endif

#if defined(OGL)
void processInputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
		g_activeCamera = (g_activeCamera + 1) % g_CameraCount;

	if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		GraphicsModule::Mouse::setPressed(true);
	

	if (glfwGetMouseButton(window, 1) == GLFW_RELEASE)
		GraphicsModule::Mouse::setPressed(false);
}
#endif
#if defined(OGL)
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	GraphicsModule::Mouse::setMousePos({ (float)xpos, -(float)ypos, 0 });
}
#endif
/**
 * @brief   Set the style for the main window and init it.
 * @param   #unsigned int: First window width.
 * @param   #unsigned int: First window height.
 * @bug     No know Bugs.
 * @return  #HRESULT: Status code.
 */
HRESULT InitWindow(LONG _width, LONG _height)
{
#if !defined(OGL)
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
#else
	if (!glfwInit())
		return 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GraphicsModule::GetManager()->setOGLWindow(glfwCreateWindow(_width, _height, "Graficas", NULL, NULL));
	if (GraphicsModule::GetManager()->getOGLWindow() == NULL)
		return E_FAIL;

	glfwMakeContextCurrent(GraphicsModule::GetManager()->getOGLWindow());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#endif

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
#if defined(DX11)

	ImGui_ImplDX11_Init(g_Test.GetDevice(), g_Test.GetDeviceContext());
	ImGui_ImplWin32_Init(g_hwnd);

#elif defined(OGL)
	ImGui_ImplGlfw_InitForOpenGL(GraphicsModule::GetManager()->getOGLWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
#endif

    return S_OK;
}

string OpenFileGetName(HWND owner = NULL)
{

	// common dialog box structure, setting all fields to 0 is important
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };

	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = ("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return szFile;
	}
    return "";
}

HRESULT Init(unsigned int width, unsigned int height)
{    
	g_Cameras = new GraphicsModule::Camara[2];
	g_Cameras[0].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, true, PIDIV4);
	g_Cameras[1].Init({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, false, PIDIV4);

	GraphicsModule::TextureManager::CreateTextureFromFile("Models/Textures/M_BaseTexture_Albedo.jpg", "Base Texture");
    
#if defined(OGL)
	glfwSetCursorPosCallback(GraphicsModule::GetManager()->getOGLWindow(), mouse_callback);
#endif
    
    return S_OK;
}

void LoadMesh(const aiScene* scene, string fileName)
{    
    g_ObjInstances.push_back(GraphicsModule::OBJInstance());
    size_t lastOBJ = g_ObjInstances.size() - 1;

    g_ObjInstances[lastOBJ].LoadModel(scene, fileName);

	g_ObjInstances[lastOBJ].setPosition({ 0,0,0 });
	g_ObjInstances[lastOBJ].setRotation({ 0,0,0 });
	g_ObjInstances[lastOBJ].setSize({ 1,1,1 });
}

void OpenMesh()
{
    string fileName = OpenFileGetName();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    if (!scene)
    {
        cout << importer.GetErrorString() << endl;
        return;
    }

	LoadMesh(scene, fileName);
}

void UIRender()
{
    // Start the Dear ImGui frame
#if defined(DX11)
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();	
#elif defined(OGL)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif
    ImGui::NewFrame();

    // example window
    if (ImGui::Begin("Another Window", nullptr))
	{
		static float dir[3]{ 0.0f, -0.5f, 1.0f };
		ImGui::Text("Light Direction:");
		if (ImGui::DragFloat3("Dir", dir, 0.001f, -1.0f, 1.0f))
		{
			g_Test.SetDirLight(Vector4{dir[0], dir[1], dir[2], 0});
		}
        ImGui::Separator();
        if (ImGui::Button("Open Mesh", ImVec2(100, 30)))
        {
            OpenMesh();
        }
        if (ImGui::CollapsingHeader("Meshes"))
		{
			size_t countOBJs = g_ObjInstances.size();
			for (int i = 0; i < countOBJs; i++)
			{
                ImGui::PushID(i);
			    if (ImGui::CollapsingHeader(to_string(i).c_str()))
			    {
			    	if (ImGui::CollapsingHeader("Transform"))
			    	{
			    		float pos[3]{ g_ObjInstances[i].getPosition().x(), g_ObjInstances[i].getPosition().y(), g_ObjInstances[i].getPosition().z() };
			    		if (ImGui::DragFloat3("Position", pos, 0.01f))
			    		{
			    			g_ObjInstances[i].setPosition({ pos[0], pos[1], pos[2] });
						}
						float rot[3]{ g_ObjInstances[i].getRotation().x() * (180 / 3.1415f), g_ObjInstances[i].getRotation().y() * (180 / 3.1415f), g_ObjInstances[i].getRotation().z() * (180 / 3.1415f) };
			    		if (ImGui::DragFloat3("Rotation", rot, 0.1f, 360, -360))
			    		{
			    			g_ObjInstances[i].setRotation({ rot[0] * (3.1415f / 180), rot[1] * (3.1415f / 180), rot[2] * (3.1415f / 180) });
			    		}
			    		float scale[3]{ g_ObjInstances[i].getSize().x(), g_ObjInstances[i].getSize().y(), g_ObjInstances[i].getSize().z() };
			    		if (ImGui::DragFloat3("Scale", scale, 0.01f))
			    		{
			    			g_ObjInstances[i].setSize({ scale[0], scale[1], scale[2] });
			    		}
			    	}
			    	if (ImGui::CollapsingHeader("Info"))
					{
                        ImGui::Text(g_ObjInstances[i].getName().c_str());
						if (ImGui::CollapsingHeader("Textures"))
						{
                            for (int j = 0; j < g_ObjInstances[i].getTextureCount(); ++j)
                            {
								float my_tex_w = 512;
								float my_tex_h = 512;
                                ImTextureID my_tex_id = ImTextureID();
#if defined(DX11)
                                my_tex_id = g_ObjInstances[i].getTexture(j).getBuffer().getPtr();
#elif defined(OGL)
								my_tex_id = (void*)g_ObjInstances[i].getTexture(j).getID();
#endif           
								ImVec2 pos = ImGui::GetCursorScreenPos();
								ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
								ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
								ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
								ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
								ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
                            }
                        }
			    	}
                }
                ImGui::PopID();
            }
        }
    }
    //ImGui::ShowDemoWindow();
    ImGui::End();

    // render UI
    ImGui::Render();
#if defined(DX11)
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif defined(OGL)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Update(float dt)
{
    g_Test.Update(dt);

    /*Update the mouse position*/
#if defined(DX11)
    LPPOINT p = new POINT;
    GetCursorPos(p);
    GraphicsModule::Mouse::setMousePos({ (float)p->x, -(float)p->y, 0 });
    delete p;
#endif


    /*Update the active camera*/
    g_Cameras[g_activeCamera].Update();


    /*Set the new view and projection matrices*/
    g_Test.GetRenderManager()->UpdateViewMatrix(g_Cameras[g_activeCamera].getViewMatrix());
    g_Test.GetRenderManager()->UpdateProjectionMatrix(g_Cameras[g_activeCamera].getProjectionMatrix());
}

void Render()
{
#if defined(DX11) || defined(OGL)
	g_Test.Clear();

	size_t countOBJs = g_ObjInstances.size();
	for (int i = 0; i < countOBJs/**//*(countOBJs == 0 ? 0 : 1)/**/; i++)
	{
        g_ObjInstances[i].Draw(GraphicsModule::GetManager());
    }

	UIRender();

	g_Test.Display();
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
#if defined(OGL)
		glfwDestroyWindow(GraphicsModule::GetManager()->getOGLWindow());
        glfwTerminate();
#endif
        return 0;
	}

	g_Test = GraphicsModule::GetTestObj(g_hwnd);

    // create UI
    if (FAILED(InitImgUI()))
    {
#if defined(DX11)
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
#elif defined(OGL)
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#endif
        ImGui::DestroyContext();
        return 0;
    }

    if (FAILED(Init(1280, 720)))
	{
#if defined(DX11)
		DestroyWindow(g_hwnd);
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
#elif defined(OGL)
		glfwDestroyWindow(GraphicsModule::GetManager()->getOGLWindow());
		glfwTerminate();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#endif
		ImGui::DestroyContext();
		return 0;
        
	}

    // main loop
    MSG msg = { 0 };
    //auto start = chrono::high_resolution_clock::now();
#if !defined(OGL)
	while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
		{
			//auto end = high_resolution_clock::now();
			//g_Test.Update(duration<double>(end - start).count());
			Update(.003f);
			//start = high_resolution_clock::now();
            Render();
        }
    }
#else
	while (!glfwWindowShouldClose(GraphicsModule::GetManager()->getOGLWindow()))
	{
		processInputs(GraphicsModule::GetManager()->getOGLWindow());
		//auto end = high_resolution_clock::now();
		//g_Test.Update(duration<double>(end - start).count());
		Update(.003f);
		//start = high_resolution_clock::now();
		Render();
		glfwPollEvents();
	}
#endif

    // clean resources
#if defined(DX11)
	DestroyWindow(g_hwnd);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
#elif defined(OGL)
	glfwDestroyWindow(GraphicsModule::GetManager()->getOGLWindow());
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
#endif
    ImGui::DestroyContext();
    g_Test.CleanupDevice();
    return (int)msg.wParam;
}