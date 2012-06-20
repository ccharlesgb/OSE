#include "Decal.h"
#include <iostream>

LINKCLASSTONAME("ent_decal", Decal);

Decal::Decal(void)
{
	RegisterInput("SetSprite", SetSprite);
	SetPos(Vector2::Random(-80.f,80.f));
	
	RenderInit();
	SetDrawOrder(RENDERGROUP_ENTITIES);
}

void Decal::SetSprite(BaseObject* ent, VariantMap &Data)
{
	Decal* me = dynamic_cast<Decal*>(ent);
	me->mPath = Data.ReadString("path");
}

void Decal::Spawn()
{
	
}

Decal::~Decal(void)
{
}

void Decal::Think()
{
}
