#include "Decal.h"
#include "../Engine/GameGlobals.h"

LINKCLASSTONAME("ent_decal", Decal);

Decal::Decal(void)
{
	RegisterInput("dietime", SetDietime);

	SetPos(Vector2::Random(-80.f,80.f));
	mDietime = -1.f;
	RenderInit();
	SetDrawOrder(RENDERGROUP_BACKGROUND);
}

void Decal::Spawn()
{
}

void Decal::SetDietime(BaseObject* ent, VariantMap &Data)
{
	Decal* me = dynamic_cast<Decal*>(ent);
	me->mDietime = Data.ReadFloat("time");
}

Decal::~Decal(void)
{
}

void Decal::Think()
{
	if (mDietime != -1.f && gGlobals.CurTime > mDietime)
	{
		Colour col = GetColour();
		float fade_factor = ig::Limit(1 - (gGlobals.CurTime - mDietime) / 5.f,0.f,1.f); // 1 - (SECONDS_PASSED / TOTAL_FADE_TIME)
		col.a = fade_factor * 255.f;
		SetColour(col);
		if (fade_factor == 0.f)
		{
			Delete();
		}
	}
}
