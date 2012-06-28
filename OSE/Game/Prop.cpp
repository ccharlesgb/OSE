#include "Prop.h"

LINKCLASSTONAME("ent_prop", Prop)
#define GRAVITY_STRENGTH 150

Prop::Prop(void)
{
	RegisterInput("SetSprite", SetSprite);
	SetPos(Vector2::Random(-80.f,80.f));

	RenderInit();
	SetDrawOrder(RENDERGROUP_ENTITIES);
	PhysicsInit(DYNAMIC_BODY);
}

void Prop::SetSprite(BaseObject* ent, VariantMap &Data)
{
	Prop* me = dynamic_cast<Prop*>(ent);
	me->mPath = Data.ReadString("path");
}

void Prop::TakeDamage(const DamageInfo &info)
{
	SetHealth(GetHealth() - info.Amount);
	Colour mCol = GetColour();
	mCol.r = (GetHealth() / GetMaxHealth()) * 210.f + 45.f;
	mCol.g = (GetHealth() / GetMaxHealth()) * 210.f + 45.f;
	mCol.b = (GetHealth() / GetMaxHealth()) * 210.f + 45.f;
	SetColour(mCol);
	if (GetHealth() <= 0)
		Delete();
}

void Prop::Spawn()
{
	GetPhysObj()->SetRestitution(1.f);
	GetPhysObj()->SetAngularDamping(7);
	GetPhysObj()->SetLinearDamping(10);

	PhysicsHullFromModel();
}

void Prop::StartTouch(CollisionInfo* info)
{
	if (info->Speed > 750)
	{
		BaseObject* scorch = CreateEntity("ent_decal");
		scorch->SetModel("scorch", ig::Random(0.5f,0.7f));
		scorch->SetPos(GetPos());
		scorch->Spawn();
		Delete();
	}
}

Prop::~Prop(void)
{
}

void Prop::Think()
{
}

void Prop::OnDelete()
{
}
