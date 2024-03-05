#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
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

	static const int screenX = 800;
	static const int screenY = 450;
	static const int gap = 250;

	typedef struct Bird {

	}Bird;

	typedef struct Pipe {
		float x;
		float width = 80;
		float heightTop, heightBot;
		float yBot;
		Color col = LIME;
		Rectangle topRect, botRect;
		bool active = true;

		Pipe(float x)
		{
			this->x = x;

			this->heightTop = GetRandomValue(175, 275);
			this->yBot = heightTop + 100;
			this->heightBot = screenY - yBot;

			topRect = Rectangle{ x,0,width,heightTop };
			botRect = Rectangle{ x,yBot,width,heightBot };
		}
		void Draw()
		{
			//draw when within bounds
			if (active)
			{
				DrawRectangleRec(topRect, col);
				DrawRectangleRec(botRect, col);
			}
		}

	}Pipe;

	vector<Pipe> pipes;
};

