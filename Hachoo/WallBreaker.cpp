#include "WallBreaker.h"
#include "raylib.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

#pragma region BASIC_FUNCTIONS
void WallBreaker::Main()
{
	InitWindow(screenSize.x, screenSize.y, "Wallbreaker");

	InitAudioDevice();
	LoadAudio();
	
	Start();
	SetTargetFPS(120);
	while(!WindowShouldClose())
	{
		Update();
	}
	
	for (int i = 0; i < soundEffects.size(); i++)
	{
		UnloadSound(soundEffects[i]);
	}

	CloseAudioDevice();
	CloseWindow();
}

void WallBreaker::Start()
{
	//Generate Level
	GenerateLevel();

	//Make stuff
	MakeBricks();
	MakePlayer();
	MakeBall();

	//Play moosic
	PlaySound(soundEffects[6]);
}

void WallBreaker::EvalCurFrame()
{
	//Game Over and Paused
	if (gameOver)
	{
		for (int i = 0; i < soundEffects.size(); i++)
		{
			StopSound(soundEffects[i]);
		}
		Restart();
		return;
	}
	if (IsKeyPressed(KEY_P))
	{
		PlaySound(soundEffects[7]);
		gamePaused = !gamePaused;
		if (gamePaused)
		{
			PauseSound(soundEffects[6]);
		}
		else
		{
			ResumeSound(soundEffects[6]);
		}
	}
	if (gamePaused)
	{
		return;
	}

	//Balls
	if (!ball.active)
	{
		ball.position = Vector2{ player.position.x,screenSize.y * 9 / 10 - ball.radius - 30 };
		if (IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
		}
	}
	else
	{
		ball.position.x += ball.speed.x * FIX_SPEED;
		ball.position.y += ball.speed.y * FIX_SPEED;
	}

	//Player controls
	if (IsKeyDown(KEY_LEFT) && player.position.x - (player.size.x/2) > 0 )
	{
		player.position.x -= player.speed * FIX_SPEED;
	}
	else if (IsKeyDown(KEY_RIGHT) && player.position.x + (player.size.x / 2) < (int)GetScreenWidth() - 5)
	{
		player.position.x += player.speed * FIX_SPEED;
	}

	//Collisions
	CollisionPaddle();
	CollisionWalls();
	CollisionBall();

	//LOSS/WIN
	if (player.curLives == 0)
	{
		StopSound(soundEffects[6]);
		PlaySound(soundEffects[5]);
		gameOver = true;
	}
	else
	{
		if (bricks.size() == 0)
		{
			StopSound(soundEffects[6]);
			PlaySound(soundEffects[4]);
			levelWin = true;
		}
	}

	//Sound be quiet shh
	if (IsKeyPressed(KEY_M))
	{
		if (IsSoundPlaying(soundEffects[6]))
		{
			StopSound(soundEffects[6]);
		}
		else
		{
			PlaySound(soundEffects[6]);
		}
	}

	ball.prevPosition = ball.position;
}

void WallBreaker::DrawCurFrame()
{
	BeginDrawing();
	ClearBackground(WHITE);

	if (!gameOver && !levelWin)
	{
		//Draw lives
		DrawText("LIVES", 20, GetScreenHeight() - 20 - 20, 18, BLACK);
		for (int i = 0; i < player.maxLives; i++)
		{
			if (i < player.curLives)
			{
				DrawRectangle(20 + 20 * i, GetScreenHeight() - 20, 10, 10, DARKBLUE);
			}
			else
			{
				DrawRectangle(20 + 20 * i, GetScreenHeight() - 20, 10, 10, LIGHTGRAY);
			}
		}

		//Draw things
		for (Brick b : bricks)
		{
			b.Draw();
			b.DrawCollider();
		}
		player.Draw();
		ball.Draw();
	}
	else if (levelWin)
	{
		DrawText("Press Enter to go to the next level", GetScreenWidth() / 2 - (MeasureText("Press Enter to go to the next level", 20) / 2), GetScreenHeight() / 2 - 20, 20, BLACK);
	}
	else
	{
		DrawText("Press Enter to play again", GetScreenWidth() / 2 - (MeasureText("Press Enter to play again", 20)/2), GetScreenHeight() / 2 - 20, 20, BLACK);
	}

	EndDrawing();
}

void WallBreaker::Update()
{
	EvalCurFrame();
	DrawCurFrame();
}
#pragma endregion

