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
		BaseObject* hit_target = PhysicsQueries::TraceLine(info);
		if (hit_target != NULL)
		{
			DamageInfo info;
			info.Amount = ig::Random(15,25);
			info.type = DAMAGETYPE_BULLET;
			info.Inflictor = me->GetOwner();
			hit_target->TakeDamage(info);
		}
		me->mLastShot = gGlobals.CurTime;
		me->SetNextPrimaryFire(gGlobals.CurTime + 0.25f);
	}
}
