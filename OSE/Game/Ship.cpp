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
	
	SetModel("Ship", 0.4);
	//SetOrigin(Vector2(0,15));
	PhysicsHullFromModel();
	
	InUse = false;
}

void Ship::OnDelete()
{
	BaseObject* Player = CreateEntity("player");
	Player->SetPos(GetPos());
	Player->Spawn();
	gGlobals.Player = Player;
	sCamera::FollowEntity(Player);
}

void Ship::StartTouch(CollisionInfo* info)
{
	if (info->OtherEnt->GetClassName() == "player")
	{
		sCamera::FollowEntity(this);
		info->OtherEnt->Delete();
		InUse = true;
	}
}

void Ship::Think()
{
	if (!InUse) { return; }
	
	//Player movement code
	float player_walk_speed = 70.f;
	
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
	if (InputHandler::IsKeyPressed(sf::Keyboard::A))
	{
		MoveVector.x = -0.7;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
	{
		MoveVector.x = 0.7;
	}
	MoveVector = MoveVector.Normalize() * player_walk_speed;
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
}