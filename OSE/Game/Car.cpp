#include "Car.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"
#include "../Engine/Sound.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("car", Car)

#define MAX_ANGLE 34.f
#define MAX_THROTTLE 120.f
#define THROTTLE_SPEED 0.8f
#define TRAIL_LIFETIME 10.f

#define DEFAULT_BACK_TRACTION 0.1f
#define DEFAULT_FRONT_TRACTION 0.06f

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

	mFrontWheelTraction = DEFAULT_FRONT_TRACTION;
	mBackWheelTraction = DEFAULT_BACK_TRACTION;
	mFrontWheelSkid = false;
	mBackWheelSkid = false;

	mLine = new Line(gGlobals.RenderWindow);
	mLine->SetColour(Colour(255, 236, 139));
	mLine2 = new Line(gGlobals.RenderWindow);
	mLine2->SetColour(Colour(255, 236, 139));
	mLine3 = new Line(gGlobals.RenderWindow);
	mLine3->SetColour(Colour(255, 139, 139));
	mLine4 = new Line(gGlobals.RenderWindow);
	mLine4->SetColour(Colour(255, 139, 139));
}

Car::~Car(void)
{	
}

void Car::Spawn()
{
	GetPhysObj()->SetAngularDamping(1);
	GetPhysObj()->SetLinearDamping(1);
	
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
			mThrottle = ig::Approach(mThrottle, MAX_THROTTLE, THROTTLE_SPEED);
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

	if (GetPhysObj()->GetLinearVelocity().Length() > 100.f && mLastTrailDrop + 0.000001 < gGlobals.CurTime)
	{
		if (mFrontWheelSkid)
		{
			VariantMap data;
			data.AddData("time", (float)gGlobals.CurTime + TRAIL_LIFETIME);

			BaseObject* scorch = CreateEntity("ent_decal");
			scorch->SetModel("scorch", ig::Random(0.3f,0.4f));
			scorch->SetPos(GetPos() + GetForward() * 90.f + GetRight() * -40.f);
			scorch->Fire("dietime", data);

			scorch = CreateEntity("ent_decal");
			scorch->SetModel("scorch", ig::Random(0.3f,0.4f));
			scorch->SetPos(GetPos() + GetForward() * 90.f + GetRight() * 40.f);
			scorch->Fire("dietime", data);
			mLastTrailDrop = gGlobals.CurTime;
		}
		if (mBackWheelSkid)
		{
			VariantMap data;
			data.AddData("time", (float)gGlobals.CurTime + TRAIL_LIFETIME);

			BaseObject* scorch = CreateEntity("ent_decal");
			scorch->SetModel("scorch", ig::Random(0.3f,0.4f));
			scorch->SetPos(GetPos() + GetForward() * -90.f + GetRight() * -40.f);
			scorch->Fire("dietime", data);

			scorch = CreateEntity("ent_decal");
			scorch->SetModel("scorch", ig::Random(0.3f,0.4f));
			scorch->SetPos(GetPos() + GetForward() * -90.f + GetRight() * 40.f);
			scorch->Fire("dietime", data);
			mLastTrailDrop = gGlobals.CurTime;
		}
	}
}

void Car::PhysicsSimulate(float delta)
{
	Vector2 Vel = GetVelocity() + GetPos();
	Vector2 LocalVel = ToLocal(Vel);

	//Physically simulate front wheels
	Vector2 LocalVelFront = LocalVel + Vector2(ig::DegToRad(GetAngularVelocity()) * 90.f,0.f);

	mLine->mVerts[0] = GetPos() + GetForward() * 90.f;
	Vector2 vel = LocalVelFront / 5.f;
	mLine->mVerts[1] = (GetPos() + GetForward() * 90.f) + (ToGlobal(vel) - GetPos());

	LocalVelFront = LocalVelFront.Rotate(mWheelAngle); //Rotate the local vel according to the wheels
	if (std::abs(LocalVelFront.x) > 500)
	{
		mFrontWheelTraction = ig::Approach(mFrontWheelTraction, 0.02f, 0.0003f);
		mFrontWheelSkid = true;
	}
	else
	{
		mFrontWheelTraction = ig::Approach(mFrontWheelTraction, DEFAULT_FRONT_TRACTION, 0.001f);
		mFrontWheelSkid = false;
	}

	Vector2 FrontForce;
	FrontForce.x = -LocalVelFront.x * mFrontWheelTraction;


//	FrontForce = FrontForce.Rotate(mWheelAngle);
	FrontForce = ToGlobal(FrontForce) - GetPos();

	mLine3->mVerts[0] = GetPos() + GetForward() * 90.f;
	mLine3->mVerts[1] = (GetPos() + GetForward() * 90.f) + FrontForce;

	ApplyForce(FrontForce * GetPhysObj()->GetMass(), GetPos() + GetForward() * 90.f);

	//Simulate back wheels
	Vector2 BackForce;
	Vector2 LocalVelBack = LocalVel - Vector2(ig::DegToRad(GetAngularVelocity()) * 90.f,0.f);


	BackForce.x = -LocalVelBack.x * mBackWheelTraction;
	BackForce.y = mThrottle * mBackWheelTraction * 2.f;

	if (InputHandler::IsKeyPressed(sf::Keyboard::Space) || std::fabs(LocalVelBack.x) > 300) //HandBrake
	{
		mBackWheelSkid = true;
		mBackWheelTraction = ig::Approach(mBackWheelTraction, 0.02f, 0.003f);
	}
	else
	{
		mBackWheelSkid = false;
		mBackWheelTraction = ig::Approach(mBackWheelTraction, DEFAULT_BACK_TRACTION, 0.02f);
	}

	BackForce = ToGlobal(BackForce) - GetPos();

	mLine4->mVerts[0] = GetPos() + GetForward() * -90.f;
	mLine4->mVerts[1] = (GetPos() + GetForward() * -90.f) + BackForce;

	mLine2->mVerts[0] = GetPos() + GetForward() * -90.f;
	Vector2 velBack = LocalVelBack / 5.f;
	mLine2->mVerts[1] = (GetPos() + GetForward() * -90.f) + (ToGlobal(velBack) - GetPos());

	ApplyForce(BackForce * GetPhysObj()->GetMass(), GetPos() + GetForward() * -90.f);
}