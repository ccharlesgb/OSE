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
		std::cout << "Weapon Firing\n";
		info.mStartPoint = me->GetPos();
		std::cout << "FORWARD: " << me->GetForward().ToString() << "\n";
		info.mEndPoint = me->GetPos() + (me->GetForward() * 1000);
		std::cout << info.mStartPoint.ToString() << info.mEndPoint.ToString() << "\n";
		BaseObject* hit_target = PhysicsQueries::TraceLine(info);
		if (hit_target != NULL)
		{
			std::cout << "Shot: " << hit_target->GetClassName() << "\n";
			hit_target->Delete();
		}
		me->SetNextPrimaryFire(gGlobals.CurTime + 0.3f);
	}
}
