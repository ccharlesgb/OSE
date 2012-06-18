#include "BasePhysics.h"

BasePhysics::BasePhysics(void)
{
	mPhysObj = NULL;
	mIsPhysics=true;
}

void BasePhysics::PhysicsInit(BODY_TYPE typ)
{
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
	BaseObject::Tick();
}

BasePhysics::~BasePhysics(void)
{
	if (mIsPhysics)
		delete GetPhysObj();
}
