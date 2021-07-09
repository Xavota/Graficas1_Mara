#pragma comment(lib, "ComDlg32.lib")

#include <windows.h>

#include "imgui.h"
#if defined(DX11)
#include "imgui_impl_win32.h"
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
#include "Vector.h"

#include "Descriptors.h"

#include <string.h>
#include <iostream>
#include <vector>
#include <list>

#include <chrono>
using namespace std;
//using namespace chrono;

// -----------------Global var-----------------------------------------------------------------
GraphicsModule::test				g_Test;

vector<GraphicsModule::Camara>		g_Cameras;
char                                g_activeCamera = 0;

vector<GraphicsModule::Mesh>		g_Mesh;
vector<GraphicsModule::OBJInstance> g_ObjInstances;

bool								g_SelectingLoadMode = false;


void Resize(unsigned int width, unsigned int height)
{
	for (int i = 0; i < g_Cameras.size(); i++)
	{
		g_Cameras[i].setViewWidth(width);
		g_Cameras[i].setViewHeight(height);
	}

	g_Test.Resize(width, height);
}

#if !defined(OGL)
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
			/*Update viewport*/
			int width = 0, height = 0;
			RECT rc;
			GetClientRect(GraphicsModule::GetManager()->GetWindow(), &rc);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			Resize(width, height);
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
            g_activeCamera = (g_activeCamera + 1) % g_Cameras.size();
		break;
        }

    case WM_RBUTTONDOWN:
        GraphicsModule::Mouse::setPressed(true);
		break;

	case WM_RBUTTONUP:
		GraphicsModule::Mouse::setPressed(false);
		break;
    }

	LPPOINT p = new POINT;
	GetCursorPos(p);
	GraphicsModule::Mouse::setMousePos({ (float)p->x, -(float)p->y, 0 });
	delete p;

    return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

