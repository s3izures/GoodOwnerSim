#include "SnakeGame.h"
#include <time.h>

int main()
{
	SetRandomSeed(time(NULL));
	SnakeGame s;
	s.Main();
}