#include "effect_light.h"


LINKCLASSTONAME("effect_light", effect_light)

effect_light::effect_light(void)
{
	mLightInfo.SetPosition(GetPos());
	mLightInfo.SetRadius(512.f);
	SetRenderBounds(Vector2_Rect(Vector2(-256, -256),Vector2(512,512)));
	Colour col = Colour(150,100,30);
	mLightInfo.SetColour(col);
	RenderInit();
	mLightInfo.UpdateLightSprite();
	mHasShadows = false;
}

effect_light::~effect_light(void)
{
}

void effect_light::Think()
{
	mLightInfo.SetPosition(GetPos());
	mLightInfo.SetAngle(GetAngle());
}