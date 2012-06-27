#pragma once

#include "Utilities/Vector2.h"

class Model
{
public:
	
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

