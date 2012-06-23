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
	mLine->SetColour(Colour(255, 236, 139));
	mNextPrimaryFire = 0.f;
	mLastShot = 0.f;
}

weapon_pistol::~weapon_pistol(void)
{
}

void weapon_pistol::Spawn()
{
	SetModel("muzzle_flash", 0.2f);
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
		info.mStartPoint = me->GetPos() + (me->GetForward() * -10.f);
		float range = 2000;
		float spread = 0.1f; //In radians
		info.mEndPoint = me->GetPos() + (me->GetForward() * range) + (me->GetRight() * ig::Random(-spread,spread) * range);
		TraceResult TraceRes;
		PhysicsQueries::TraceLine(info, &TraceRes);

		//Tracer Effect
		me->mLine->mVerts[0] = info.mStartPoint;
		me->mLine->mVerts[1] = TraceRes.mHitPos + (me->GetForward() * 10.f); // Add some slop to prevent line stopping before sprite

		if (TraceRes.mHitEnt != NULL)
		{
			DamageInfo info;
			info.Amount = ig::Random(15,25);
			info.type = DAMAGETYPE_BULLET;
			info.Inflictor = me->GetOwner();
			TraceRes.mHitEnt->TakeDamage(info);
		}
		me->mLastShot = (float)gGlobals.CurTime;
		me->SetNextPrimaryFire((float)gGlobals.CurTime + 0.1f);
	}
}
