#include "BasePhysics.h"

BasePhysics::BasePhysics(void)
{
	mPhysObj = NULL;
}

void BasePhysics::PhysicsInit(BODY_TYPE typ)
{
	mIsPhysics=true;
	mPhysObj = new PhysicsDef;
	mPhysObj->SetPos(GetPos());
	mPhysObj->SetAngle(0);
	mPhysObj->SetType(typ);
}

void BasePhysics::Tick()
{
	if (mPhysObj != NULL)
	{
		mPosition = mPhysObj->GetPos();
		mAngle = mPhysObj->GetAngle();
	}
	BaseRenderable::Tick();
}

BasePhysics::~BasePhysics(void)
{
	if (mIsPhysics)
		delete GetPhysObj();
}
