@REM Build (every 15 sec)
:start
cmake --build build
TIMEOUT /T 15
Goto Start
:EOF
