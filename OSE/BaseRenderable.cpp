#include "BaseRenderable.h"

BaseRenderable::BaseRenderable(void)
{
	mAngle = 0.f;
	mIsRenderable = false;
	mIsPhysics = false;
}

BaseRenderable::~BaseRenderable(void)
{
}

void BaseRenderable::InitRenderer()
{
	mIsRenderable = true;
}

void BaseRenderable::Tick()
{
	if (mIsRenderable)
	{
		
	}
	BaseObject::Tick();
}