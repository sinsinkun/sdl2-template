# SDL2 Project Template

Starter template for SDL2 project

API Reference: https://wiki.libsdl.org/SDL2/FrontPage

## Installation (with MSYS2)
- Go through setup process to get MinGW compiler working with VSCode (https://code.visualstudio.com/docs/languages/cpp)
- Search for WSL2 package in MinGW repository `pacman -Ss wsl2`
- Copy package name that matches g++ version (default is ucrt64)
- Install package with `pacman -S {...}`
- (FYI: `-lmingw32 -lSDL2main -lSDL2` flags were added to tasks.json manually)
- Hit run/debug button in VSCode, or F5 to compile and run