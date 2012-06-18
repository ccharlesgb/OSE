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
	switch(mRenderType)
	{
	case RENDER_SHAPE:
		mRender = new ShapeRender;
		break;
	case RENDER_SPRITE:
		mRender = new SpriteRender;
		break;
	case RENDER_CUSTOM:
		break;
	default:
		std::cout << "INVALID RENDER TYPE\n";
	}
	//mRender->SetPos(GetPos());
	mIsRenderable = true;
}

void BaseRenderable::Tick()
{
	if (mIsRenderable)
	{
		mRender->SetPos(GetPos());
		mRender->SetAngle(GetAngle());
	}
	BaseObject::Tick();
}