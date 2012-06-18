#include "StarField.h"
#include <iostream>

LINKCLASSTONAME("ent_starfield", StarField)

StarField::StarField(void)
{
	SetPos(Vector2::Random(-80.f,80.f));

	SetRenderType(RENDER_SPRITE);
	//InitRenderer();
	//GetRenderer()->SetTexture(std::string("planet.png"), true);
	//GetRenderer()->SetSize(Vector2(mRadius * 2.f,mRadius * 2.f));
	SetDrawPriority(0);
	mIsPhysics = true;
}

StarField::~StarField(void)
{
}

void StarField::Think()
{

}
