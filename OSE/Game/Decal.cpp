#include "Decal.h"
#include <iostream>

LINKCLASSTONAME("ent_decal", Decal);

Decal::Decal(void)
{
	SetPos(Vector2::Random(-80.f,80.f));
	
	RenderInit();
	SetDrawOrder(RENDERGROUP_ENTITIES);
	
}

void Decal::Spawn()
{
	std::cout << "DECAL PHYS " << mIsPhysics << "\n";
}

Decal::~Decal(void)
{
}

void Decal::Think()
{
}
