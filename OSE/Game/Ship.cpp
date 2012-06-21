#include "Ship.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("ship", Ship)

#define MAX_ANGLE 25.f
#define MAX_THROTTLE 75.f

Ship::Ship(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	mWheelAngle = 0.f;
}

Ship::~Ship(void)
{	
}

void Ship::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(6);
	
	SetModel("car", 1.f);
	//SetOrigin(Vector2(0,15));
	PhysicsHullFromModel();
	
	mDriver = NULL;
}

void Ship::OnDelete()
{
	std::cout << "OnDelete" << "\n";
	if (InUse())
		Exit(GetPos());
}

void Ship::StartTouch(CollisionInfo* info)
{
	if (info->OtherEnt->GetClassName() == "player")
	{
		std::cout << "clicked";
		sCamera::FollowEntity(this);
		mDriver = info->OtherEnt;
		mDriver->SetNoDraw(true);
	}
}

void Ship::Exit(Vector2 position)
{
	mDriver->SetPos(position);
	mDriver->SetNoDraw(false);
	sCamera::FollowEntity(mDriver);
	mDriver = NULL;
}

void Ship::Think()
{
	if (!InUse()) { return; }
	
	if (InputHandler::IsKeyPressed(sf::Keyboard::E))
	{
		Exit(GetPos() + Vector2(-20, 0));
	}
	
	//Player movement code
	float player_walk_speed = 150.f;
	float steer_factor = GetPhysObj()->GetLinearVelocity().Length() / 1000.f;
	Vector2 MoveVector;
	if (InputHandler::IsKeyPressed(sf::Keyboard::W))
	{
		mThrottle = ig::Approach(mThrottle, MAX_THROTTLE, 1);
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
	{
		mThrottle = ig::Approach(mThrottle, -MAX_THROTTLE / 3.f, 1.5);
	}
	else
		mThrottle = ig::Approach(mThrottle, 0, 0.3);
	if (InputHandler::IsKeyPressed(sf::Keyboard::A))
	{
		mWheelAngle = ig::Approach(mWheelAngle, -MAX_ANGLE, 1);
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
	{
		mWheelAngle = ig::Approach(mWheelAngle, MAX_ANGLE, 1);
	}
	else
		mWheelAngle = ig::Approach(mWheelAngle, 0, steer_factor);
	MoveVector.y = mThrottle;
	//MoveVector.x = mWheelAngle * mThrottle;
	MoveVector = MoveVector.Rotate(-mWheelAngle);
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForce(MoveVector * GetPhysObj()->GetMass(), GetPos() + GetForward() * 70.f);

	Vector2 BackFric;
	BackFric.x = ToLocal(GetVelocity() + GetPos()).x;
	BackFric = ToGlobal(BackFric) - GetPos();
	//ApplyForce(BackFric * -50.f, GetPos() + GetForward() * -70.f);

	if (GetPhysObj()->GetLinearVelocity().Length() > 200.f)
	{
		//BaseObject* scorch = CreateEntity("ent_decal");
		//scorch->SetModel("scorch", ig::Random(0.5,0.7));
		//scorch->SetPos(GetPos());
		//scorch->Spawn();
	}
}