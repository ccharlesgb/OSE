#include "Decal.h"
#include <iostream>

LINKCLASSTONAME("ent_decal", Decal);

Decal::Decal(void)
{
	SetPos(Vector2::Random(-80.f,80.f));
	
	RenderInit();
	SetDrawOrder(RENDERGROUP_BACKGROUND);
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
