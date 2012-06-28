#include "Car.h"

#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/Camera.h"
#include "../Engine/Sound.h"

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("car", Car)

#define MAX_ANGLE 34.f
#define STEER_SPEED 70.f
#define MAX_THROTTLE 80.f
#define THROTTLE_INCREASE_SPEED 10.f
#define THROTTLE_DECREASE_SPEED 50.f
#define THROTTLE_DIE_SPEED 25.f
#define TRAIL_LIFETIME 10.f

#define FRONT_SKID_THRESHOLD 380.f
#define BACK_SKID_THRESHOLD 200.f

#define DEFAULT_BACK_TRACTION 0.08f
#define DEFAULT_FRONT_TRACTION 0.06f

#define BACK_SKID_SPEED 0.1f
#define BACK_RESTORE_TRACTION_SPEED 0.1f
#define FRONT_SKID_SPEED 0.1f
#define FRONT_RESTORE_TRACTION_SPEED 0.1f

Car::Car(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_ENTITIES);
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
	CreateSound("skid", "tires_squeal_loop");
	GetSound("skid")->SetLoop(true);
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

	Vector2_Rect AABB = GetAABB();

	mLine->mVerts[0] = AABB.Position;
	mLine->mVerts[1] = AABB.Position + AABB.Size;

	//mLine->Draw();
	//mLine2->Draw();
	//mLine3->Draw();
	//mLine4->Draw();
}

void Car::OnDelete()
{
	if (InUse())
		Exit(GetPos());
}

void Car::StartTouch(CollisionInfo* info)
{
	if (info->Speed > 500)
	{
		DamageInfo d_info;
		d_info.Inflictor = info->OtherEnt;
		d_info.Amount = info->Speed / 10;
		d_info.type = DAMAGETYPE_PHYSICS;
		info->OtherEnt->TakeDamage(d_info);
	}
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

	//Controls
	float steer_restore_factor = LocalVel.y * 0.1f;
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
			mThrottle = ig::Approach(mThrottle, MAX_THROTTLE, THROTTLE_INCREASE_SPEED * delta);
		}
		else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
		{
			mThrottle = ig::Approach(mThrottle, -MAX_THROTTLE / 3.f, THROTTLE_DECREASE_SPEED  * delta);
		}
		else
			mThrottle = ig::Approach(mThrottle, 0.f, THROTTLE_DIE_SPEED * delta);
		GetSound("idle")->SetPitch(0.7f + (std::fabs(mThrottle / MAX_THROTTLE) / 2.f));

		if (InputHandler::IsKeyPressed(sf::Keyboard::A))
		{
			mWheelAngle = ig::Approach(mWheelAngle, -MAX_ANGLE, STEER_SPEED * delta);
		}
		else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
		{
			mWheelAngle = ig::Approach(mWheelAngle, MAX_ANGLE, STEER_SPEED * delta);
		}
		else
			mWheelAngle = ig::Approach(mWheelAngle, 0, steer_restore_factor * delta);
	}
	else
	{
		mThrottle = ig::Approach(mThrottle, 0, THROTTLE_DIE_SPEED * 10.f * delta);
		mWheelAngle = ig::Approach(mWheelAngle, 0, steer_restore_factor * delta);
	}

	//Physically simulate front wheels
	Vector2 LocalVelFront = LocalVel + Vector2(ig::DegToRad(GetAngularVelocity()) * 90.f,0.f);

	LocalVelFront = LocalVelFront.Rotate(mWheelAngle); //Rotate the local vel according to the wheels
	if (std::abs(LocalVelFront.x) > FRONT_SKID_THRESHOLD)
	{
		if (!mFrontWheelSkid && !mBackWheelSkid)
			GetSound("skid")->Play();
		mFrontWheelTraction = ig::Approach(mFrontWheelTraction, 0.03f, FRONT_SKID_SPEED * delta);
		mFrontWheelSkid = true;
	}
	else
	{
		mFrontWheelTraction = ig::Approach(mFrontWheelTraction, DEFAULT_FRONT_TRACTION, FRONT_RESTORE_TRACTION_SPEED * delta);
		mFrontWheelSkid = false;
		if (!mBackWheelSkid)
			GetSound("skid")->Stop();
	}

	Vector2 FrontForce;
	FrontForce.x = -LocalVelFront.x * mFrontWheelTraction;


	FrontForce = FrontForce.Rotate(-mWheelAngle);
	FrontForce = ToGlobal(FrontForce) - GetPos();

	mLine3->mVerts[0] = GetPos() + GetForward() * 90.f;
	mLine3->mVerts[1] = (GetPos() + GetForward() * 90.f) + FrontForce * 2.f;

	ApplyForce(FrontForce * GetPhysObj()->GetMass(), GetPos() + GetForward() * 90.f);

	//Simulate back wheels
	Vector2 BackForce;
	Vector2 LocalVelBack = LocalVel - Vector2(ig::DegToRad(GetAngularVelocity()) * 90.f,0.f);

	BackForce.x = -LocalVelBack.x * mBackWheelTraction;
	BackForce.y = mThrottle * mBackWheelTraction * 6.f;

	if ((InUse() && InputHandler::IsKeyPressed(sf::Keyboard::Space)) || std::fabs(LocalVelBack.x) > BACK_SKID_THRESHOLD) //HandBrake
	{
		if (!mFrontWheelSkid && !mBackWheelSkid)
			GetSound("skid")->Play();
		mBackWheelSkid = true;
		mBackWheelTraction = ig::Approach(mBackWheelTraction, 0.03f, BACK_SKID_SPEED * delta);
	}
	else
	{
		mBackWheelSkid = false;
		mBackWheelTraction = ig::Approach(mBackWheelTraction, DEFAULT_BACK_TRACTION, BACK_RESTORE_TRACTION_SPEED * delta);
		if (!mFrontWheelSkid)
			GetSound("skid")->Stop();
	}

	BackForce = ToGlobal(BackForce) - GetPos();

	mLine4->mVerts[0] = GetPos() + GetForward() * -90.f;
	mLine4->mVerts[1] = (GetPos() + GetForward() * -90.f) + BackForce * 2.f;

	ApplyForce(BackForce * GetPhysObj()->GetMass(), GetPos() + GetForward() * -90.f);

	//Air Resistance
	float Drag = (LocalVel.y * LocalVel.y) * 0.00001f;
	Vector2 vec = Vector2(0.f, -Drag);
	ApplyForceCenter((ToGlobal(vec) - GetPos()) * GetPhysObj()->GetMass());
}