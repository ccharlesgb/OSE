#include "Enemy.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"

//This function registers the entity to the EntityCreator.
LINKCLASSTONAME("enemy", Enemy)

Enemy::Enemy(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	mTarget = NULL;
}

void Enemy::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
	SetModel("zombie", 0.25);
	SetOrigin(Vector2(0,45));
	PhysicsHullFromModel();
	SetMaxHealth(200);
	SetHealth(200);
}

Enemy::~Enemy(void)
{
}

void Enemy::Draw()
{
	DrawModel();
	//mText->Draw();
}

void Enemy::Think()
{
	ObjList* Players = gGlobals.gEntList.FindInCircle(GetPos(), 512);
	EntityList<BaseObject*>::iter CurEnt = Players->FirstEnt();
	while (Players->CurrentIsValid())
	{
		if ((*CurEnt)->GetClassName() == "player")
		{
			mTarget = (*CurEnt);
			break;
		}
		CurEnt = Players->NextEnt(CurEnt);
	}
}

void Enemy::TakeDamage(const DamageInfo &info)
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

void Enemy::PhysicsSimulate(float delta)
{
	float player_walk_speed = 50.f;
	//Point towards player
	if (mTarget)
	{
		Vector2 MousePos = mTarget->GetPos();
		Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
		float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;
		GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass());

		Vector2 MoveVector;
		MoveVector.y = 1;
	
		MoveVector = MoveVector.Normalize() * player_walk_speed;
		MoveVector = ToGlobal(MoveVector) - GetPos();
		ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
	}
}
