#pragma comment(lib, "ComDlg32.lib")

#include <windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#if defined(DX11)
#include "imgui_impl_dx11.h"
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
    
    

    return S_OK;
}

void LoadMesh(const aiScene* scene, aiMesh** _mesh, unsigned int _numMeshes)
{
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		
        for (int j = 1; j < aiTextureType_UNKNOWN; ++j)
        {
            cout << scene->mMaterials[i]->GetTextureCount((aiTextureType)j) << endl;
        }
	}
    
    for (int i = 0; i < scene->mNumTextures; i++)
    {
        cout << scene->mTextures[i]->mFilename.C_Str() << endl;
    }

	HRESULT hr;
    unsigned int indexCount = 0;
	for (int i = 0; i < _numMeshes; ++i)
	{
        cout << i << ", " << _mesh[i]->mNumVertices << endl;
        indexCount += _mesh[i]->mNumVertices;
    }

    vector<Vertex> vertices;
    int k = 0;
	for (int i = 0; i < _numMeshes; ++i)
    {
        for (int j = 0; j < _mesh[i]->mNumVertices; ++j)
		{
            Vertex v;
            if (_mesh[i]->HasPositions())
            {
                v.Pos.x = _mesh[i]->mVertices[j].x;
                v.Pos.y = _mesh[i]->mVertices[j].y;
                v.Pos.z = _mesh[i]->mVertices[j].z;/**/
            }
			else
			    v.Pos = Vector3{};
			if (_mesh[i]->HasTextureCoords(0))
            {
				v.Tex.x = _mesh[i]->mTextureCoords[0][j].x;
				v.Tex.y = 1 - _mesh[i]->mTextureCoords[0][j].y;/**/
            }
            else
				v.Tex = Vector2{};
			if (_mesh[i]->HasNormals())
            {
				v.Normales.x = _mesh[i]->mNormals[j].x;
				v.Normales.y = _mesh[i]->mNormals[j].y;
				v.Normales.z = _mesh[i]->mNormals[j].z;/**/
            }
			else
				v.Normales = Vector3{};

            vertices.push_back(v);
            ++k;
        }
    }

	for (int i = 0; i < _numMeshes; ++i)
	{
		for (int j = 0; j < _mesh[i]->mNumVertices; ++j)
		{
            for (int l = 1; l < _mesh[i]->mMaterialIndex; ++l)
            {
			    if (_mesh[i]->HasTextureCoords(l))
			    {
			    	cout << "i: " << i << "j: " << j << "l: " << l << endl;
                    cout << _mesh[i]->mTextureCoords[l][j].x << ", " << _mesh[i]->mTextureCoords[l][j].y << ", " << _mesh[i]->mTextureCoords[l][j].z << endl;
			    }
            }
		}
	}

    g_Mesh.push_back(GraphicsModule::Mesh(g_Mesh.size()));

    hr = g_Mesh[g_Mesh.size() - 1].setVertex(vertices);
    //delete[] vertices;

    /*unsigned int indexCount2 = 0;

    for (int i = 0; i < _numMeshes; ++i)
    {
		for (int j = 0; j < _mesh[i]->mNumFaces; ++j)
		{
			indexCount2 += _mesh[i]->mFaces[j].mNumIndices;
		}
    }

    unsigned short* indices = new unsigned short[indexCount2];
    unsigned int l = 0;
	for (int i = 0; i < _numMeshes; ++i)
	{
		for (int j = 0; j < _mesh[i]->mNumFaces; ++j)
		{
            for (int k = 0; k < _mesh[i]->mFaces[j].mNumIndices; ++k)
            {
			    indices[l++] = _mesh[i]->mFaces[j].mIndices[k];
            }
		}
	}/**/

	vector<unsigned int> indices;
    size_t vertexCount = vertices.size();
    for (int i = 0; i < vertexCount; ++i)
    {
        indices.push_back(i);
    }/**/

    hr = g_Mesh[g_Mesh.size() - 1].setIndices(indices);
    indices.clear();/**/
    vertices.clear();

	/*unsigned short indexCount = 0;
	for (int i = 1; i < 2; ++i)
	{
		indexCount += _mesh[i]->mNumVertices;
	}
	Vertex* vertices = new Vertex[indexCount];
	for (int i = 1; i < 2; ++i)
	{
		for (int j = 0; j < _mesh[i]->mNumVertices; ++j)
		{
			if (_mesh[i]->HasPositions())
				vertices[j].Pos = *reinterpret_cast<Vector3*>(&_mesh[i]->mVertices[j]);
			else
				vertices[j].Pos = Vector3{};
			if (_mesh[i]->HasTextureCoords(0))
				vertices[j].Tex = Vector2{ _mesh[i]->mTextureCoords[0][j].x, _mesh[i]->mTextureCoords[0][j].y };
			else
				vertices[j].Tex = Vector2{};
			if (_mesh[i]->HasNormals())
				vertices[j].Normales = *reinterpret_cast<Vector3*>(&_mesh[i]->mNormals[j]);
			else
				vertices[j].Normales = Vector3{};
		}
	}

	hr = g_Mesh.setVertex(vertices, indexCount);

	unsigned short* indices = new unsigned short[indexCount];
	for (int i = 0; i < indexCount; ++i)
	{
		indices[i] = i;
	}

	hr = g_Mesh.setIndices(indices, indexCount);/**/


	/*hr = g_Mesh.setVertex(new Vertex[24]{
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
			{ Vector3{-1.0f, 1.0f, 1.0f},   Vector2{0.0f, 1.0f},    Vector3{0.0f, 0.0f, 1.0f}  } }, 24);*/

	//if (FAILED(hr))
		//return hr;




	// Create index buffer
	/*hr = g_Mesh.setIndices(new unsigned short[36]{
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
	);*/

	//if (FAILED(hr))
		//return hr;

	//g_ObjInstances = new GraphicsModule::OBJInstance[1];
    g_ObjInstances.push_back(GraphicsModule::OBJInstance());
    size_t lastOBJ = g_ObjInstances.size() - 1;

	g_ObjInstances[lastOBJ].setMesh(&g_Mesh[g_Mesh.size() - 1]);
	g_ObjInstances[lastOBJ].setPosition({ 0,0,0 });
	g_ObjInstances[lastOBJ].setRotation({ 0,0,0 });
	g_ObjInstances[lastOBJ].setSize({ 1,1,1 });

	TextureManager::CreateTextureFromFile("C:/Users/marad/OneDrive/Escritorio/Grafics/Backups 3parcial/3.- Assimp/Graficos1/bin/Models/base_albedo.jpg", "Texture");
	g_ObjInstances[lastOBJ].setTexture(TextureManager::GetTexture("Texture"));
}

