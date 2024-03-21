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
  - [x] C17
  - [x] Standard libraries
  - [x] Raylib5
- Targets:
  - [x] Win64 (Windows)
  - [ ] JS (Javascript)
  - [ ] WASM (WebAssembly)
- Graphics
  - :hourglass: Render: 
      [x] PostProcessing
      [x] Texture tiling
  - :hourglass: Camera: 
      :hourglass: ThirdPerson
      [ ] FOV
  - :hourglass: Lighting:
      :hourglass: Real Time
      [ ] Shadows
  - :hourglass: Meshes: 
      [x] Meshes
      [x] Textures
      [x] Shaders
      [x] Materials
      [ ] LOD
  - [x] Sky:
      [x] Color
      [x] Cubemap
      :hourglass: Fog
- World
  - [ ] Physics
      [ ] hitbox
      [ ] gravity
  - [ ] Simulation
      [ ] LevelTick
- Scripting
  - :hourglass: Properties: 
      [x] code 
      [ ] file
      [ ] save
  - :hourglass: Console
      [x] output
      [ ] input    
- Animation
  - [x] Character
- Audio: 
  - [ ] Sound:
      [ ] Wave
      [ ] Spatial
  - [ ] Music:
      [ ] Player
      [ ] Wave
      [ ] Mp3
- Input
  - [ ] UI: 
      [ ] main menu
      [ ] quit menu
      [ ] i18n
  - :hourglass: Control:
      :hourglass: Keyboard
      [ ] Gamepad
      :hourglass: Tank control
      [ ] 3D control
- Other
  - [x] launching screen

Repository layout:
```
├── assets
│   ├── i18n
│   └── images
│   └── ...
├── docs
├── infra
├── lib
│   ├── raylib-5.0_win64_msvc16
│   ├── winmm-10-win64
├── platform
│   ├── engine
│   └── game
├── standalone
│   ├── desktop_win64
└── tools
│   └── blender
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

## Assets Pipeline

### Installation

1. Download [io_scene_m3d.py](./tools/blender/io_scene_m3d.py)
1. Start Blender
1. Go to menu, "Edit" > "Preferences..." > "Add-ons" tab > "Install..." button
1. Browse and double click on the downloaded file
1. On the "Add-ons" tab, search for "m3d"
1. Click on "Enable Add-on."
1. Now in the menu, "File" > "Export" > "Model 3D (.m3d)" (and eventually "File" > "Import" > "Model 3D (.m3d)") should appear

### 3D Models

1. Start WebBrowser...
1. https://www.mixamo.com
1. Click Characters 
1. search "X bot"
1. Click Download
1. Check Format: fbx, pose: t-pose
1. Click Download
1. Start Blender...
1. Go to menu, File > New > General ...Delete Cube
1. Go to menu, File > Import > FBX > "X bot.fbx"
1. Click on the "Texture Paint" tab > click "new" > type "X_bot_diffuse"
1. On right side panel > click "material" > "base color" > "image texture" > select "X_bot_diffuse"
1. Go to menu, File > Export > M3D > scale: 2.0 > "X_bot.m3d" > Export

### 3D Animations

1. Start WebBrowser...
1. https://www.mixamo.com
1. Click Animations 
1. search "Slow Run"
1. check "In Place" box
1. Click Download
1. Check Format: fbx, Skin: with skin
1. Click Download
1. Start Blender...
1. Go to menu, File > New > General ...Delete Cube
1. Go to menu, File > Import > FBX > "Slow Run.fbx"
1. Click on the "Animation" tab > click "editor type icon" > "non linear animation" (new channel editor)
1. click "push down action" (new channel)
1. Go to menu, File > Export > M3D > scale: 2.0 > "Slow Run.m3d" > Export

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
- Model3D Format
  -[Welcome to Model 3D!](https://bztsrc.gitlab.io/model3d/)
  -[Model 3D Blender Integration](https://gitlab.com/bztsrc/model3d/tree/master/blender/)
  -[io_scene_m3d.py](https://gitlab.com/bztsrc/model3d/-/blob/master/blender/io_scene_m3d.py)

There are several games made with raylib to highlight:
- Hare136: https://play.google.com/store/apps/details?id=games.haremonic.slider
- SideStep Legends: https://store.steampowered.com/app/2077590/Sidestep_Legends/
- Pure Logic: https://store.steampowered.com/app/1861500/Pure_Logic/

There are also many small games (and tools) made with raylib on itchio: https://itch.io/c/380928/made-with-raylib
