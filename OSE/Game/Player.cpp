#include "Player.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"

#define USE_RANGE 50.f

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("player", Player)

Player::Player(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	mWeapon = CreateEntity("weapon_pistol");
	mWeapon->SetOwner(this);
	mWeapon->SetPos(GetPos());
	mWeapon->SetAngle(GetAngle());
	mWeapon->Spawn();
	mNextUse = 0.f;
}

void Player::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
	std::cout << "SPAWN\n";
	SetModel("player");
	SetOrigin(Vector2(0,15));
	PhysicsHullFromModel();
}

Player::~Player(void)
{
}

void Player::Think()
{	
	//Player movement code
	float player_walk_speed = 60.f;

	//Point player towards mouse
	if (!InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		Vector2 MousePos = InputHandler::GetMousePosWorld();
		Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
		float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;
		GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass());
	}

	Vector2 MoveVector;
	if (InputHandler::IsKeyPressed(sf::Keyboard::W))
	{
		MoveVector.y = 1;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
	{
		MoveVector.y = -1;
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::A))
	{
		MoveVector.x = -0.7f;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
	{
		MoveVector.x = 0.7f;
	}
	MoveVector = MoveVector.Normalize() * player_walk_speed;
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());

	if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left) && !InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		mWeapon->SetPos(GetPos() + GetForward() * 50.f);
		mWeapon->SetAngle(GetAngle());
		VariantMap dat;
		mWeapon->Fire("fire1", dat);
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
}