void OpenMesh()
{
    string fileName = OpenFileGetName();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, NULL);
    if (!scene)
    {
        cout << importer.GetErrorString() << endl;
        return;
    }
    //cout << "Archivo " << fileName << " importado con exito" << endl;

	LoadMesh(scene, scene->mMeshes, scene->mNumMeshes);
    //aiReleaseImport(scene);
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
		ImGui::Text("Light Direction:");
		if (ImGui::DragFloat3("Dir", dir, 0.001f, -1.0f, 1.0f))
		{
			g_Test.SetDirLight(XMFLOAT4(dir[0], dir[1], dir[2], 0));
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
			    if (ImGui::CollapsingHeader(to_string(i).c_str()))
			    {
			    	if (ImGui::CollapsingHeader("Transform"))
			    	{
			    		float pos[3]{ g_ObjInstances[i].getPosition().x(), g_ObjInstances[i].getPosition().y(), g_ObjInstances[i].getPosition().z() };
			    		if (ImGui::DragFloat3(("Position" + to_string(i)).c_str(), pos, 0.01f))
			    		{
			    			g_ObjInstances[i].setPosition({ pos[0], pos[1], pos[2] });
						}
						float rot[3]{ g_ObjInstances[i].getRotation().x() * (180 / 3.1415f), g_ObjInstances[i].getRotation().y() * (180 / 3.1415f), g_ObjInstances[i].getRotation().z() * (180 / 3.1415f) };
			    		if (ImGui::DragFloat3(("Rotation" + to_string(i)).c_str(), rot, 0.1f, 360, -360))
			    		{
			    			g_ObjInstances[i].setRotation({ rot[0] * (3.1415f / 180), rot[1] * (3.1415f / 180), rot[2] * (3.1415f / 180) });
			    		}
			    		float scale[3]{ g_ObjInstances[i].getSize().x(), g_ObjInstances[i].getSize().y(), g_ObjInstances[i].getSize().z() };
			    		if (ImGui::DragFloat3(("Scale" + to_string(i)).c_str(), scale, 0.01f))
			    		{
			    			g_ObjInstances[i].setSize({ scale[0], scale[1], scale[2] });
			    		}
			    	}
			    	if (ImGui::CollapsingHeader("Info"))
			    	{
			    		float my_tex_w = 256;
			    		float my_tex_h = 256;
			    		ImTextureID my_tex_id = g_ObjInstances[i].getTexture().getPtr();
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
    //ImGui::ShowDemoWindow();
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
    //g_Cameras[g_activeCamera].Update();


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

    size_t countOBJs = g_ObjInstances.size();
    for (int i = 0; i < countOBJs; i++)
    {
        g_ObjInstances[i].getMesh()->setColor(g_vMeshColor);
    }

    static GraphicsModule::Vector rotation_c{ 0,0,0 };
    rotation_c.setVector(0, dtt, 3.14159265);

    //if (g_ObjInstances != nullptr)
        //g_ObjInstances[0].setRotation(rotation_c);
}

void Render()
{
	g_Test.Render();
	size_t countOBJs = g_ObjInstances.size();
	for (int i = 0; i < countOBJs; i++)
	{
	    GraphicsModule::GetManager()->DrawObject(&g_ObjInstances[i]);
    }
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