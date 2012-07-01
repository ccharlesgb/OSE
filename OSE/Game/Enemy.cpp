#include "Enemy.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/Render/Sprite.h"

//This function registers the entity to the EntityCreator.
LINKCLASSTONAME("enemy", Enemy)

Enemy::Enemy(void)
{
	mTarget = NULL;
	mLastSearch = ig::Random(0.f,1.f); //If laod
	mLastWander = ig::Random(0.f, 2.f);
}

void Enemy::Spawn()
{
	BaseHuman::Spawn();
	
	SetModel("npc/zombie", 0.25f);
	CreateHead("zombie_head");
	mHead->SetOrigin(Vector2(0,37));
	SetOrigin(Vector2(0,45));
	PhysicsHullFromModel();
	SetMaxHealth(150);
	SetHealth(150);
	UpdateWanderPos();
}

Enemy::~Enemy(void)
{
}

void Enemy::StartTouch(CollisionInfo *info)
{
	if (info->OtherEnt->GetClassName() == "player")
	{
		DamageInfo dmg_info;
		dmg_info.Amount = ig::Random(5,10);
		dmg_info.Inflictor = this;
		dmg_info.type = DAMAGETYPE_MELEE;
		info->OtherEnt->TakeDamage(dmg_info);
	}
}

void Enemy::UpdateWanderPos()
{
	mWanderPos = GetPos() + Vector2::Random(-400.f,400.f);
}

void Enemy::Think()
{
	if (mLastSearch + 1.f < gGlobals.CurTime)
	{
		ObjList* Players = gGlobals.gEntList.FindInCircle(GetPos(), 512);
		EntityList<BaseObject*>::iter CurEnt = Players->FirstEnt();
		while (CurEnt != Players->End())
		{
			if ((*CurEnt)->GetClassName() == "player")
			{
				mTarget = (*CurEnt);
				break;
			}
			CurEnt = Players->NextEnt(CurEnt);
		}
		if (mTarget == NULL && mLastWander + 6.f < gGlobals.CurTime)
		{
			UpdateWanderPos();
			mLastWander = gGlobals.CurTime;
		}
		mLastSearch = gGlobals.CurTime;
	}
}

void Enemy::TakeDamage(const DamageInfo &info)
{
	SetHealth(GetHealth() - info.Amount);
	Colour mCol = GetColour();
#pragma warning( disable : 4244 )
	mCol.r = (GetHealth() / GetMaxHealth()) * 210.f + 45.f;
	mCol.g = (GetHealth() / GetMaxHealth()) * 210.f + 45.f;
	mCol.b = (GetHealth() / GetMaxHealth()) * 210.f + 45.f;
#pragma warning( default : 4244 )
	SetColour(mCol);
	mHead->SetColour(mCol);
	if (GetHealth() <= 0)
		Delete();
}

void Enemy::PhysicsSimulate(float delta)
{
	float player_walk_speed = 36.f;
	//Point towards player
	Vector2 TargetPos;
	if (mTarget)
	{
		TargetPos = mTarget->GetPos();
	}
	else
	{
		TargetPos = mWanderPos;
		if ((TargetPos - GetPos()).Length() < 40.f) //Arrived!
		{
			return;
		}
	}
	Vector2 MouseDirHat = (TargetPos - GetPos()).Normalize();
	float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;

	SetHeadAngle(TargetAngle - GetAngle());

	GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass() * 0.3f);

	Vector2 MoveVector;
	MoveVector.y = 1;
	
	MoveVector = MoveVector.Normalize() * player_walk_speed;
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
}

void Enemy::OnDelete()
{
	BaseObject* decal = CreateEntity("ent_decal");
	decal->SetModel("decals/blood_spatter");
	decal->SetPos(GetPos());
}