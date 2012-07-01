#include "BasePhysics.h"
#include <fstream>
#include <iostream>

BasePhysics::BasePhysics(void)
{
	mPhysObj = NULL;
	SetCastShadows(true);
}

void BasePhysics::PhysicsInit(BODY_TYPE typ)
{
	mIsPhysics = true;
	mPhysObj = new PhysicsDef;
	mPhysObj->SetOwner(this);
	mPhysObj->SetPos(GetPos());
	mPhysObj->SetAngle(GetAngle());
	mPhysObj->SetType(typ);
}

void BasePhysics::PhysicsHullFromModel()
{
	if (mModelInfo->mLoaded)
	{
		PolygonShape shape;
		shape.mDensity = mModelInfo->mDensity;
		
		for (int i = 0; i < mModelInfo->mVertexCount; i++)
		{
			Vector2 vert = mModelInfo->mVertices[i];
			vert.x *= mScale.x;
			vert.y *= mScale.y;
			shape.AddVertex(vert);
		}
		
		GetPhysObj()->AddPhysicsShape(&shape);
		return;
	}
}

void BasePhysics::Tick()
{
	if (mPhysObj != NULL && mPhysObj->IsAwake())
	{
		SetPos(mPhysObj->GetPos());
		SetAngle(mPhysObj->GetAngle());
	}
	BaseObject::Tick();
}

BasePhysics::~BasePhysics(void)
{
	if (mIsPhysics)
		delete GetPhysObj();
}
