@echo off

set "source=..\_external\Assimp\bin\x64\"
set "dest=..\x64\Debug\"
set "files=assimp-vc143-mt.dll"

for %%f in (%files%) do (
    if not exist "%dest%%%f" (
        copy "%source%%%f" "%dest%%%f"
    )
)
