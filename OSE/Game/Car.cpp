#include "Car.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"
#include "../Engine/Sound.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("car", Car)

#define MAX_ANGLE 20.f
#define MAX_THROTTLE 50.f
#define TRAIL_LIFETIME 10.f

Car::Car(void)
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
	mLine2 = new Line(gGlobals.RenderWindow);
	mLine2->SetColour(Colour(255, 236, 139));
	mLine3 = new Line(gGlobals.RenderWindow);
	mLine3->SetColour(Colour(255, 236, 139));
	mLine4 = new Line(gGlobals.RenderWindow);
	mLine4->SetColour(Colour(255, 236, 139));
}

Car::~Car(void)
{	
}

void Car::Spawn()
{
	GetPhysObj()->SetAngularDamping(10);
	GetPhysObj()->SetLinearDamping(3);
	
	SetModel("car", 1.f);
	PhysicsHullFromModel();
	mDriver = NULL;
	CreateSound("enter", "engine_start");
	CreateSound("idle", "engine_idle");
}

void Car::Draw()
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
	mLine2->Draw();
	mLine3->Draw();
	mLine4->Draw();
}

void Car::OnDelete()
{
	if (InUse())
		Exit(GetPos());
}

void Car::StartTouch(CollisionInfo* info)
{

}

void Car::Use(BaseObject *ply)
{
	sCamera::FollowEntity(this);
	mDriver = ply;
	mDriver->SetNoDraw(true);
	mEnterTime = (float)gGlobals.CurTime;
	//EmitSound("enter");
	GetSound("idle")->SetLoop(true);
	GetSound("idle")->Play();
}


void Car::Exit(Vector2 position)
{
	mDriver->SetPos(position);
	mDriver->SetNoDraw(false);
	sCamera::FollowEntity(mDriver);
	mDriver = NULL;
	GetSound("idle")->Stop();
}

void Car::Think()
{
	//Car Movement
	float player_walk_speed = 150.f;
	float steer_factor = GetPhysObj()->GetLinearVelocity().Length() / 1000.f;

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

void Car::PhysicsSimulate(float delta)
{
	Vector2 Vel = GetVelocity() + GetPos();
	Vector2 LocalVel = ToLocal(Vel);

	//Physically simulate front wheels
	Vector2 LocalVelFront = LocalVel + Vector2(ig::DegToRad(GetAngularVelocity()) * 90.f,0.f);
	Vector2 FrontForce;

	mLine->mVerts[0] = GetPos() + GetForward() * 90.f;
	mLine->mVerts[1] = (GetPos() + GetForward() * 90.f) + (LocalVelFront * 10.f);


	FrontForce = ToGlobal(FrontForce) - GetPos();
	ApplyForce(FrontForce * GetPhysObj()->GetMass(), GetPos() + GetForward() * 90.f);

	//Simulate back wheels
	Vector2 BackForce;

	if (InputHandler::IsKeyPressed(sf::Keyboard::Space)) //HandBrake
	{
		BackForce.x = 0.f;
	}

	BackForce = ToGlobal(BackForce) - GetPos();
	Vector2 LocalVelBack = LocalVel - Vector2(ig::DegToRad(GetAngularVelocity()) * 90.f,0.f);


	mLine->mVerts[0] = GetPos() + GetForward() * -90.f;
	mLine->mVerts[1] = (GetPos() + GetForward() * -90.f) + (LocalVelBack * 10.f);

	ApplyForce(BackForce * GetPhysObj()->GetMass(), GetPos() + GetForward() * -90.f);
}