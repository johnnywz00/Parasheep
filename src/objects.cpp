
#include "objects.hpp"

Enemy::Enemy ()
{
	float i = randRange(-350, -200);
	jumpVel = i / 100;
	floatRate = gFloatRate * (float(randRange(90, 110)) / 100);
}
