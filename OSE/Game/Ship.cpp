#include "Ship.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("ship", Ship)

#define MAX_ANGLE 20.f
#define MAX_THROTTLE 50.f

Ship::Ship(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	mWheelAngle = 0.f;
	mThrottle = 0.f;
}

Ship::~Ship(void)
{	
}

void Ship::Spawn()
{
	GetPhysObj()->SetAngularDamping(10);
	GetPhysObj()->SetLinearDamping(3);
	
	SetModel("car", 1.f);
	//SetOrigin(Vector2(0,15));
	PhysicsHullFromModel();
	mDriver = NULL;
	CreateSound("enter", "engine_start");
	CreateSound("idle", "engine_idle");
}

void Ship::OnDelete()
{
	std::cout << "OnDelete" << "\n";
	if (InUse())
		Exit(GetPos());
}

void Ship::StartTouch(CollisionInfo* info)
{

}

void Ship::Use(BaseObject *ply)
{
	sCamera::FollowEntity(this);
	mDriver = ply;
	mDriver->SetNoDraw(true);
	mEnterTime = gGlobals.CurTime;
	EmitSound("enter");
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
	//Car Movement
	float player_walk_speed = 150.f;
	float steer_factor = GetPhysObj()->GetLinearVelocity().Length() / 1000.f;
	Vector2 MoveVector;

	if (InUse()) //What is our driver pressing?
	{
		if (InputHandler::IsKeyPressed(sf::Keyboard::E) && mEnterTime + 0.5f < gGlobals.CurTime)
		{
			Exit(GetPos() + Vector2(-20, 0));
			return;
		}
		mDriver->SetPos(GetPos() + GetForward() * -160.f);
		mDriver->SetAngle(GetAngle());
	
		if (InputHandler::IsKeyPressed(sf::Keyboard::W))
		{
			mThrottle = ig::Approach(mThrottle, MAX_THROTTLE, 1);
			EmitSound("idle");
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
	}
	else
	{
		mThrottle = ig::Approach(mThrottle, 0, 5);
		mWheelAngle = ig::Approach(mWheelAngle, 0, steer_factor);
	}

	//Physically simular wheels
	MoveVector.y = mThrottle;
	MoveVector = MoveVector.Rotate(-mWheelAngle);
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForce(MoveVector * GetPhysObj()->GetMass(), GetPos() + GetForward() * 70.f);

	Vector2 BackFric;
	Vector2 pos = GetVelocity() + GetPos();
	BackFric.x = ToLocal(pos).x;
	BackFric.x = BackFric.x + (GetAngularVelocity() * -1.f);
	BackFric = ToGlobal(BackFric) - GetPos();
	ApplyForce(BackFric * -200.f, GetPos() + GetForward() * -70.f);

	if (GetPhysObj()->GetLinearVelocity().Length() > 200.f)
	{
		//BaseObject* scorch = CreateEntity("ent_decal");
		//scorch->SetModel("scorch", ig::Random(0.5,0.7));
		//scorch->SetPos(GetPos());
		//scorch->Spawn();
	}
}