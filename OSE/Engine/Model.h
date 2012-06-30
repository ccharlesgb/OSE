#pragma once

#include "Utilities/Vector2.h"
#include "Animation.h"

#define MODEL_MAX_ANIMATIONS 4

struct Model
{
public:
	
	//ANIMAION PROPERTIES
	Animation mAnimations[MODEL_MAX_ANIMATIONS];
	bool mIsAnimating;
	int mAnimationCount;


	bool mLoaded;
	
	Vector2 mVertices[8];
	
	int mVertexCount;
	
	float mDensity;
	
	std::string mTexturePath;
	
	float mScale;
	
	/**
	 * Default constructor.
	 */
	Model();
	
	/**
	 * Load model information from file (xml).
	 *
	 * @param char Path
	 */
	bool LoadFromFile(const char *path);
};

