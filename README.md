:warning: WORK IN PROGRESS
==========================

# C++ 2020 - RayLib - Starter

<p align="center">
  <img src="./docs/README/thumb-raylib.png" height="175" width="auto" />
</p>

## Content

- [:warning: WORK IN PROGRESS](#warning-work-in-progress)
- [C++ 2020 - RayLib - Starter](#c-2020---raylib---starter)
  - [Content](#content)
  - [Features](#features)
  - [Dependencies](#dependencies)
  - [Screenshots](#screenshots)
  - [Install](#install)
    - [Windows](#windows)
  - [Usage](#usage)
    - [Build everything at once](#build-everything-at-once)
  - [Resources](#resources)

---------------------------------------

## Features

- Stack
  - :heavy_check_mark: C++20
  - :heavy_check_mark: Standard libraries
  - :heavy_check_mark: Raylib
- Targets:
  - :x: x86 Windows

## Dependencies

- [RayLib 4.0.0](https://www.raylib.com/)

## Screenshots

<p align="center">
  <img src="./docs/README/thumb-screenshot.png" height="175" width="auto" />
</p>

![alt text](./docs/README/diagrams-1.png)

Repository layout:
```
├── assets
│   ├── i18n
│   └── images
├── docs
├── infra
├── lib
│   ├── raylib-4.0.0-win64
│   ├── winmm-10-win64
├── platform
│   ├── core
│   └── desktop
├── standalone
│   ├── desktop_win64
└── tools
```

## Install

### Windows

CMake:
- Download the `v3.28.3` release of `CMake` at http://www.cmake.org/download/.
- Pick Windows (`Windows x64 Installer`).
- Run the installer
- When asked for, select `Add CMake to the system PATH for all users`.
- Run software installation...

Visual Studio:
- Download the `Community` version of `Visual Studio 2022` release of Visual Studio at https://visualstudio.microsoft.com/downloads/
- Run the installer...
- Run software `Visual Studio Installer`
- When asked for, click on `modify` under `Visual Studio 2002`
- Select `Desktop development with C++` (contains MVC package)
- Click on `modify`

PowerShell:
```powershell
cmake -S "." -B "build" -G "Visual Studio 17 2022"
```

VS Code:

.vscode/c_cpp_properties.json:
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/platform/core/include",
                "${workspaceFolder}/platform/desktop/include"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.22621.0",
            "compilerPath": "cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++20",
            "intelliSenseMode": "windows-msvc-x64"
        }
    ],
    "version": 4
}
```

## Usage

### Build everything at once

```powershell
# Build
cmake -S "." -B "build"
cmake --build build

# Run
cd .\build\desktop_win64\Debug\
.\desktop_win64.exe
```

## Resources

- Raylib
  - Examples
    - shaders_postprocessing
      - [demo](https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing)
      - [github](https://github.com/raysan5/raylib/blob/master/examples/shaders/shaders_postprocessing.c)
