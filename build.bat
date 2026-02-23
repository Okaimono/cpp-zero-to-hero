@echo off
del bin\*.exe 2>nul
del src\*.o 2>nul

g++ -std=c++17 ^
    src/*.cpp ^
    src/engine/*.cpp ^
    src/game/*.cpp ^
    src/glad.c ^
    include\imgui/imgui.cpp ^
    include\imgui/imgui_draw.cpp ^
    include\imgui/imgui_tables.cpp ^
    include\imgui/imgui_widgets.cpp ^
    include\imgui/imgui_impl_glfw.cpp ^
    include\imgui/imgui_impl_opengl3.cpp ^
    -Iinclude ^
    -Iinclude/imgui ^
    -IHeaderFiles ^
    -Isrc ^
    -Llib -lglfw3 -lopengl32 -lgdi32 ^
    -o bin/game.exe

if %errorlevel% equ 0 (
    cls
    echo Build successful!
    bin\game.exe
) else (
    echo Build failed!
)