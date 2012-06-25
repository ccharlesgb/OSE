#pragma once

#include "Utilities/Vector2.h"

class Model
{
public:
	Model();
	Vector2 mVertices[8];
	int mVertexCount;
	float mDensity;
	const char* mTexturePath;
	float mScale;
	bool LoadFromFile(const char *path);
};

