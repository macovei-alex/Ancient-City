@echo off

copy "..\_external\Assimp\bin\x64\assimp-vc143-mt.dll" "..\x64\Debug\assimp-vc143-mt.dll"

REM set "source=..\_external\Assimp\bin\x64\"
REM set "dest=..\x64\Debug\"
REM set "files=assimp-vc143-mt.dll"
REM for %%f in (%files%) do (
    REM if not exist "%dest%%%f" (
        REM copy "%source%%%f" "%dest%%%f"
    REM )
REM )

xcopy "Models" "..\x64\Debug\Models" /e /i /y
xcopy "Shaders" "..\x64\Debug\Shaders" /e /i /y
