#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
using namespace std;

class GOS
{
public:
	void Main();

private:
	void Start();
	void Update();
	void EvalFrame();
	void DrawFrame();

	static const int screenY = 500;
	static const int screenX = 800;

	//feed pet, pickup box when mouse pressed on rect, shake food into bowl, pet approaches 10 seconds after last shake frame
	//wash pet, pickup sponge when mouse pressed on rect, rub pet or dirty spots (lower opacity on enter) maybe hitbox smaller than square?
	//pet pet, rub pet
	//bar decay, pet slowly gets dirty
	//buttons UI
	//pet wander state
};

