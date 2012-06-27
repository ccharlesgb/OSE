#include "Model.h"
#include "Resource.h"

Model::Model()
{
	mLoaded = false;
	
	mVertexCount = 0;
	mDensity = 0.f;
	mTexturePath = NULL;
	mScale = 1.f;
}

bool Model::LoadFromFile(const char *path)
{
	Model *model = ModelResource::GetModel(path);
	mTexturePath = model->mTexturePath;
	mVertexCount = model->mVertexCount;
	for (int i=0; i < mVertexCount; i++)
		mVertices[i] = model->mVertices[i];
	mLoaded = true;
	mDensity = model->mDensity;
	mScale = model->mScale;
	return true;
}