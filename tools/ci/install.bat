@REM Clean
RMDIR /S /Q ".\build\"

@REM Install
cmake -S "." -B "build"

@REM Build
cmake --build build
