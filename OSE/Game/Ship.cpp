#include "Ship.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("ship", Ship)

Ship::Ship(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
}

Ship::~Ship(void)
{	
}

void Ship::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
	
	SetModel("Ship", 0.3);
	//SetOrigin(Vector2(0,15));
	PhysicsHullFromModel();
	
	mDriver = NULL;
}

void Ship::OnDelete()
{
	std::cout << "OnDelete" << "\n";
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
	
	Vector2 MousePos = InputHandler::GetMousePosWorld();
	Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
	float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;
	GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass());
	//SetAngle(GetPhysObj()->GetAngle());
	//SetAngle(TargetAngle);
	
	Vector2 MoveVector;
	if (InputHandler::IsKeyPressed(sf::Keyboard::W))
	{
		MoveVector.y = 1;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
	{
		MoveVector.y = -1;
	}
	MoveVector = MoveVector.Normalize() * player_walk_speed;
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
}