#else 
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
		g_activeCamera = (g_activeCamera + 1) % g_Cameras.size();

	if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		GraphicsModule::Mouse::setPressed(true);
	

	if (glfwGetMouseButton(window, 1) == GLFW_RELEASE)
		GraphicsModule::Mouse::setPressed(false);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	GraphicsModule::Mouse::setMousePos({ (float)xpos, -(float)ypos, 0 });
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Resize(width, height);
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
    GraphicsModule::GetManager()->SetWindow(CreateWindow("TutorialWindowClass", "Graficos 1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, NULL, NULL));
    if (!GraphicsModule::GetManager()->GetWindow())
    {
        return E_FAIL;
    }
	ShowWindow(GraphicsModule::GetManager()->GetWindow(), SW_SHOWNORMAL);
#else
	if (!glfwInit())
		return 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GraphicsModule::GetManager()->SetWindow(glfwCreateWindow(_width, _height, "Graficas", NULL, NULL));
	if (GraphicsModule::GetManager()->GetWindow() == NULL)
		return E_FAIL;

	glfwMakeContextCurrent(GraphicsModule::GetManager()->GetWindow());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(GraphicsModule::GetManager()->GetWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(GraphicsModule::GetManager()->GetWindow(), mouse_callback);

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
	ImGui_ImplWin32_Init(GraphicsModule::GetManager()->GetWindow());
#elif defined(OGL)
	ImGui_ImplGlfw_InitForOpenGL(GraphicsModule::GetManager()->GetWindow(), true);
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

//void OpenMesh(string fileName, unsigned int Flags, MATRIX mat, GraphicsModule::eDIMENSION dim);
void LoadMesh(string fileName, unsigned int Flags, MATRIX mat, GraphicsModule::eDIMENSION dim);

//GraphicsModule::OBJInstance SAQ;

HRESULT Init(unsigned int width, unsigned int height)
{
	g_Cameras.push_back(GraphicsModule::Camara({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, true, PIDIV4));
	g_Cameras.push_back(GraphicsModule::Camara({ 0.0f, 3.0f, -6.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		width, height, 0.01f, 100.0f, false, PIDIV4));

	GraphicsModule::TextureManager::CreateTextureFromFile({"Models/Textures/M_BaseTexture_Albedo.jpg"}, "Base Texture", MODEL_LOAD_FORMAT_RGBA, GraphicsModule::eDIMENSION::TEXTURE2D);
    //"Models/Models/CuboPuzzle.obj"
    //LoadMesh("Models/Models/Pistola.obj", MODEL_LOAD_FORMAT_TRIANGLES | MODEL_LOAD_FORMAT_BGRA, GraphicsModule::OBJInstance::getModelMatrix({ 75,75,75 }, { 0,0,0 }, { 0,0,0 }), GraphicsModule::eDIMENSION::TEXTURE2D);
    LoadMesh("Models/Models/boblampclean.md5mesh", MODEL_LOAD_FORMAT_TRIANGLES | MODEL_LOAD_FORMAT_BGRA, GraphicsModule::OBJInstance::getModelMatrix({ 1,1,1 }, { 0,0,0 }, { 0 * (3.1415 / 180),0 * (3.1415 / 180),0 }), GraphicsModule::eDIMENSION::TEXTURE2D);
	//LoadMesh("Models/Models/CuboPuzzle.obj", MODEL_LOAD_FORMAT_TRIANGLES | MODEL_LOAD_FORMAT_BGRA, GraphicsModule::OBJInstance::getModelMatrix({ 10,10,10 }, { 0,0,0 }, { 0,0,0 }), GraphicsModule::eDIMENSION::TEXTURE2D);

	LoadMesh("Models/Models/SkySphere.3ds", MODEL_LOAD_FORMAT_TRIANGLES | MODEL_LOAD_FORMAT_BGRA, GraphicsModule::OBJInstance::getModelMatrix({ 10,10,10 },{ 0,0,0 },{ 0,0,0 }) /*MATRIX(10,0,0,0, 0,10,0,0, 0,0,10,0, 0,0,0,1)*/, GraphicsModule::eDIMENSION::TEX_CUBE);
#if defined(DX11)
	LoadMesh("Models/Models/SAQ.obj", MODEL_LOAD_FORMAT_TRIANGLES | MODEL_LOAD_FORMAT_BGRA, GraphicsModule::OBJInstance::getModelMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 }), GraphicsModule::eDIMENSION::TEXTURE2D);
#elif defined(OGL)
	LoadMesh("Models/Models/SAQ_OGL.obj", MODEL_LOAD_FORMAT_TRIANGLES | MODEL_LOAD_FORMAT_BGRA, GraphicsModule::OBJInstance::getModelMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 }), GraphicsModule::eDIMENSION::TEXTURE2D);
#endif


#if defined(DX11)
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("GBuffer", &g_ObjInstances[0], true);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("GBuffer", &g_ObjInstances[1], true);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("SkyBox", &g_ObjInstances[g_ObjInstances.size() - 2], true);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("Lights", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("SSAO", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("ToneMap", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("Copy", &g_ObjInstances[g_ObjInstances.size() - 1], false);

	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("Lights", &g_ObjInstances[0], true);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("Lights", &g_ObjInstances[1], true);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("SkyBox", &g_ObjInstances[g_ObjInstances.size() - 2], true);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("ToneMap", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("Copy", &g_ObjInstances[g_ObjInstances.size() - 1], false);/**/
#elif defined(OGL)
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("GBuffer", &g_ObjInstances[0], true);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("GBuffer", &g_ObjInstances[1], true);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("SkyBox", &g_ObjInstances[g_ObjInstances.size() - 2], true);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("Lights", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("SSAO", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("ToneMap", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Deferred").AddObjectToPass("Copy", &g_ObjInstances[g_ObjInstances.size() - 1], false);/**/

	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("Lights", &g_ObjInstances[0], true);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("Lights", &g_ObjInstances[1], true);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("SkyBox", &g_ObjInstances[g_ObjInstances.size() - 2], true);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("ToneMap", &g_ObjInstances[g_ObjInstances.size() - 1], false);
	g_Test.GetRenderManager()->getShader("Forward").AddObjectToPass("Copy", &g_ObjInstances[g_ObjInstances.size() - 1], false);/**/
#endif


	return S_OK;
}

//void LoadMesh(const aiScene* scene, string fileName, unsigned int Flags, MATRIX mat, GraphicsModule::eDIMENSION dim)
void LoadMesh(string fileName, unsigned int Flags, MATRIX mat, GraphicsModule::eDIMENSION dim)
{    
    g_ObjInstances.push_back(GraphicsModule::OBJInstance());
    size_t lastOBJ = g_ObjInstances.size() - 1;

	//g_ObjInstances[lastOBJ].LoadModel(scene, fileName, Flags, mat, dim);
	g_ObjInstances[lastOBJ].LoadModel(fileName, Flags, mat, dim);
	for (int i = 0; i < lastOBJ; i++)
	{
		if (g_ObjInstances[i].m_OBJModel.m_name == g_ObjInstances[lastOBJ].m_OBJModel.m_name)
		{
			g_ObjInstances.erase(g_ObjInstances.end() - 1);
			cout << "Model ya cargado" << endl;
			return;
		}
	}

	g_ObjInstances[lastOBJ].setPosition({ 0,0,0 });
	g_ObjInstances[lastOBJ].setRotation({ 0,0,0 });
	g_ObjInstances[lastOBJ].setSize({ 1,1,1 });
}

/*void OpenMesh(string fileName, unsigned int Flags, MATRIX mat, GraphicsModule::eDIMENSION dim)
{
    //string fileName = OpenFileGetName();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    if (!scene)
    {
        cout << importer.GetErrorString() << endl;
        return;
    }

	LoadMesh(scene, fileName, Flags, mat, dim);
}*/

bool TryLoadMesh(string fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
	if (!scene)
		return false;
	return true;
}

void UIRender()
{
#if defined(DX11) || defined(OGL)
    
#if defined(DX11)
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();	
#elif defined(OGL)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif

    ImGui::NewFrame();
	static string fileName;

	if (!g_SelectingLoadMode)
	{
		if (ImGui::Begin("RenderTargets"))
		{
			std::vector<GraphicsModule::RenderTargetStruct>* rtvs;
			rtvs = g_Test.GetRenderManager()->GetRenderTargets();
			for (int i = 0; i < rtvs->size(); i++)
			{
				ImGui::Text((*rtvs)[i].m_names[0].c_str());
#if defined(DX11)
				ImGui::ImageButton((void*)(*rtvs)[i].m_tex.getBuffer().getPtr(), ImVec2(1920 / 4, 1080 / 4));
#elif defined(OGL)
				ImGui::ImageButton((void*)(*rtvs)[i].m_tex.getID(), ImVec2(1920 / 4, 1080 / 4));
#endif           
			}
		}
		ImGui::End();
		if (ImGui::Begin("Lights"))
		{
			/*Ambient light*/
			ImGui::Text("Ambient light:");
			ImGui::Separator();

			ImGui::PushID("ambient");

			static float ambient = .1;
			ImGui::DragFloat("Ambient", &ambient, 0.001f, 0.0f, 1.0f);

			static float specular = .5;
			ImGui::DragFloat("Specular", &specular, 0.001f, 0.0f, 1.0f);

			static float diffuse = 1.0f;
			ImGui::DragFloat("Diffuse", &diffuse, 0.001f, 0.0f, 1.0f);

			static float shininess = 32;
			ImGui::DragFloat("Shininnes", &shininess, 0.01f, 0.0f, 200.0f);

			Material mat;

			mat.ambient = ambient;
			mat.specular = specular;
			mat.diffuse = diffuse;
			mat.shininess = shininess;

			g_Test.GetRenderManager()->UpdateMaterial(mat);

			ImGui::PopID();
			ImGui::Separator();

			/*Directional Light*/
			ImGui::Text("Directional Light");
			ImGui::Separator();

			ImGui::PushID("directional");

			/*static float dir[3]{ 0.0f, 0.5f, -1.0f };
			ImGui::DragFloat3("Light Direction", dir, 0.001f, -1.0f, 1.0f);

			static float specular = .5;
			ImGui::DragFloat("Specular strenght", &specular, 0.001f, 0.0f, 1.0f);

			static float scatering = 50;
			ImGui::DragFloat("Scatering", &scatering, 0.5f, 1.0f, 256.0f);
			g_Test.SetLightScatering(scatering);

			ImGui::PopID();
			ImGui::Separator();

			DirectionalLight dirDesc;
			dirDesc.lightDir = Vector4{ dir[0], dir[1], dir[2], 0 };
			dirDesc.ambient = Vector4{ambient, ambient, ambient, 1};
			dirDesc.diffuse = Vector4{1,1,1,1};
			dirDesc.specular = Vector4{ specular, specular, specular, 1 };/**/

			static float dir[3]{ 0.0f, -1.0f, 0.0f };
			ImGui::DragFloat3("Light Direction", dir, 0.001f, -1.0f, 1.0f);

			static float color[3]{ 1.0f, 1.0f, 1.0f };
			ImGui::DragFloat3("Light Color", color, 0.001f, 0.0f, 1.0f);

			ImGui::PopID();
			ImGui::Separator();

			DirectionalLight dirDesc;
			dirDesc.lightDir = Vector4{ dir[0], dir[1], dir[2], 0 };
			dirDesc.color = Vector4{ color[0], color[1], color[2], 1 };

			g_Test.GetRenderManager()->UpdateDirectionalLight(dirDesc);

			/*Point Light*/
			ImGui::Text("Point Light");
			ImGui::Separator();

			ImGui::PushID("point");

			/*static float pointPos[3]{ 0.0f, 0.0f, 10.0f };
			ImGui::DragFloat3("Light Position", pointPos, 0.5f, -20.0f, 20.0f);

			static float pointDiff = 1;
			ImGui::DragFloat("Intensity", &pointDiff, 0.01f, 0.0f, 10.0f);

			static float pointSpecular = .5;
			ImGui::DragFloat("Specular strength", &pointSpecular, 0.001f, 0.0f, 1.0f);

			static float pointBlurDist = 20;
			ImGui::DragFloat("Blur Distance", &pointBlurDist, 0.5f, 1.0f, 256.0f);

			ImGui::PopID();
			ImGui::Separator();

			PointLight pointDesc;
			pointDesc.lightPos = Vector4{ pointPos[0], pointPos[1], pointPos[2], 0 };
			pointDesc.diffuse = Vector4{ pointDiff, pointDiff, pointDiff, 1 };
			pointDesc.specular = Vector4{ pointSpecular, pointSpecular, pointSpecular, 1 };
			pointDesc.blurDistance = pointBlurDist;*/

			static float pointPos[3]{ 0.0f, 0.0f, 10.0f };
			ImGui::DragFloat3("Light Position", pointPos, 0.5f, -20.0f, 20.0f);

			static float pointColor[3]{ 1.0f, 0.0f, 0.0f };
			ImGui::DragFloat3("Light Color", pointColor, 0.5f, 0.0f, 1.0f);

			static float pointAtt = 20;
			ImGui::DragFloat("Light Attenuation", &pointAtt, 0.001f, 0.0f, 100.0f);

			ImGui::PopID();
			ImGui::Separator();

			PointLight pointDesc;
			pointDesc.point_lightPos = Vector4{ pointPos[0], pointPos[1], pointPos[2], 0 };
			pointDesc.point_lightColor = Vector4{ pointColor[0], pointColor[1], pointColor[2], 1 };
			pointDesc.point_lightAtt = pointAtt;

			g_Test.GetRenderManager()->UpdatePointLight(pointDesc);

			/*Spot Light*/
			ImGui::Text("Spot Light");
			ImGui::Separator();

			ImGui::PushID("spot");

			/*static float spotDiff[3]{ 1.0f, 1.0f, 1.0f };
			ImGui::DragFloat3("Intensity", spotDiff, 0.01f, 0.0f, 10.0f);

			static float spotSpecular = .5;
			ImGui::DragFloat("Specular strength", &spotSpecular, 0.001f, 0.0f, 1.0f);

			static float cutOff = 40;
			static float outerCutOff = 45;
			ImGui::DragFloat("Cut Off", &cutOff, 1.0f, 10.0f, outerCutOff);

			ImGui::DragFloat("Outer Cut Off", &outerCutOff, 1.0f, cutOff, 80.0f);

			static float spotBlurDist = 100;
			ImGui::DragFloat("Blur Distance", &spotBlurDist, 0.5f, 1.0f, 256.0f);

			ImGui::PopID();

			SpotLight spotDesc;
			GraphicsModule::Vector sportDir = g_Cameras[g_activeCamera].GetFrontVector();
			GraphicsModule::Vector sportPos = g_Cameras[g_activeCamera].getEyePos();

			spotDesc.lightPos = Vector4{ sportPos.x(), sportPos.y(), sportPos.z(), 1.0f };
			spotDesc.lightDir = Vector4{ sportDir.x(), sportDir.y(), sportDir.z(), 1.0f };
			spotDesc.cutOff = cutOff;
			spotDesc.outerCutOff = outerCutOff;
			spotDesc.diffuse = Vector4{ spotDiff[0], spotDiff[1], spotDiff[2], 1 };
			spotDesc.specular = Vector4{ spotSpecular , spotSpecular , spotSpecular , 1};
			spotDesc.blurDistance = spotBlurDist;/**/

			static float spotColor[3]{ 0.0f, 1.0f, 0.0f };
			ImGui::DragFloat3("Light Color", spotColor, 0.01f, 0.0f, 1.0f);

			static float spotAtt = .5;
			ImGui::DragFloat("Light Attenuation", &spotAtt, 0.001f, 0.0f, 1.0f);

			static float cutOff = 40;
			static float outerCutOff = 45;
			ImGui::DragFloat("Cut Off", &cutOff, 1.0f, 10.0f, outerCutOff);

			ImGui::DragFloat("Outer Cut Off", &outerCutOff, 1.0f, cutOff, 80.0f);

			ImGui::PopID();

			SpotLight spotDesc;
			GraphicsModule::Vector sportDir = g_Cameras[g_activeCamera].GetFrontVector();
			GraphicsModule::Vector sportPos = g_Cameras[g_activeCamera].getEyePos();

			spotDesc.lightPos = Vector4{ sportPos.x(), sportPos.y(), sportPos.z(), 1.0f };
			spotDesc.lightDir = Vector4{ sportDir.x(), sportDir.y(), sportDir.z(), 1.0f };
			spotDesc.lightColor = Vector4{ spotColor[0], spotColor[1], spotColor[2], 1 };
			spotDesc.lightAtt = spotAtt;
			spotDesc.cutOff = cutOff;
			spotDesc.outerCutOff = outerCutOff;

			g_Test.GetRenderManager()->UpdateSpotLight(spotDesc);
		}
		ImGui::End();
		if (ImGui::Begin("Buffers"))
		{
			/*Tone Correction*/

			ImGui::Text("Tone Correction:");
			ImGui::Separator();

			ImGui::PushID("exposure");

			static Vector4 exposure = Vector4{0.6f,0.0f,0.0f,0.0f};
			ImGui::DragFloat("Exposure", &exposure.x, 0.001f, 0.0f, 10.0f);

			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("ToneMap", "Exposure", &exposure);
			g_Test.GetRenderManager()->getShader("Forward").SetPassValue("ToneMap", "Exposure", &exposure);


			ImGui::PopID();

			ImGui::Separator();


			/*SSAO*/

			ImGui::Text("SSAO:");
			ImGui::Separator();

			ImGui::PushID("sample radious");

			static float sampleRadious = .34f;
			ImGui::DragFloat("Sample Radious", &sampleRadious, 0.001f, 0.0f, 1.0f);

			static float scale = .01f;
			ImGui::DragFloat("Scale", &scale, 0.001f, 0.0f, 1.0f);

			static float bias = .23f;
			ImGui::DragFloat("Bias", &bias, 0.001f, 0.0f, 1.0f);

			static float intensity = 4.0f;
			ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 50.0f);

			static int sampleInteractions = 5;
			ImGui::DragInt("Sample Iterations", &sampleInteractions, 1.0f, 0.0f, 100.0f);

			SSAO ssao;
			ssao.sampleRadius = sampleRadious;
			ssao.scale = scale;
			ssao.bias = bias;
			ssao.intensity = intensity;
			ssao.sampleIterations = sampleInteractions;

#if defined(DX11)
			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("SSAO", "SSAO", &ssao);
#elif defined(OGL)
			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("SSAO", "SSAO_rad", &ssao.sampleRadius);
			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("SSAO", "SSAO_scale", &ssao.scale);
			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("SSAO", "SSAO_bias", &ssao.bias);
			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("SSAO", "SSAO_intens", &ssao.intensity);
			g_Test.GetRenderManager()->getShader("Deferred").SetPassValue("SSAO", "SSAO_it", &ssao.sampleIterations);/**/
#endif
			ImGui::PopID();

			ImGui::Separator();
		}
		ImGui::End();
		if (ImGui::Begin("Defines"))
		{		
/*			static int lightTypeIndex = 0;
			const char* lightTypes[] = { "None", "Vertex light", "Pixel light" };
			ImGui::Combo("Light Type", &lightTypeIndex, lightTypes, IM_ARRAYSIZE(lightTypes));

			static int specularTypeIndex = 0;
			const char* specularTypes[] = { "None", "PHONG", "BLINN PHONG" };
			ImGui::Combo("Specular Type", &specularTypeIndex, specularTypes, IM_ARRAYSIZE(specularTypes));

			static bool diffuseMap = false;
			static bool normalMap = false;
			static bool specularMap = false;
			
			ImGui::Checkbox("Diffuse map", &diffuseMap);
			ImGui::Checkbox("Normal map", &normalMap);
			ImGui::Checkbox("Specular map", &specularMap);

			static int toneMapTypeIndex = 0;
			const char* toneMapType[] = { "None", "BASIC", "REINHARD", "BURGESS DAWSON", "UNCHARTED2" };
			ImGui::Combo("Tone Correction Type", &toneMapTypeIndex, toneMapType, IM_ARRAYSIZE(toneMapType));

			eNORMAL_TECHNIQUES normalTech = eNORMAL_TECHNIQUES::NONE;
			eSPECULAR_TECHNIQUES specularTech = eSPECULAR_TECHNIQUES::NONE;
			unsigned int mapFlags = 0;
			eTONE_CORRECTION_TECHNIQUES toneMapTech = eTONE_CORRECTION_TECHNIQUES::NONE;

			if (lightTypeIndex == 1)
			{
				normalTech =  eNORMAL_TECHNIQUES::VERTEX_SHADER;
			}
			else if (lightTypeIndex == 2)
			{
				normalTech = eNORMAL_TECHNIQUES::PIXEL_SHADER;
			}

			if (specularTypeIndex == 1)
			{
				specularTech = eSPECULAR_TECHNIQUES::PHONG;
			}
			else if (specularTypeIndex == 2)
			{
				specularTech = eSPECULAR_TECHNIQUES::BLINN_PHONG;
			}

			if (diffuseMap)
			{
				mapFlags |= TEXTURE_MAP_DIFFUSE;
			}
			if (normalMap)
			{
				mapFlags |= TEXTURE_MAP_NORMAL;				
			}
			if (specularMap)
			{
				mapFlags |= TEXTURE_MAP_SPECULAR;				
			}

			if (toneMapTypeIndex == 1)
			{
				toneMapTech = eTONE_CORRECTION_TECHNIQUES::BASIC;
			}
			else if (toneMapTypeIndex == 2)
			{
				toneMapTech = eTONE_CORRECTION_TECHNIQUES::REINHARD;
			}
			else if (toneMapTypeIndex == 3)
			{
				toneMapTech = eTONE_CORRECTION_TECHNIQUES::BURGESS_DAWSON;
			}
			else if (toneMapTypeIndex == 4)
			{
				toneMapTech = eTONE_CORRECTION_TECHNIQUES::UNCHARTED2;
			}

			g_Test.GetRenderManager()->SetShaderFlags(normalTech, specularTech, mapFlags, toneMapTech);*/


			static int techniqueIndex = 0;
			const char* techniques[] = { "Deferred", "Forward" };
			ImGui::Combo("Technique", &techniqueIndex, techniques, IM_ARRAYSIZE(techniques));

			if (techniqueIndex == 0)
			{
				g_Test.SetTechnique("Deferred");
			}
			else if (techniqueIndex == 1)
			{
				g_Test.SetTechnique("Forward");
			}
		}
		ImGui::End();
		if (ImGui::Begin("Models"))
		{
		    if (ImGui::Button("Open Mesh", ImVec2(100, 30)))
			{
				fileName = OpenFileGetName();
				g_SelectingLoadMode = TryLoadMesh(fileName);
		    }
		    if (ImGui::CollapsingHeader("Meshes"))
			{
				size_t countOBJs = g_ObjInstances.size();
				for (int i = 0; i < countOBJs; i++)
				{
					ImGui::Text("    "); ImGui::SameLine();
		            ImGui::PushID(i);
				    if (ImGui::CollapsingHeader(g_ObjInstances[i].getName().c_str()))
				    {
						ImGui::Text("        "); ImGui::SameLine();
				    	if (ImGui::CollapsingHeader("Transform"))
						{
							ImGui::Text("            "); ImGui::SameLine();
				    		float pos[3]{ g_ObjInstances[i].getPosition().x(), g_ObjInstances[i].getPosition().y(), g_ObjInstances[i].getPosition().z() };
				    		if (ImGui::DragFloat3("Position", pos, 0.01f))
				    		{
				    			g_ObjInstances[i].setPosition({ pos[0], pos[1], pos[2] });
							}
							ImGui::Text("            "); ImGui::SameLine();
							float rot[3]{ g_ObjInstances[i].getRotation().x() * (180 / 3.1415f), g_ObjInstances[i].getRotation().y() * (180 / 3.1415f), g_ObjInstances[i].getRotation().z() * (180 / 3.1415f) };
				    		if (ImGui::DragFloat3("Rotation", rot, 0.1f, 360, -360))
				    		{
				    			g_ObjInstances[i].setRotation({ rot[0] * (3.1415f / 180), rot[1] * (3.1415f / 180), rot[2] * (3.1415f / 180) });
							}
							ImGui::Text("            "); ImGui::SameLine();
				    		float scale[3]{ g_ObjInstances[i].getSize().x(), g_ObjInstances[i].getSize().y(), g_ObjInstances[i].getSize().z() };
				    		if (ImGui::DragFloat3("Scale", scale, 0.01f))
				    		{
				    			g_ObjInstances[i].setSize({ scale[0], scale[1], scale[2] });
				    		}
						}
						ImGui::Text("        "); ImGui::SameLine();
				    	if (ImGui::CollapsingHeader("Info"))
						{
							ImGui::Text("            "); ImGui::SameLine();
							ImGui::Text(g_ObjInstances[i].getName().c_str());
							ImGui::Text("            "); ImGui::SameLine();
							if (ImGui::CollapsingHeader("Textures"))
							{
								ImGui::Text("               "); ImGui::SameLine();
		                        for (int j = 0; j < g_ObjInstances[i].getMeshCount(); ++j)
		                        {	
									for (int k = 0; k < g_ObjInstances[i].getTextureCount(j); ++k)
									{
										float my_tex_w = 256;
										float my_tex_h = 256;
										ImTextureID my_tex_id = ImTextureID();
#if defined(DX11)
										my_tex_id = g_ObjInstances[i].getTexture(j, k).getBuffer().getPtr();
#elif defined(OGL)
										my_tex_id = (void*)g_ObjInstances[i].getTexture(j, k).getID();
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
		            }
		            ImGui::PopID();
		        }
		    }	
		}
		ImGui::End();
	}
	else
	{
		if (ImGui::Begin("Load Configuration", nullptr))
		{
			static int colorFormatItem = 0;
			static int modelFormatItem = 0;
			static float pos[3]{};
			static float rot[3]{};
			static float scale[3]{1.0f, 1.0f, 1.0f};
			if (ImGui::CollapsingHeader("Modes"))
			{
				const char* colorFormatItems[] = { "RGBA", "BGRA" };
				ImGui::Text("    "); ImGui::SameLine();
				ImGui::Combo("Color Format", &colorFormatItem, colorFormatItems, IM_ARRAYSIZE(colorFormatItems));
				const char* modelFormatItems[] = { "Triangles", "Wireframe", "Points" };
				ImGui::Text("    "); ImGui::SameLine();
				ImGui::Combo("Model Format", &modelFormatItem, modelFormatItems, IM_ARRAYSIZE(modelFormatItems));
			}
			if (ImGui::CollapsingHeader("Transforms"))
			{
				ImGui::Text("    "); ImGui::SameLine();
				if (ImGui::DragFloat3("Position", pos, 0.01f)){}
				ImGui::Text("    "); ImGui::SameLine();
				if (ImGui::DragFloat3("Rotation", rot, 0.1f, 360, -360)){}
				ImGui::Text("    "); ImGui::SameLine();
				if (ImGui::DragFloat3("Scale", scale, 0.01f)){}


			}
			if (ImGui::Button("Load", ImVec2(100, 30)))
			{
				unsigned int Flags = 0;
				if (colorFormatItem == 0)
				{
					Flags |= MODEL_LOAD_FORMAT_RGBA;
				}
				else if (colorFormatItem == 1)
				{
					Flags |= MODEL_LOAD_FORMAT_BGRA;
				}

				if (modelFormatItem == 0)
				{
					Flags |= MODEL_LOAD_FORMAT_TRIANGLES;
				}
				else if (modelFormatItem == 1)
				{
					Flags |= MODEL_LOAD_FORMAT_WIREFRAME;
				}
				else if (modelFormatItem == 2)
				{
					Flags |= MODEL_LOAD_FORMAT_POINTS;
				}
				float piOver180 = 3.1415 / 180;

				auto mat = GraphicsModule::OBJInstance::getModelMatrix(GraphicsModule::Vector(scale[0], scale[1], scale[2]), GraphicsModule::Vector(pos[0], pos[1], pos[2]), GraphicsModule::Vector(rot[0] * piOver180, rot[1] * piOver180, rot[2] * piOver180));
				//OpenMesh(fileName, Flags, mat, GraphicsModule::eDIMENSION::TEXTURE2D);
				LoadMesh(fileName, Flags, mat, GraphicsModule::eDIMENSION::TEXTURE2D);

				std::vector<GraphicsModule::OBJInstance*> models;
				for (int i = 0; i < g_ObjInstances.size() - 2; i++)
				{
					models.push_back(&g_ObjInstances[i]);
				}
				g_Test.GetRenderManager()->getShader("Deferred").ResetObjectsOfPass("GBuffer", models);
				g_Test.GetRenderManager()->getShader("Forward").ResetObjectsOfPass("Lights", models);

				g_SelectingLoadMode = false;
			}
		}
		ImGui::End();
	}/**/

    //ImGui::ShowDemoWindow();

    // render UI
    ImGui::Render();

#if defined(DX11)
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif defined(OGL)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

#endif
}

void Update(float dt)
{
    g_Test.Update(dt);


    /*Update the active camera*/
    g_Cameras[g_activeCamera].Update();

	GraphicsModule::Vector dir = g_Cameras[g_activeCamera].GetFrontVector();
	GraphicsModule::Vector pos = g_Cameras[g_activeCamera].getEyePos();
	g_Test.SetViewPosAndDir(Vector4{ pos.x(), pos.y(), pos.z(), 0.0f }, Vector4{ dir.x(), dir.y(), dir.z(), 0.0f });


    /*Set the new view and projection matrices*/
    g_Test.GetRenderManager()->UpdateViewMatrix(g_Cameras[g_activeCamera].getViewMatrix());
    g_Test.GetRenderManager()->UpdateProjectionMatrix(g_Cameras[g_activeCamera].getProjectionMatrix());

	//g_Test.GetRenderManager()->ShaderSetFloat("mat1.specular", 1);


	for (int i = 0; i < g_ObjInstances.size(); i++)
	{
		g_ObjInstances[i].Update(dt);
	}
}

void Render()
{
	g_Test.Clear();

	size_t countOBJs = g_ObjInstances.size();
	for (int i = 0; i < countOBJs/**//*(countOBJs == 0 ? 0 : 1)/**/; i++)
	{
        //g_ObjInstances[i].Draw(GraphicsModule::GetManager());
    }

	UIRender();

	g_Test.Display();
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
#if !defined(OGL)
        DestroyWindow(GraphicsModule::GetManager()->GetWindow());
#else
		glfwDestroyWindow(GraphicsModule::GetManager()->GetWindow());
        glfwTerminate();
#endif
        return 0;
	}

	g_Test = GraphicsModule::GetTestObj();

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
		DestroyWindow(GraphicsModule::GetManager()->GetWindow());
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
#elif defined(OGL)
		glfwDestroyWindow(GraphicsModule::GetManager()->GetWindow());
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
			Update(.000833f);
			//start = high_resolution_clock::now();
            Render();
        }
    }
#else
	while (!glfwWindowShouldClose(GraphicsModule::GetManager()->GetWindow()))
	{
		processInputs(GraphicsModule::GetManager()->GetWindow());
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
	DestroyWindow(GraphicsModule::GetManager()->GetWindow());
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
#elif defined(OGL)
	glfwDestroyWindow(GraphicsModule::GetManager()->GetWindow());
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
#endif
    ImGui::DestroyContext();
    g_Test.CleanupDevice();
    return (int)msg.wParam;
}