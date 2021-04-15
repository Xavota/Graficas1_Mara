# Graficas1_Mara

Controles:
Movimiento de cámara:
- AD en X
- WS en Y
- QE en Z
- Clik derecho y mover el mouse para rotar la cámara.

- Tab para cambio de perspectiva


Requisitos:
- ImGui 1.8
    + Includes:
        * $(DEVLIBS)imgui-1.8\
        * $(DEVLIBS)imgui-1.8\backends\
- Free Image 3.18.0
    + Includes:
        * $(DEVLIBS)FreeImage\Source
    + Libs: 
        * $(DEVLIBS)FreeImage\Win32\Release
        * $(DEVLIBS)FreeImage\x64\Release
        * $(DEVLIBS)FreeImage\Win32\Debug
        * $(DEVLIBS)FreeImage\x64\Debug
- ASSIMP 5.0.1
    + Includes:
        * $(DEVLIBS)ASSIMP\include
    + Libs:
        * $(DEVLIBS)ASSIMP\lib\Release\x86
        * $(DEVLIBS)ASSIMP\lib\Release\x64
        * $(DEVLIBS)ASSIMP\lib\Debug\x86
        * $(DEVLIBS)ASSIMP\lib\Debug\x64
- GLFW 3.3.3
    + Includes:
        * $(DEVLIBS)OpenGL\GLFW\include
    + Libs:
        * $(DEVLIBS)OpenGL\Libs\Release\x86
        * $(DEVLIBS)OpenGL\Libs\Release\x64
        * $(DEVLIBS)OpenGL\Libs\Debug\x86
        * $(DEVLIBS)OpenGL\Libs\Debug\x64
- GLAD 3.3.3
    + Includes:
        * $(DEVLIBS)OpenGL\GLAD\include
- GLM 0.9.8.5
    + Includes:
        * $(DEVLIBS)OpenGL\GLM\include
