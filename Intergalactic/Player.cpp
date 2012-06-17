#include "Player.h"
#include <iostream>
#include "InputHandler.h"
#include "GameGlobals.h"

LINKCLASSTONAME("player", Player)

Player::Player(void)
{
	RegisterInput("break", Stop);
	SetPos(Vector2(1,0));
	Speed = 0;
	TankSpeed = 50.f;
	
	SetRenderType(RENDER_SPRITE);
	InitRenderer();
	PhysicsInit(DYNAMIC_BODY);
	GetRenderer()->SetDrawOrder(RENDERGROUP_PLAYER);
}

void Player::Spawn()
{
	float player_size = 30.f;
	float width = 25;
	float height = 30;

	GetRenderer()->SetTexture("ship.png", true);
	GetRenderer()->SetSize(Vector2(width * 3.4f,width * 3.4f));
	CircleShape* shape = new CircleShape;
	shape->mRadius = player_size;
	shape->mDensity = 1.f;
	GetPhysObj()->AddPhysicsShape(shape);

	GetPhysObj()->SetAngularDamping(7);
	GetPhysObj()->SetLinearDamping(10);
}

void Player::Stop(BaseObject *ent, VariantMap &Data)
{
	Player* me = dynamic_cast<Player*>(ent);
	me->GetPhysObj()->SetLinearVelocity(b2Vec2(0,0));
}

Player::~Player(void)
{

}

void Player::DefinePhysics()
{

}

void Player::Think()
{	
	float delta = gGlobals.FrameTime;

	float rot = ig::NormalizeAngle(GetAngle());

	float player_walk_speed = 50.f;
	float player_strafe_speed = 40.f;

	double x,y;
	x = sin(ig::DegToRad(rot));
	y = cos(ig::DegToRad(rot));

	Vector2 MousePos = InputHandler::GetMousePosWorld();
	Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
	SetAngle(ig::RadToDeg(-std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f);


	Vector2 MoveVector;
	if (InputHandler::IsKeyPressed(sf::Keyboard::W))
	{
		MoveVector.y = player_walk_speed;	
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
	{
		MoveVector.y = -player_walk_speed;
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::A))
	{
		MoveVector.x = -player_strafe_speed;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
	{
		MoveVector.x = player_strafe_speed;
	}
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * 100.f);
}
