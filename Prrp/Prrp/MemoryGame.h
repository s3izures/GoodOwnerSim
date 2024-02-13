#pragma once
#include <raylib.h>
#include <vector>
#include <string>
using namespace std;

class MemoryGame
{
public:
#define SPEEDCHECK = GetFrameTime() * 50

	// -------------- FUNCTIONS --------------

	void Main();
	void Start();
	void Update();
	void EvalFrame();
	void DrawFrame();


	// -------------- VARIABLES --------------

	const int screenWidth = 800;
	const int screenHeight = 500;
	int gridSize = 4;
	int gap = 5;

	// Tile + Grid
	float totalHeightMinusGaps = screenHeight - (gap + (gap * gridSize));
	float tileSize = totalHeightMinusGaps / gridSize;
	typedef struct Tile {
		int num = -1;
		Rectangle rect;
		Color color = GRAY;
		int fontSize = 36;

		Vector2 center = Vector2{ rect.x + rect.width / 2,rect.y + rect.height / 2 };
		
		void Draw()
		{
			DrawRectangleRounded(rect, 0.1, 5, color);
		}
		void DrawNum()
		{
			string ber = to_string(num);
			DrawText(ber.c_str(), center.x - (MeasureText(ber.c_str(),fontSize) / 2), center.y - (fontSize / 2), fontSize, WHITE);
		}
	}Tile;

	vector<Tile> tiles;


};

