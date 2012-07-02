#include "effect_light.h"

LINKCLASSTONAME("effect_light", effect_light)

effect_light::effect_light(void)
{
	mLightInfo.SetPosition(GetPos());
	mLightInfo.SetRadius(512.f);
	mLightInfo.SetColour(Colour(ig::Random(0,255),ig::Random(0,255),ig::Random(0,255)));
	RenderInit();
}

effect_light::~effect_light(void)
{
}

void effect_light::Think()
{
	mLightInfo.SetPosition(GetPos());
}