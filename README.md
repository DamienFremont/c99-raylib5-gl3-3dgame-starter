:warning: WORK IN PROGRESS
==========================

# 3D Game Starter - C 2017, RayLib 5.0, OpenGL 3

2024's small template to start your raylib game. Featuring 3D, C 2017, Raylib 5.0, OpenGL 3 and a modular architecture.

<p align="center">
  <img src="./docs/README/thumb-raylib.png" height="175" width="auto" />
</p>

## Content

  - [Dependencies](#dependencies)
  - [Screenshots](#screenshots)
  - [Features](#features)
  - [Install](#install)
  - [Usage](#usage)
  - [Resources](#resources)

---------------------------------------

## Dependencies

- C++ 2020 / C 2017
- [RayLib 5.0](https://www.raylib.com/)
- OpenGL 3.30

## Screenshots

![alt text](./docs/README/screenshot-raylib.png)

![alt text](./docs/README/screenshot-unreal.jpg)

![alt text](./docs/README/diagrams-1.png)

## Features

- Stack
  - :heavy_check_mark: C++20
  - :heavy_check_mark: Standard libraries
  - :heavy_check_mark: Raylib
- Targets:
  - :heavy_check_mark: Win64 (Windows)
  - :x: JS (Javascript)
  - :x: WASM (WebAssembly)
- Graphics
  - :hourglass: Render: 
      :heavy_check_mark: PostProcessing
      :heavy_check_mark: Texture tiling
  - :hourglass: Camera: 
      :hourglass: ThirdPerson
      :x: FOV
  - :hourglass: Lighting:
      :hourglass: Real Time
      :x: Shadows
  - :hourglass: Meshes: 
      :heavy_check_mark: Meshes
      :heavy_check_mark: Textures
      :heavy_check_mark: Shaders
      :heavy_check_mark: Materials
      :x: LOD
  - :x: Visual Effects:
      :x: Particles
      :x: Distant Horizon
  - :x: Sky:
      :heavy_check_markx: Color
      :heavy_check_markx: Cubemap
- World
  - :x: Physics
      :x: hitbox
      :x: gravity
  - :x: Simulation
      :x: LevelTick
- Scripting
  - :hourglass: Properties: 
      :heavy_check_mark: code 
      :x: file
      :x: save
  - :hourglass: Console
      :heavy_check_mark: output
      :x: input    
- Animation
  - :heavy_check_mark: Character
- Audio: 
  - :x: Sound:
      :x: Wave
      :x: Spatial
  - :x: Music:
      :x: Player
      :x: Wave
      :x: Mp3
- Input
  - :x: UI: 
      :x: main menu
      :x: quit menu
      :x: i18n
  - :hourglass: Control:
      :hourglass: Keyboard
      :x: Gamepad
      :hourglass: Tank control
      :x: 3D control
- Other
  - :heavy_check_mark: launching screen

Repository layout:
```
├── assets
│   ├── i18n
│   └── images
│   └── ...
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
│   ├── desktop_wasm
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

## Usage

### CMake (CLI)

Install:

```powershell
# Install, Build
cmake -S "." -B "build"
cmake --build build
```

Dev:

```powershell
# Build & Run
cmake --build build | .\build\desktop_win64\Debug\desktop_win64.exe
```

```powershell
# ReInstall ./assets/resources/* (if needed)
cmake -S "." -B "build"
```

Release:

```powershell
# Clean, Install, Release
rm .\build\ -r -force
cmake -S "." -B "build"
cmake --build build --config Release

# Test
.\build\desktop_win64\Release\desktop_win64.exe
```

### VisualStudio 2022 (IDE)

Install:

```powershell
# Install
cmake -S "." -B "build"
```

Install:

![alt text](./docs/README/thumb-vs-open.png)

Dev:

![alt text](./docs/README/thumb-vs-debug.png)

```powershell
# ReInstall ./assets/resources/* (if needed)
cmake -S "." -B "build"
```

### VSCode

Install:

```powershell
# Install
cmake -S "." -B "build"
```

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

## Resources

- C
  - [ModernCppStarter](https://github.com/TheLartians/ModernCppStarter/tree/master)
- CMake
  - [CMake with include and source paths - basic setup](https://stackoverflow.com/questions/8304190/cmake-with-include-and-source-paths-basic-setup)
  - [CMake add_library](http://www.cunningplanning.com/post/cmake-add-library/)
  - [Specify the default startup project in a Visual Studio solution.](https://cmake.org/cmake/help/latest/prop_dir/VS_STARTUP_PROJECT.html)
- Monorepo
  - [How to structure a monorepo](https://lucapette.me/writing/how-to-structure-a-monorepo/?utm_source=atom_feed)
  - [A monorepo template that actually works](https://levelup.gitconnected.com/a-monorepo-template-that-actually-works-3efd87770fa5)
- Raylib
  - Examples
    - [GitHub](https://github.com/raysan5/raylib/blob/master/examples/)
    - [Demos](https://www.raylib.com/examples.html)
      - [shaders_postprocessing](https://www.raylib.com/examples/shaders/loader.html?name=shaders_postprocessing) 
      - [shaders_hybrid_render](https://www.raylib.com/examples/shaders/loader.html?name=shaders_hybrid_render)
      - [shaders_texture_tiling](https://www.raylib.com/examples/shaders/loader.html?name=shaders_texture_tiling)
      - [shaders_basic_lighting](https://www.raylib.com/examples/shaders/loader.html?name=shaders_basic_lighting)
    - [how to properly setup raylib app to talk to glsl shaders](https://sepi.prose.sh/2022-09-11-how-to-properly-use-shaders-in-raylib)

There are several games made with raylib to highlight:
- Hare136: https://play.google.com/store/apps/details?id=games.haremonic.slider
- SideStep Legends: https://store.steampowered.com/app/2077590/Sidestep_Legends/
- Pure Logic: https://store.steampowered.com/app/1861500/Pure_Logic/

There are also many small games (and tools) made with raylib on itchio: https://itch.io/c/380928/made-with-raylib