#pragma region SETUP
void WallBreaker::LoadAudio()
{
	Sound launchSFX = LoadSound("Resources\\beepa.wav");
	Sound hitSFX = LoadSound("Resources\\beepb.wav");
	Sound dieSFX = LoadSound("Resources\\beepc.wav");

	SetSoundVolume(launchSFX, 0.5);
	SetSoundVolume(hitSFX, 0.5);
	SetSoundVolume(dieSFX, 0.5);

	Sound powerupSFX = LoadSound("Resources\\powerup.wav");
	Sound winSFX = LoadSound("Resources\\win.wav");
	Sound loseSFX = LoadSound("Resources\\fail.wav");
	Sound BGM = LoadSound("Resources\\music.wav");
	Sound menuSFX = LoadSound("Resources\\menu.wav");

	soundEffects.push_back(launchSFX); //0
	soundEffects.push_back(hitSFX); //1
	soundEffects.push_back(dieSFX); //2
	soundEffects.push_back(powerupSFX); //3
	soundEffects.push_back(winSFX); //4
	soundEffects.push_back(loseSFX); //5
	soundEffects.push_back(BGM); //6
	soundEffects.push_back(menuSFX); //7
}

void WallBreaker::Restart()
{
	if (IsKeyPressed(KEY_ENTER))
	{
		bricks.clear();
		Start();
		gameOver = false;
	}
}

void WallBreaker::GenerateLevel()
{
	int rows = rand() % 5 + 3;
	int cols = rand() % 5 + 5;
	int colorStyle = rand() % 2;
	brickRows = rows;
	brickCols = cols;
	if (colorStyle == 0) //Set Palette
	{
		Color list[7] = { RED,ORANGE,YELLOW,GREEN,BLUE,PURPLE,PINK };
		for (int i = 0; i < brickRows * brickCols; i++)
		{
			brickColor.push_back(list[i % 7]);
		}
	}
	else if (colorStyle == 1) //Random Colors
	{
		for (int i = 0; i < brickRows * brickCols; i++)
		{
			Vector3 randomRGB = { rand() % 255,rand() % 255,rand() % 255 };
			brickColor.push_back(Color{ (unsigned char)randomRGB.x,(unsigned char)randomRGB.y,(unsigned char)randomRGB.z,255 });
		}
	}
}
#pragma endregion

#pragma region ASSET_CREATION
void WallBreaker::MakeBricks()
{
	//A sprinkle of more random
	int pattern = rand() % 4;

	//Brick Creation
	float spaceForBricks = screenSize.x - (brickCols * gap + gap);
	Vector2 brickSize = Vector2{ spaceForBricks / brickCols, brickHeight };

	for (int row = 0; row < brickRows; row++)
	{
		for (int col = 0; col < brickCols; col++)
		{
			//Size
			float x = gap + (gap + brickSize.x) * col;
			float y = gap + (gap + brickSize.y) * row;

			//Rect
			Rectangle brick = Rectangle{ x,y,brickSize.x,brickSize.y };
			Brick brickk;

			//Color pattern
			if (pattern == 0) //By Row
			{
				brickk = Brick{ brickColor[row],brick };
			}
			else if (pattern == 1) //Alternating
			{
				brickk = Brick{ brickColor[row + col],brick };
			}
			else if (pattern == 2) //By Col
			{
				brickk = Brick{ brickColor[col],brick };
			}
			else if (pattern == 3) //Absolute Chaos
			{
				int max = rand() % brickColor.size();
				brickk = Brick{ brickColor[max], brick };
			}

			//Collisions
			int hitboxSize = 10;
			brickk.hitboxes[tLCorn] = Rectangle{ x,y,brickSize.x / hitboxSize,brickSize.y / hitboxSize };
			brickk.hitboxes[bLCorn] = Rectangle{ x,y + brickSize.y - (brickSize.y / hitboxSize),brickSize.x / hitboxSize,brickSize.y / hitboxSize };
			brickk.hitboxes[tRCorn] = Rectangle{ x + brickSize.x - (brickSize.x / hitboxSize), y,brickSize.x / hitboxSize,brickSize.y / hitboxSize };
			brickk.hitboxes[bRCorn] = Rectangle{ x + brickSize.x - (brickSize.x / hitboxSize), y + brickSize.y - (brickSize.y / hitboxSize),brickSize.x / hitboxSize,brickSize.y / hitboxSize };

			brickk.hitboxes[top] = Rectangle{ x + brickSize.x / (hitboxSize - 1), y - (brickSize.y / (hitboxSize - 1)),brickSize.x - float(brickSize.x / 4.5),1 };
			brickk.hitboxes[bottom] = Rectangle{ x + brickSize.x / (hitboxSize - 1), y + brickSize.y + (brickSize.y / (hitboxSize - 1)),brickSize.x - float(brickSize.x / 4.5),1 };
			brickk.hitboxes[left] = Rectangle{ x, y + (brickSize.y / (hitboxSize - 1)), 1, brickSize.y - float(brickSize.y / 4.5) };
			brickk.hitboxes[right] = Rectangle{ x + brickSize.x, y + (brickSize.y / (hitboxSize - 1)), 1, brickSize.y - float(brickSize.y / 4.5) };

			bricks.push_back(brickk);
		}
	}
}

