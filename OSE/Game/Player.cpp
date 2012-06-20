#include "Player.h"
#include <iostream>
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("player", Player)

Player::Player(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
}

void Player::Spawn()
{
	float player_size = 30.f;
	float width = 25;
	float height = 30;

	//Declare the physics object
	//CircleShape* shape = new CircleShape;
	//shape->mRadius = player_size;
	//shape->mDensity = 1.f;
	//GetPhysObj()->AddPhysicsShape(shape); //Add it to our physics object (They support multipe shapes!)

	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);

	SetModel("player");
	SetOrigin(Vector2(0,15));
	PhysicsHullFromModel();
}

Player::~Player(void)
{

}

void Player::DefinePhysics()
{

}

void Player::Think()
{	
	//Player movement code
	float rot = ig::NormalizeAngle(GetAngle());

	float player_walk_speed = 70.f;
	float player_strafe_speed = 40.f;

	double x,y;
	x = sin(ig::DegToRad(rot));
	y = cos(ig::DegToRad(rot));

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
