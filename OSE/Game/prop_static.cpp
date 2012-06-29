#include "Prop_Static.h"

LINKCLASSTONAME("ent_prop_static", Prop_Static)
#define GRAVITY_STRENGTH 150

Prop_Static::Prop_Static(void)
{
	RegisterInput("SetSprite", SetSprite);
	SetPos(Vector2::Random(-80.f,80.f));

	RenderInit();
	SetDrawOrder(RENDERGROUP_ENTITIES);
	PhysicsInit(STATIC_BODY);
}

void Prop_Static::SetSprite(BaseObject* ent, VariantMap &Data)
{
	Prop_Static* me = dynamic_cast<Prop_Static*>(ent);
	me->mPath = Data.ReadString("path");
}

void Prop_Static::TakeDamage(const DamageInfo &info)
{

}

void Prop_Static::Spawn()
{
	GetPhysObj()->SetRestitution(1.f);
	GetPhysObj()->SetAngularDamping(7);
	GetPhysObj()->SetLinearDamping(10);

	PhysicsHullFromModel();
}

void Prop_Static::StartTouch(CollisionInfo* info)
{
}

Prop_Static::~Prop_Static(void)
{
}

void Prop_Static::Think()
{
}

void Prop_Static::OnDelete()
{
}
