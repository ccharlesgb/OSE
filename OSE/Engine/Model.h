#pragma once

#include "Utilities/Vector2.h"

class Model
{
public:
	
	bool Loaded;
	
	Vector2 mVertices[8];
	
	int mVertexCount;
	
	float mDensity;
	
	const char* mTexturePath;
	
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