void WallBreaker::MakePlayer()
{
	player.position = Vector2{ screenSize.x / 2,screenSize.y * 9 / 10 };
	player.size = { screenSize.x / 10,20 };
	player.curLives = player.maxLives;
}

void WallBreaker::MakeBall()
{
	ball.position = Vector2{ player.position.x,player.position.y - 30 };
	ball.speed = Vector2{ 0,-5 };
	ball.radius = 10;
	ball.color = DARKBLUE; //Default
}
#pragma endregion

#pragma region COLLISION
void WallBreaker::CollisionPaddle()
{
	if (CheckCollisionCircleRec(ball.position, ball.radius, player.getRect()))
	{
		PlaySound(soundEffects[0]);
		ball.speed.y *= -1;
		ball.speed.x = (ball.position.x - player.position.x) / (player.size.x / 10);
	}
}

void WallBreaker::CollisionWalls()
{
	if (ball.position.y <= 0 + ball.radius) //Top screen
	{
		PlaySound(soundEffects[0]);
		//Reverse ball
		ball.speed.y *= -1;
	}
	if (ball.position.x <= 0 + ball.radius || ball.position.x >= GetScreenWidth() - ball.radius) //Side screen
	{
		PlaySound(soundEffects[0]);
		//Reverse ball
		ball.speed.x *= -1;
	}
	if (ball.position.y >= GetScreenHeight() + 20) //Bottom screen
	{
		PlaySound(soundEffects[2]);
		player.curLives--;
		ball.active = false;
		ball.speed = { 0,-5 };
	}
}

void WallBreaker::CollisionBall()
{
	for (int i = 0; i < bricks.size(); i++)
	{
		if (CheckCollisionCircleRec(ball.position, ball.radius, bricks[i].rect))
		{
			/*SpawnLife(Vector2{ bricks[i].rect.x, bricks[i].rect.y });*/
			int scenario = CollisionWithHitBox(bricks[i]);

			Vector2 hitBoxesPositions[8];
			for (int j = 0; j != end; j++)
			{
				hitBoxesPositions[j].x = bricks[i].hitboxes[j].x;
				hitBoxesPositions[j].y = bricks[i].hitboxes[j].y;
			}

			// delete the brick
			bricks.erase(bricks.begin() + i);

			PlaySound(soundEffects[1]);

			/*Default*/
			//ball.speed.y *= -1;

			switch (scenario)
			{
			case tLCorn:
				if (ball.prevPosition.x < hitBoxesPositions[tLCorn].x)
				{
					ball.speed.x *= -1;
				}

				if (ball.prevPosition.y < hitBoxesPositions[tLCorn].y)
					ball.speed.y *= -1;

				break;

			case tRCorn:
				if (ball.prevPosition.x > hitBoxesPositions[tRCorn].x)
				{
					ball.speed.x *= -1;
				}

				if (ball.prevPosition.y < hitBoxesPositions[tRCorn].y)
					ball.speed.y *= -1;

				break;

			case bLCorn:
				if (ball.prevPosition.x < hitBoxesPositions[bLCorn].x)
				{
					ball.speed.x *= -1;
				}

				if (ball.prevPosition.y > hitBoxesPositions[bLCorn].y)
					ball.speed.y *= -1;

				break;

			case bRCorn:
				if (ball.prevPosition.x > hitBoxesPositions[bRCorn].x)
				{
					ball.speed.x *= -1;
				}

				if (ball.prevPosition.y > hitBoxesPositions[bRCorn].y)
					ball.speed.y *= -1;

				break;

			case top:
				ball.speed.y *= -1;
				break;

			case bottom:
				ball.speed.y *= -1;
				break;

			case left:
				ball.speed.x *= -1;
				break;

			case right:
				ball.speed.x *= -1;
				break;

			default:
				break;
			}

			break;		
		}


	}
}

int WallBreaker::CollisionWithHitBox(Brick brick)
{
	for (int i = 0; i != end; i++)
	{
		if (CheckCollisionCircleRec(ball.position, ball.radius, brick.hitboxes[i]))
		{
			return i;
		}
	}
}
#pragma endregion