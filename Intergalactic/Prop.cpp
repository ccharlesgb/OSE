#include "Prop.h"
#include <iostream>

LINKCLASSTONAME("ent_prop", Prop)
#define GRAVITY_STRENGTH 150

Prop::Prop(void)
{
	RegisterInput("SetSprite", SetSprite);
	SetPos(Vector2::Random(-80.f,80.f));

	SetRenderType(RENDER_SPRITE);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_ENTITIES);

	PhysicsInit(DYNAMIC_BODY);
}

void Prop::SetSprite(BaseObject* ent, VariantMap &Data)
{

}


void Prop::Spawn()
{

}


Prop::~Prop(void)
{
}

void Prop::Think()
{
}
