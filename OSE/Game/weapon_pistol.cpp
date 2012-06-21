#include "weapon_pistol.h"
#include "../Engine/PhysicsWorld.h"

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
	me->mPhysicsWorld->TraceLine(info);
	
}
