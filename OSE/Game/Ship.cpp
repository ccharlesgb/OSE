#include "Ship.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"
#include "../Engine/Sound.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("ship", Ship)

#define MAX_ANGLE 20.f
#define MAX_THROTTLE 50.f
#define TRAIL_LIFETIME 10.f

Ship::Ship(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	mWheelAngle = 0.f;
	mThrottle = 0.f;
	mWheelSprite = new Sprite(gGlobals.RenderWindow);
	mWheelSprite->SetTexture("car_wheel");
	mWheelSprite->SetOrigin(Vector2());
	mWheelSprite->SetScale(0.4f);

	mLastTrailDrop = gGlobals.CurTime;

	mLine = new Line(gGlobals.RenderWindow);
	mLine->SetColour(Colour(255, 236, 139));
}

Ship::~Ship(void)
{	
}

void Ship::Spawn()
{
	GetPhysObj()->SetAngularDamping(10);
	GetPhysObj()->SetLinearDamping(3);
	
	SetModel("car", 1.f);
	PhysicsHullFromModel();
	mDriver = NULL;
	CreateSound("enter", "engine_start");
	CreateSound("idle", "engine_idle");
}

void Ship::Draw()
{

	mWheelSprite->SetAngle(GetAngle() - mWheelAngle);
	Vector2 FL_Pos, FR_Pos;
	FL_Pos = (GetForward() * 80.f) + (GetRight() * -43.f);
	FR_Pos = (GetForward() * 80.f) + (GetRight() * 43.f);
	mWheelSprite->SetPosition(GetPos() + FL_Pos);
	mWheelSprite->Draw();

	mWheelSprite->SetPosition(GetPos() + FR_Pos);
	mWheelSprite->Draw();

	DrawModel();

	mLine->Draw();
}

void Ship::OnDelete()
{
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
	mEnterTime = (float)gGlobals.CurTime;
	//EmitSound("enter");
	GetSound("idle")->SetLoop(true);
	GetSound("idle")->Play();
}


void Ship::Exit(Vector2 position)
{
	mDriver->SetPos(position);
	mDriver->SetNoDraw(false);
	sCamera::FollowEntity(mDriver);
	mDriver = NULL;
	GetSound("idle")->Stop();
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
		}
		else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
		{
			mThrottle = ig::Approach(mThrottle, -MAX_THROTTLE / 3.f, 1.5);
		}
		else
			mThrottle = ig::Approach(mThrottle, 0.f, 0.3f);
		GetSound("idle")->SetPitch(0.7f + (std::fabs(mThrottle / MAX_THROTTLE) / 2.f));

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

	Vector2 Vel = GetVelocity() + GetPos();
	Vector2 LocalVel = ToLocal(Vel);

	//Physically simular wheels
	MoveVector.x = LocalVel.y * mWheelAngle * 0.001f; //Times traction?
	MoveVector = MoveVector.Rotate(-mWheelAngle);
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForce(MoveVector * GetPhysObj()->GetMass(), GetPos() + GetForward() * 90.f);

	mLine->mVerts[0] = GetPos() + GetForward() * 90.f;
	mLine->mVerts[1] = (GetPos() + GetForward() * 90.f) + (MoveVector * 10.f);

	Vector2 BackFric;
	Vector2 pos = GetVelocity() + GetPos();
	BackFric.y = mThrottle;
	BackFric.x = -LocalVel.x;
	BackFric.x = BackFric.x + (ig::DegToRad(GetAngularVelocity()) * 45.f);
	BackFric.x *= 0.03f;
	//mLine->mVerts[0] = GetPos() + GetForward() * -90.f;
	//mLine->mVerts[1] = (GetPos() + GetForward() * -90.f) + ((ToGlobal(Vector2(BackFric.x,0.f)) - GetPos()) * 10.f);

	if (InputHandler::IsKeyPressed(sf::Keyboard::Space))
	{
		BackFric.x = 0.f;
	}
	BackFric = ToGlobal(BackFric) - GetPos();


	ApplyForce(BackFric * GetPhysObj()->GetMass(), GetPos() + GetForward() * -90.f);

	if (GetPhysObj()->GetLinearVelocity().Length() > 500.f && mLastTrailDrop + 0.1 < gGlobals.CurTime)
	{
		VariantMap data;
		data.AddData("time", (float)gGlobals.CurTime + TRAIL_LIFETIME);

		/*
		BaseObject* scorch = CreateEntity("ent_decal"); //DEBUGGING VEHICLE PHYSICS
		scorch->SetModel("scorch", 0.2f);
		scorch->SetColour(Colour(255,0,0));
		scorch->SetPos(GetPos() + GetForward() * 90.f);
		scorch->Fire("dietime", data);

		scorch = CreateEntity("ent_decal");
		scorch->SetModel("scorch", 0.2f);
		scorch->SetPos(GetPos() + GetForward() * -90.f);
		scorch->Fire("dietime", data);
		*/

		BaseObject* scorch = CreateEntity("ent_decal");
		scorch->SetModel("scorch", ig::Random(0.8f,1.f));
		scorch->SetPos(GetPos() + GetForward() * -90.f);
		scorch->Fire("dietime", data);
		mLastTrailDrop = gGlobals.CurTime;
	}
}