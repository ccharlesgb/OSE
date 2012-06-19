#include "Prop.h"
#include <iostream>

LINKCLASSTONAME("ent_prop", Prop)
#define GRAVITY_STRENGTH 150

Prop::Prop(void)
{
	RegisterInput("SetSprite", SetSprite);
	SetPos(Vector2::Random(-80.f,80.f));

	RenderInit();
	SetDrawOrder(RENDERGROUP_ENTITIES);
	PhysicsInit(DYNAMIC_BODY);
}

void Prop::SetSprite(BaseObject* ent, VariantMap &Data)
{
	Prop* me = dynamic_cast<Prop*>(ent);
	me->mPath = Data.ReadString("path");
}


void Prop::Spawn()
{
	GetPhysObj()->SetAngularDamping(7);
	GetPhysObj()->SetLinearDamping(10);

	PhysicsHullFromModel();
}

void Prop::StartTouch(CollisionInfo* info)
{
	if (info->OtherEnt->GetClassName() == "player")
	{
		Delete();
	}
}

Prop::~Prop(void)
{
}

void Prop::Think()
{
}
