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
	bool gameOver = false;

#pragma region Structs

	typedef struct Pet {
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
			Color eyeColor = BLUE;
			Color tailColor = BLACK;

		//Variables
		Vector2 position;
		Vector2 speed;
		Vector4 eyeAnchor;

		//Functions
		Pet() //Initialization
		{
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

			speed = Vector2{ 0,0 };
		}
		void Draw()
		{
			DrawRectangleRec(tail, tailColor);
			DrawRectangleRec(body, bodyColor);

			DrawRectangleRec(leftEar, earColor);
			DrawRectangleRec(rightEar, earColor);

			DrawRectangleRec(leftEye, eyeColor);
			DrawRectangleRec(rightEye, eyeColor);
		}
		void LookAtMouse() 
		{
			Vector2 mousePoint = GetMousePosition();
			Vector2 leftEyeOG = Vector2{ eyeAnchor.x,eyeAnchor.y };
			Vector2 rightEyeOG = Vector2{ eyeAnchor.z,eyeAnchor.w };
			float eyeMovementRadius = 3; //Area around eye

			//LEFT EYE
				if (mousePoint.x > leftEye.x &&
					leftEye.x <= leftEyeOG.x + eyeMovementRadius)
				{
					leftEye.x++;
				}
				if (mousePoint.x < leftEye.x &&
					leftEye.x >= leftEyeOG.x - eyeMovementRadius)
				{
					leftEye.x--;
				}
				if (mousePoint.y > leftEye.y &&
					leftEye.y <= leftEyeOG.y + eyeMovementRadius)
				{
					leftEye.y++;
				}
				if (mousePoint.y < leftEye.y &&
					leftEye.y >= leftEyeOG.y - eyeMovementRadius)
				{
					leftEye.y--;
				}

			//RIGHT EYE
				if (mousePoint.x > rightEye.x &&
					rightEye.x <= rightEyeOG.x + eyeMovementRadius)
				{
					rightEye.x++;
				}
				if (mousePoint.x < rightEye.x &&
					rightEye.x >= rightEyeOG.x - eyeMovementRadius)
				{
					rightEye.x--;
				}
				if (mousePoint.y > rightEye.y &&
					rightEye.y <= rightEyeOG.y + eyeMovementRadius)
				{
					rightEye.y++;
				}
				if (mousePoint.y < rightEye.y &&
					rightEye.y >= rightEyeOG.y - eyeMovementRadius)
				{
					rightEye.y--;
				}
		}

	}Pet;

#pragma endregion

	Pet pet;


	//feed pet, pickup box when mouse pressed on rect, shake food into bowl, pet approaches 10 seconds after last shake frame
	//wash pet, pickup sponge when mouse pressed on rect, rub pet or dirty spots (lower opacity on enter) maybe hitbox smaller than square?
	//pet pet, rub pet
	//bar decay, pet slowly gets dirty
	//buttons UI
	//pet wander state
};

