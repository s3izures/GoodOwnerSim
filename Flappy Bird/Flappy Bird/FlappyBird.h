#pragma once
#include "raylib.h"
#include <iostream>
using namespace std;

class FlappyBird
{
public:
	void Main();

private:
	void Start();
	void Update();
	void EvalFrame();
	void DrawFrame();

	void InitPipes();

	Vector2 screen = Vector2{ 800,450 };

	typedef struct Bird {

	}Bird;
};

