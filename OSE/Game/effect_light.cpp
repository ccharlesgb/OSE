#include "effect_light.h"

LINKCLASSTONAME("effect_light", effect_light)

effect_light::effect_light(void)
{
	mLightInfo.SetPosition(GetPos());
	mLightInfo.SetRadius(512.f);
	mLightInfo.SetColour(Colour(200,200,200));
	RenderInit();
}

effect_light::~effect_light(void)
{
}

void effect_light::Think()
{
	mLightInfo.SetPosition(GetPos());
}