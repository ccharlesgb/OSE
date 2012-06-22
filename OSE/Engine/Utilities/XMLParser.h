#pragma once

#include <mxml.h>
#include <iostream>
#include "Vector2.h"

class ModelData
{
public:
	ModelData() {mVertexCount = 0; mDensity = 0.f; mTexturePath = NULL;};
	Vector2 mVertices[8];
	int mVertexCount;
	float mDensity;
	const char* mTexturePath;
};

class XMLParser
{
private:
	static FILE *mFile;
	static mxml_node_t *mTree;
	static ModelData CurrentModel;
public:
	static mxml_type_t LoadCallBack(mxml_node_t *node);
	static void LoadFile(const char *path);
};

