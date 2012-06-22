#include "weapon_pistol.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/GameGlobals.h"

LINKCLASSTONAME("weapon_pistol", weapon_pistol)

weapon_pistol::weapon_pistol(void)
{
	RegisterInput("fire1", PrimaryFire);
	CreateSound("shot", "gunshot");
	RenderInit();
	GetSound("shot")->SetVolume(20.f);
	SetDrawOrder(RENDERGROUP_ENTITIES);
	mLine = new Line(gGlobals.RenderWindow);
	mLine->SetColour(Colour(255,215,0));
}

weapon_pistol::~weapon_pistol(void)
{
}

void weapon_pistol::Spawn()
{
	SetModel("muzzle_flash", 0.2);
}

void weapon_pistol::Draw()
{
	if (mLastShot + 0.05 > gGlobals.CurTime)
	{
		mLine->Draw();
		DrawModel();
	}
}

void weapon_pistol::PrimaryFire(BaseObject* ent, VariantMap &Data)
{
	weapon_pistol* me = dynamic_cast<weapon_pistol*>(ent);
	if (me->GetNextPrimaryFire() < gGlobals.CurTime)
	{
		me->EmitSound("shot");
		TraceInfo info;
		info.mStartPoint = me->GetPos();
		float range = 1000;
		float spread = 0.15; //In radians
		info.mEndPoint = me->GetPos() + (me->GetForward() * range) + (me->GetRight() * ig::Random(-spread,spread) * range);
		TraceResult TraceRes;
		PhysicsQueries::TraceLine(info, TraceRes);

		//Tracer Effect
		me->mLine->mVerts[0] = info.mStartPoint;
		me->mLine->mVerts[1] = info.mEndPoint;

		if (TraceRes.mHitEnt != NULL)
		{
			DamageInfo info;
			info.Amount = ig::Random(15,25);
			info.type = DAMAGETYPE_BULLET;
			info.Inflictor = me->GetOwner();
			TraceRes.mHitEnt->TakeDamage(info);
		}
		me->mLastShot = gGlobals.CurTime;
		me->SetNextPrimaryFire(gGlobals.CurTime + 0.25f);
	}
}
