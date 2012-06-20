#pragma once

#define _USE_MATH_DEFINES
#include "math.h"

namespace ig
{
void SeedRandom();
float Random(float min, float max);
int RandomInt(int min, int max);

float RadToDeg(float Rad);
float DegToRad(float Deg);

float NormalizeAngle(float Deg);

float Approach(float Current, float Target, float Inc);
float RotateTowards(float ang , float targ, float speed);

float Limit(float var, float min, float max);
}