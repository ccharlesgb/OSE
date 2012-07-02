#include "effect_light.h"


LINKCLASSTONAME("effect_light", effect_light)

effect_light::effect_light(void)
{
	mLightInfo.SetPosition(GetPos());
	mLightInfo.SetRadius(512.f);
	SetRenderBounds(Vector2_Rect(Vector2(-256, -256),Vector2(512,512)));
	Colour col;
	int id = ig::RandomInt(0,3);
	switch (id)
	{
	case 0:
		col = Colour(255,0,0);
		break;
	case 1:
		col = Colour(0,255,0);
		break;
	case 2:
		col = Colour(0,0,255);
		break;
	case 3:
		col = Colour(255,255,255);
		break;
	}

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