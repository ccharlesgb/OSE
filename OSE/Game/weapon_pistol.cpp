#include "weapon_pistol.h"
#include "../Engine/PhysicsWorld.h"

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
	info.mStartPoint = me->GetPos();
	info.mEndPoint = me->GetPos() + (me->GetForward() * 1000);
	//BaseObject* hit_target = me->mPhysicsWorld->TraceLine(info);
	//hit_target->Delete();
}
