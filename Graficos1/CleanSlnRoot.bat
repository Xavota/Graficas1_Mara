:: Clean all build folders

SET RootPath=%~dp0

@RD /S /Q "%RootPath%.vs"
@RD /S /Q "%RootPath%bin\\Debug"
@RD /S /Q "%RootPath%bin\\Release"
@RD /S /Q "%RootPath%bin\\Debug_DirectX"
@RD /S /Q "%RootPath%bin\\Release_DirectX"
@RD /S /Q "%RootPath%Intermediate"
@RD /S /Q "%RootPath%lib"