@echo off

copy "..\_external\Assimp\bin\x64\assimp-vc143-mt.dll" "..\x64\Release\assimp-vc143-mt.dll"

xcopy "Models" "..\x64\Release\Models" /e /i /y
xcopy "Shaders" "..\x64\Release\Shaders" /e /i /y
