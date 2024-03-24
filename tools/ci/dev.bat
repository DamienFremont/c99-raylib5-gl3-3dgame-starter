@REM ReInstall ./assets/resources/* (if needed)
@REM cmake -S "." -B "build"

@REM Build
cmake --build build

@REM Run
.\build\desktop_win64\Debug\desktop_win64.exe
