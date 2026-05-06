@REM Clean
RMDIR /S /Q ".\build\"

@REM Install
cmake -S "." -B "build"

@REM Build Release
cmake --build build --config Release

@REM Test
.\build\desktop_win64\Release\desktop_win64.exe
