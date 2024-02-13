#pragma once
#include <raylib.h>
#include <vector>
#include <string>
using namespace std;

class MemoryGame
{
public:
#define FIXTIME + (GetFrameTime() * 100)

	// -------------- FUNCTIONS --------------

	void Main();
	void Start();
	void Update();
	void EvalFrame();
	void DrawFrame();


	// -------------- VARIABLES --------------

	const int screenWidth = 800;
	const int screenHeight = 500;
	int gridSize = 6;
	int gap = 5;
	float timeOfClick;
	bool canClick = true;

	// Tile + Grid
	float totalHeightMinusGaps = screenHeight - (gap + (gap * gridSize));
	float tileSize = totalHeightMinusGaps / gridSize;

	enum TileState {
		hidden,
		flipping,
		revealed,
		found,
	};

	typedef struct Tile {
		int num = -1;
		Rectangle rect;
		Color color;
		int fontSize = 36;
		int state = hidden;
		string text;

		Vector2 center = Vector2{ rect.x + rect.width / 2,rect.y + rect.height / 2 };
		
		void DetState()
		{
			switch (state)
			{
			case hidden:
				text = "?";
				color = GRAY;
				break;

			case flipping:
				text = "";
				color = YELLOW;
				break;

			case revealed:
				text = to_string(num);
				color = ORANGE;
				break;

			case found:
				text = ":D";
				color = LIME;
				break;

			default:
				break;
			}
		}
		void Draw()
		{
			DrawRectangleRounded(rect, 0.1, 5, color);
		}
		void DrawNum()
		{
			string ber = to_string(num);
			DrawText(text.c_str(), center.x - (MeasureText(ber.c_str(), fontSize) / 2), center.y - (fontSize / 2), fontSize, WHITE);
		}
	}Tile;

	vector<Tile> tiles;
	vector<Tile*> matching;
};

