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
	SetModel("zombie", 0.3);
	SetOrigin(Vector2(0,45));
	PhysicsHullFromModel();
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
	ObjList* Players = gGlobals.gEntList.FindInCircle(GetPos(), 1024);
	BaseObject* CurEnt = Players->FirstEnt();
	if (Players->CurrentIsValid())
	{
		if (CurEnt->GetClassName() == "player")
			mTarget = Players->FirstEnt();
		CurEnt = Players->NextEnt();
	}
}

void Enemy::PhysicsSimulate(float delta)
{
	float player_walk_speed = 60.f;
	//Point towards player
	if (mTarget)
	{
		Vector2 MousePos = mTarget->GetPos();
		Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
		float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;
		GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass());

		Vector2 MoveVector;
	
		MoveVector = MoveVector.Normalize() * player_walk_speed;
		MoveVector = ToGlobal(MoveVector) - GetPos();
		ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
	}
}
