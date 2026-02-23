/* External headers for project

-------- MY VOXEL ENGINE -------- 

- 14088 lines of code

-- SIDE NOTES --
- I do use AI to accelerate learning.
- I also am learning throughout this lal since I started learning C++ and OpenGL.
  as of ~10/1/2025, as well as this project.
- Working on rough draft before pushing to Github.

-------- TO-DO LIST --------

WORKING ON:
- Gameplay/General UI.
- Upload Videos (hopefully proof that I'm not using AI only to do this project).
- Learning effective UI
- Make custom textures

TODO:
- REOGRANIZE FILES!!!!!
- Make gameplay UI.
- Make general UI (settings, escape, etc.).
- Make load screen/options etc..
- Make survival features.
- Lighting.
- Mobs.
- Day/Night cycle.
- Items/etc.
*/

#include<iostream>
#include<cmath>
#include<filesystem>
#include<unordered_map>
#include<utility>
#include <x86intrin.h> // or <intrin.h> on MSVC
#include <memory>
#include <algorithm>
	
namespace fs = std::filesystem;

// Included external libraries
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Created headers for project
#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Chunk.h"
#include"World.h"
#include"GUIManager.h"
#include"PhysicsEngine.h"
#include"Mob.h"
#include "ArmHUD.h"
#include "engine/Engine.h"
#include "game/PlayerInput.h"
#include "game/Game.h"

// Variables for window height and width
unsigned int width = 854;
unsigned int height = 480;

// Variable for render distance (in chunks)
const int renderDistRadius = 4;
const float FOV = 100.0f;

// TEST ENGINE
int main()
{
	Engine engine;
	Game game(engine);

	game.init();
}