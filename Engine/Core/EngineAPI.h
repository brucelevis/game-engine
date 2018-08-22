#pragma once

#include "Display.h"
#include "Window.h"
#include "Input.h"
#include "GameTimer.h"

// The API we expose to the underlying game
// NOTE: the api should be all ptrs to the underlying engine
// Since we create a new struct everytime we load the game, we are sending a copy of this struct down
// Therefore this struct should be lightweight
typedef struct EngineAPI {
	Display* display;
	Window* window;
	Input* input;
	GameTimer* game_loop;
} EngineAPI;