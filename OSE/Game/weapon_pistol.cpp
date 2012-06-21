#include "weapon_pistol.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/GameGlobals.h"

LINKCLASSTONAME("weapon_pistol", weapon_pistol)

weapon_pistol::weapon_pistol(void)
{
	RegisterInput("fire1", PrimaryFire);
}

weapon_pistol::~weapon_pistol(void)
{
}

void weapon_pistol::PrimaryFire(BaseObject* ent, VariantMap &Data)
{
	weapon_pistol* me = dynamic_cast<weapon_pistol*>(ent);
	if (me->GetNextPrimaryFire() < gGlobals.CurTime)
	{
		TraceInfo info;
		info.mStartPoint = me->GetPos();
		float range = 1000;
		float spread = 0.3; //In radians
		info.mEndPoint = me->GetPos() + (me->GetForward() * range) + (me->GetRight() * ig::Random(-spread,spread) * range);
		BaseObject* hit_target = PhysicsQueries::TraceLine(info);
		if (hit_target != NULL)
		{
			hit_target->Delete();
		}
		me->SetNextPrimaryFire(gGlobals.CurTime + 0.1f);
	}
}
