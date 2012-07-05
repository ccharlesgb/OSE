#include "effect_light.h"


LINKCLASSTONAME("effect_light", effect_light)

effect_light::effect_light(void)
{
	mLightInfo.SetPosition(GetPos());
	mLightInfo.SetRadius(512.f);
	SetRenderBounds(Vector2_Rect(Vector2(-256, -256),Vector2(512,512)));
	Colour col = Colour(255,215,100);
	mLightInfo.SetColour(col);
	RenderInit();
	mLightInfo.UpdateLightSprite();
}

effect_light::~effect_light(void)
{
}

void effect_light::Think()
{
	mLightInfo.SetPosition(GetPos());
}