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
	int maxStat = 10;
	bool gameOver = false;
	Color backgroundCol = RAYWHITE;

	//bool petBusy = false; //Bool array sum is 1, active can only be 1
	bool petAction[3] = { false, false, false };
	bool petFree = true;

	float timer = 0; //general use
	float stareTimer;
	float playTimer;
	float pettingTimer;

	float decay = 0;
	float energyRegen = 0;

	bool played = false;
	bool canPet = true;
	bool canPlay = true;
	bool isMoving = false;

	bool BoundaryCheck();
	void PetInteraction();
	int PetCheck();

	enum Action { Playing, Petting, Eating };
	enum Mood { Default, Happy, Energetic, Sad, Hungry, Tired, Curious, Angry };
	enum Expression { Neutral, ClosedDown, ClosedUp, Squint, };

#pragma region Objects

	typedef struct Pet {
		//Stats
		Mood mood;
		Expression emote;
		int fullness;
		int energy;
		int play;
		int love;

		//Appearance
			//Shapes
				Rectangle body;
				Rectangle leftEar, rightEar;
				Rectangle leftEye, rightEye;
				Rectangle tail;
			//Size
				float bodySize = 100;
				Vector2 earSize = { 40, 60 };
				float eyeSize = 17;
				float tailSize = 30;
			//Colors
				Color bodyColor = WHITE;
				Color earColor = BLACK;
				Color tailColor = BLACK;
				Color eyeColor;

		//Variables
		Vector2 position;
		Vector2 speed = { 4, 3 };
		Vector4 eyeAnchor;

		//Functions
		Pet() //Initialization
		{
			mood = Default;
			emote = Neutral;
			fullness = 5;
			energy = 5;
			play = 5;
			love = 5;

			position = Vector2{ screenX / 2,screenY / 2 };

			body = Rectangle	{ 
								position.x - bodySize / 2,
								position.y - bodySize / 2,
								bodySize,
								bodySize 
			};
			tail = Rectangle	{
								body.x + bodySize - tailSize/8,
								body.y + bodySize - tailSize,
								tailSize,
								tailSize
			};

			leftEar = Rectangle	{ 
								body.x,
								body.y - bodySize / 3 - earSize.y / 3,
								earSize.x,
								earSize.y
			};
			rightEar = Rectangle{ 
								body.x + bodySize - earSize.x, 
								body.y - bodySize / 3 - earSize.y / 3,
								earSize.x,
								earSize.y
			};

			eyeAnchor = Vector4{
								body.x + bodySize / 2 - eyeSize / 2 - 25,
								body.y + bodySize / 3 + eyeSize / 2,
								body.x + bodySize / 2 - eyeSize / 2 + 25,
								body.y + bodySize / 3 + eyeSize / 2
			};
			leftEye = Rectangle{
								eyeAnchor.x,
								eyeAnchor.y,
								eyeSize,
								eyeSize
			};
			rightEye = Rectangle{
								eyeAnchor.z,
								eyeAnchor.w,
								eyeSize,
								eyeSize
			};
		}
		void Draw()
		{
			DrawRectangleRec(tail, tailColor);
			DrawRectangleRec(body, bodyColor);

			DrawRectangleRec(leftEar, earColor);
			DrawRectangleRec(rightEar, earColor);

			switch (mood)
			{
			case Default:
				eyeColor = BLACK;
				break;
			case Happy:
				eyeColor = PINK;
				break;
			case Energetic:
				eyeColor = GREEN;
				break;
			case Sad:
				eyeColor = DARKBLUE;
				break;
			case Hungry:
				eyeColor = BROWN;
				break;
			case Tired:
				eyeColor = GRAY;
				break;
			case Curious:
				eyeColor = ORANGE;
				break;
			case Angry:
				eyeColor = RED;
				break;
			default:
				eyeColor = BLACK;
				break;
			}

			DrawRectangleRec(leftEye, eyeColor);
			DrawRectangleRec(rightEye, eyeColor);

			switch (emote)
			{
			case Neutral:
				break;
			case ClosedDown:
				DrawRectangle(leftEye.x + 2, leftEye.y, leftEye.width - 4, leftEye.height - 2, bodyColor);
				DrawRectangle(rightEye.x + 2, rightEye.y, rightEye.width - 4, rightEye.height - 2, bodyColor);
				break;
			case ClosedUp:
				DrawRectangle(leftEye.x + 2, leftEye.y + 4, leftEye.width - 4, leftEye.height - 2, bodyColor);
				DrawRectangle(rightEye.x + 2, rightEye.y + 4, rightEye.width - 4, rightEye.height - 2, bodyColor);
				break;
			case Squint:
				DrawRectangle(leftEye.x, leftEye.y, leftEye.width, 5, bodyColor);
				DrawRectangle(leftEye.x, leftEye.y + leftEye.height/2 + 5, leftEye.width, 5, bodyColor);
				DrawRectangle(rightEye.x, rightEye.y, rightEye.width, 5, bodyColor);
				DrawRectangle(rightEye.x, rightEye.y + rightEye.height/2 + 5, rightEye.width, 5, bodyColor);
				break;
			default:
				break;
			}
		}
		void LookAt(Vector2 point) 
		{
			float eyeMovementRadius = 2; //Area around eye

			//LEFT EYE
				if (point.x > leftEye.x &&
					leftEye.x <= eyeAnchor.x + eyeMovementRadius)
				{
					leftEye.x++;
				}
				if (point.x < leftEye.x &&
					leftEye.x >= eyeAnchor.x - eyeMovementRadius)
				{
					leftEye.x--;
				}
				if (point.y > leftEye.y &&
					leftEye.y <= eyeAnchor.y + eyeMovementRadius)
				{
					leftEye.y++;
				}
				if (point.y < leftEye.y &&
					leftEye.y >= eyeAnchor.y - eyeMovementRadius)
				{
					leftEye.y--;
				}

			//RIGHT EYE
				if (point.x > rightEye.x &&
					rightEye.x <= eyeAnchor.z + eyeMovementRadius)
				{
					rightEye.x++;
				}
				if (point.x < rightEye.x &&
					rightEye.x >= eyeAnchor.z - eyeMovementRadius)
				{
					rightEye.x--;
				}
				if (point.y > rightEye.y &&
					rightEye.y <= eyeAnchor.w + eyeMovementRadius)
				{
					rightEye.y++;
				}
				if (point.y < rightEye.y &&
					rightEye.y >= eyeAnchor.w - eyeMovementRadius)
				{
					rightEye.y--;
				}
		}
		void ResetEyes() //Undos LookAtMouse
		{
			//LEFT EYE
			if (leftEye.x < eyeAnchor.x)
			{
				leftEye.x++;
			}
			if (leftEye.x > eyeAnchor.x)
			{
				leftEye.x--;
			}
			if (leftEye.y < eyeAnchor.y)
			{
				leftEye.y++;
			}
			if (leftEye.y > eyeAnchor.y)
			{
				leftEye.y--;
			}

			//RIGHT EYE
			if (rightEye.x < eyeAnchor.z)
			{
				rightEye.x++;
			}
			if (rightEye.x > eyeAnchor.z)
			{
				rightEye.x--;
			}
			if (rightEye.y < eyeAnchor.w)
			{
				rightEye.y++;
			}
			if (rightEye.y > eyeAnchor.w)
			{
				rightEye.y--;
			}
		}
		void Move(Vector2 pos)
		{
			//EDIT THE BOUNDARY INTO ANOTHER FUNCTION

			//Move TOWARDS pos
			if (position.x < pos.x)
				position.x += speed.x;
			if (position.x > pos.x)
				position.x -= speed.x;
			if (position.y < pos.y)
				position.y += speed.y;
			if (position.y > pos.y)
				position.y -= speed.y;

			/*if (position.x < pos.x && position.x + bodySize <= screenX)
				position.x += speed.x;
			if (position.x > pos.x && position.x - bodySize >= 0)
				position.x -= speed.x;
			if (position.y < pos.y && position.y + bodySize <= screenY)
				position.y += speed.y;
			if (position.y > pos.y && position.y - bodySize >= 0)
				position.y -= speed.y;*/

			body.x = position.x - bodySize / 2;
			body.y = position.y - bodySize / 2;

			if (tail.x < pos.x)
			{
				tail.x = body.x - tailSize + tailSize / 8;
			}
			else
			{
				tail.x = body.x + bodySize - tailSize / 8;
			}
			tail.y = body.y + bodySize - tailSize;

			leftEar.x = body.x;
			rightEar.x = body.x + bodySize - earSize.x;
			leftEar.y = body.y - bodySize / 3 - earSize.y / 3;
			rightEar.y = body.y - bodySize / 3 - earSize.y / 3;

			eyeAnchor.x = body.x + bodySize / 2 - eyeSize / 2 - 25;
			eyeAnchor.z = body.x + bodySize / 2 - eyeSize / 2 + 25;
			eyeAnchor.y = body.y + bodySize / 3 + eyeSize / 2;
			eyeAnchor.w = body.y + bodySize / 3 + eyeSize / 2;

			if (leftEye.x < pos.x)
			{
				leftEye.x = eyeAnchor.x + 3;
				rightEye.x = eyeAnchor.z + 3;
			}
			else
			{
				leftEye.x = eyeAnchor.x - 3;
				rightEye.x = eyeAnchor.z - 3;
			}
			if (leftEye.y < pos.y)
			{
				leftEye.y = eyeAnchor.y + 3;
				rightEye.y = eyeAnchor.w + 3;
			}
			else
			{
				leftEye.y = eyeAnchor.y - 3;
				rightEye.y = eyeAnchor.w - 3;
			}
		}

	}Pet;

	Pet pet;

#pragma endregion
};

