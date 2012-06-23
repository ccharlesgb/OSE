#pragma once

#include "pugixml.hpp"
#include <iostream>
#include "Vector2.h"

class ModelData
{
public:
	ModelData() {mVertexCount = 0; mDensity = 0.f; mTexturePath = NULL; mScale = 1.f;};
	Vector2 mVertices[8];
	int mVertexCount;
	float mDensity;

	const char* mTexturePath;
	float mScale;
};

class XMLParser
{
private:
	
public:
	static void LoadFile(const char *path);
};

