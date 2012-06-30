#include "Player.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/Render/Sprite.h"

#define USE_RANGE 50.f
#define DEFAULT_WALK_SPEED 50.f
#define HEALTH_REGEN_TIME 5.f

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("player", Player)

Player::Player(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	GiveWeapon(CreateEntity("weapon_pistol"));
	mNextUse = 0.f;
	SetWalkSpeed(DEFAULT_WALK_SPEED);
	mLastTakeDamage = 0.f;
}

void Player::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
	SetModel("npc/player", 0.5f);
	CreateHead("player_head");
	SetOrigin(Vector2(0,30));
	PhysicsHullFromModel();
}

Player::~Player(void)
{
}

void Player::GiveWeapon(BaseObject* ent)
{
	ent->SetOwner(this);
	Vector2 shoot_pos = GetPos() + GetForward() * 40.f + GetRight() * 10.f;
	ent->SetPos(shoot_pos);
	ent->SetAngle(GetAngle());
	ent->SetParent(this);
	mWeapons.Append(ent);
	mActiveWeapon = dynamic_cast<weapon_pistol*>(ent);
}

void Player::TakeDamage(const DamageInfo &info)
{
	SetHealth(GetHealth() - info.Amount);
	SetWalkSpeed(ig::Approach(GetWalkSpeed(), 20.f, -15.f));
	mLastTakeDamage = gGlobals.CurTime;
}

void Player::ChooseWeapon(const char* name)
{
	EntityList<BaseObject*>::iter CurEnt = mWeapons.FirstEnt();
	while (CurEnt != mWeapons.End())
	{
		if ((*CurEnt)->GetClassName() == name)
		{
			mActiveWeapon = dynamic_cast<weapon_pistol*>(*CurEnt);
			break;
		}
		CurEnt = mWeapons.NextEnt(CurEnt);
	}
}

void Player::Think()
{	
	if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left) && !InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		VariantMap dat;
		mActiveWeapon->Fire("fire1", dat);
	}

	if (mLastTakeDamage + HEALTH_REGEN_TIME < gGlobals.CurTime)
	{
		SetHealth(ig::Approach(GetHealth(), GetMaxHealth(), 0.1f));
	}
	if (mLastTakeDamage + 0.5f < gGlobals.CurTime)
	{
		SetWalkSpeed(ig::Approach(GetWalkSpeed(), DEFAULT_WALK_SPEED, 0.5f));
	}

	if (InputHandler::IsKeyPressed(sf::Keyboard::E) && mNextUse < gGlobals.CurTime)
	{
		TraceInfo info;
		info.mStartPoint = GetPos();
		info.mEndPoint = GetPos() + (GetForward() * USE_RANGE);
		TraceResult TraceRes;
		PhysicsQueries::TraceLine(info, &TraceRes);
		if (TraceRes.mHitEnt != NULL && TraceRes.mHitEnt != this)
		{
			TraceRes.mHitEnt->Use(this);
		}
		mNextUse = gGlobals.CurTime + USE_DELAY;
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::R))
	{
		mActiveWeapon->Reload();
	}
}

void Player::PhysicsSimulate(float delta)
{
	//Point player towards mouse
	Vector2 MousePos = InputHandler::GetMousePosWorld();
	Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
	float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;
	SetHeadAngle(TargetAngle - GetAngle());
	if (!InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass());
	}

	Vector2 MoveVector;
	if (InputHandler::IsKeyPressed(sf::Keyboard::W))
	{
		MoveVector.y = 1;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
	{
		MoveVector.y = -0.8f;
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::A))
	{
		MoveVector.x = -0.7f;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
	{
		MoveVector.x = 0.7f;
	}
	if (MoveVector.Length() > 1.f)
		MoveVector = MoveVector.Normalize();
	if (MoveVector.Length() > 0.f)
	{
		if (!GetCurrentAnimation() || (GetCurrentAnimation()->mName != "walk"))
			PlayAnimation("walk", true);
	}
	else
	{
		PlayAnimation("idle", false);
	}
	MoveVector = MoveVector * GetWalkSpeed();
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
}
