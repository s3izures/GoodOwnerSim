#pragma once
#include "raylib.h"
#include <vector>
using namespace std;

class PathFinder
{
public:
	void Main();

private:
#define ROWS 10
#define COLS 20

	typedef struct Tile {
		Vector2 pos;
		int row, col;
		Color fill, outline;
		bool blocked = false;

		void Draw()
		{
			if (blocked)
				DrawRectangleV(pos, Vector2{ 50,50 }, fill);
			else
				DrawRectangleLines(pos.x, pos.y, 50, 50, outline);
		}

		Tile(int row, int col, Vector2 pos, Color fill, Color outline, bool blocked) 
			: row(row), col(col), pos(pos), fill(fill), outline(outline), blocked(blocked){}
	}Tile;

	vector<vector<Tile>> tiles;

	void Start();
	void Update();
};

