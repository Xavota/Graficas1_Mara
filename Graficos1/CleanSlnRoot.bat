:: Clean all build folders

SET RootPath=%~dp0

@RD /S /Q "%RootPath%.vs"

@RD /S /Q "%RootPath%bin\\Debug\\x86\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Debug\\x86\\Graficos 1.ilk"
@RD /S /Q "%RootPath%bin\\Debug\\x86\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Debug\\x64\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Debug\\x64\\Graficos 1.ilk"
@RD /S /Q "%RootPath%bin\\Debug\\x64\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x86\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x86\\Graficos 1.ilk"
@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x86\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x64\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x64\\Graficos 1.exp"
@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x64\\Graficos 1.ilk"
@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x64\\Graficos 1.lib"
@RD /S /Q "%RootPath%bin\\Debug_DirectX\\x64\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Debug_OpenGL\\x86\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Debug_OpenGL\\x86\\Graficos 1.ilk"
@RD /S /Q "%RootPath%bin\\Debug_OpenGL\\x86\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Debug_OpenGL\\x64\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Debug_OpenGL\\x64\\Graficos 1.ilk"
@RD /S /Q "%RootPath%bin\\Debug_OpenGL\\x64\\Graficos 1.pdb"



@RD /S /Q "%RootPath%bin\\Release\\x86\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Release\\x86\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Release\\x64\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Release\\x64\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Release_DirectX\\x86\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x86\\Graficos 1.iobj"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x86\\Graficos 1.ipdb"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x86\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Release_DirectX\\x64\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x64\\Graficos 1.exp"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x64\\Graficos 1.iobj"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x64\\Graficos 1.ipdb"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x64\\Graficos 1.lib"
@RD /S /Q "%RootPath%bin\\Release_DirectX\\x64\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Release_OpenGL\\x86\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Release_OpenGL\\x86\\Graficos 1.pdb"

@RD /S /Q "%RootPath%bin\\Release_OpenGL\\x64\\Graficos 1.exe"
@RD /S /Q "%RootPath%bin\\Release_OpenGL\\x64\\Graficos 1.pdb"

@RD /S /Q "%RootPath%Intermediate"
@RD /S /Q "%RootPath%lib"