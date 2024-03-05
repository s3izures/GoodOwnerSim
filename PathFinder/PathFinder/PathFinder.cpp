#include "PathFinder.h"

void PathFinder::Main()
{
	InitWindow(1000, 500, "Pathfinder");
	SetTargetFPS(60);
	Start();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		Update();

		for (vector<Tile> p : tiles)
			for (Tile v : p)
				v.Draw();

		EndDrawing();
	}

	CloseWindow();
}

void PathFinder::Start()
{
	tiles.clear();
	vector<Tile>rowTiles;
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			float y = 50 * row;
			float x = 50 * col;
			Tile tile = Tile(row, col, Vector2{ x,y }, DARKGRAY, GRAY, false);
			rowTiles.push_back(tile);
		}
		tiles.push_back(rowTiles);
	}

	for (int i = 0; i < 30; i++)
	{
		int randC = GetRandomValue(0, ROWS - 1);
		int randR = GetRandomValue(0, COLS - 1);

		
	}
}

void PathFinder::Update()
{
	for (int r = 0; r < tiles.size(); r++)
	{
		for (int c = 0; c < tiles[0].size(); c++)
		{

		}
	}
}
