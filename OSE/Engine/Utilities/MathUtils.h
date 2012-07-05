#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

class Vector2;

/**
 * Math Library
 *
 * @ingroup Utilty
 */
namespace ig
{
	/**
	 * Seed the random number generator. Called by main() to make sure
	 * random numbers are different every time.
	 */
	void SeedRandom();
	/**
	 * Get a random float between the two paramaters
	 * @param min The smallest number in the range
	 * @param max The largest number in the range
	 */
	float Random(float min, float max);
	/**
	 * Get a random integer between these two values
	 * @param min The smallest number in the range
	 * @param max The largest number in the range
	 */
	int RandomInt(int min, int max);
	/**
	 * Convert radians to degrees
	 * @param Rad The radians to be converted
	 */
	float RadToDeg(float Rad);
	/**
	 * Convert degrees to radians
	 * @param Deg the degrees to be converted
	 */
	float DegToRad(float Deg);
	/**
	 * Reduces angles to the range between -180 and 180
	 * @param Deg the angle to be normalised
	 */
	float NormalizeAngle(float Deg);
	/**
	 * Increments the current value until it reaches the target,
	 * adds (or subtracts) by Inc each time
	 * @param Current is the current value that needs to be incremented
	 * @param Target the value that Current should approach
	 * @param Inc how much Current should be incremented each call to
	 * this function.
	 */
	float Approach(float Current, float Target, float Inc);
	/**
	 * Helper function to smoothly rotate an angle to another angle,
	 * taking into account which direction is the fastest direction
	 * to spin.
	 * @param ang the current angle
	 * @param targ the target angle
	 * @param speed how fast it should approach
	 */
	float RotateTowards(float ang , float targ, float speed);

	/**
	 * Limit a value to this range
	 * @param var the value to be range checked
	 * @param min the minimum value in the range, if var is less
	 * than this the function will return min.
	 * @param max the maximum value in the range.
	 */
	float Limit(float var, float min, float max);
	
	/**
	 * Round a value down to the nearest integer
	 */
	inline float Floor(float val)
	{
		return std::floor(val);
	}

	/**
	 * Return the smaller value.
	 */
	template<class T>
	inline T Min(T one, T two)
	{
		return (one < two) ? one : two;
	}

	/**
	 * Return the bigger value.
	 */
	template<class T>
	inline T Max(T one, T two)
	{
		return (one > two) ? one : two;
	}

	/**
	 * Get the absolute value (magnitude) of a number
	 */
	inline float Abs(float val)
	{
		return (val >= 0.f) ? val : (val * -1.f);
	}

	inline int Abs(int val)
	{
		return (val >= 0) ? val : (val * -1);
	}


	/**
	 * Helper function to convert a coordinate in the game world, to
	 * the screenspace coordinate system for rendering
	 */
	Vector2 GameToSFML(Vector2 Pos);
	/**
	 * Helper function to convert a coordinate in screenspace to
	 * the gameworld coordinate system.
	 */
	Vector2 SFMLToGame(Vector2 Pos);
	/**
	 * Convert an angle in degrees relative to the horizontal (right)
	 * to a unit vector represting this direction.
	 */
	Vector2 AngleToVector(float angle);
	/**
	 * Round a vector to the nearest grid position.
	 */
	Vector2 NearestGrid(Vector2 pos, int grid_size_x , int grid_size_y);
}

