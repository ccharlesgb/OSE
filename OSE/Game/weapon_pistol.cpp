#include "weapon_pistol.h"
#include "../Engine/PhysicsQueries.h"

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
	TraceInfo info;
	std::cout << "Weapon Firing\n";
	info.mStartPoint = me->GetPos();
	info.mEndPoint = me->GetPos() + (me->GetForward() * 1000);
	std::cout << info.mStartPoint.ToString() << info.mEndPoint.ToString() << "\n";
	BaseObject* hit_target = PhysicsQueries::TraceLine(info);
	if (hit_target != NULL)
		hit_target->Delete();
}
