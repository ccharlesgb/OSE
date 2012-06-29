#include "Player.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"

#define USE_RANGE 50.f
#define DEFAULT_WALK_SPEED 50.f
#define HEALTH_REGEN_TIME 5.f

//This function registers the entity to the EntityCreator.
//"player" is the classname. Player is the coded classname
LINKCLASSTONAME("player", Player)

Player::Player(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	GiveWeapon(CreateEntity("weapon_pistol"));
	mNextUse = 0.f;
	mText = new Text(gGlobals.RenderWindow);
	mText->SetFont("LCDM2N");
	mText->SetText("Test");
	mText->SetPosition(GetPos());
	SetWalkSpeed(DEFAULT_WALK_SPEED);
	mLastTakeDamage = 0.f;

	mCurFrame = 1;
	mLastFrameChange = 0.f;

	mSequence[0] = 0;
	mSequence[1] = 1;
	mSequence[2] = 2;
	mSequence[3] = 1;
	mSequence[4] = 0;
	mSequence[5] = 3;
	mSequence[6] = 4;
	mSequence[7] = 3;
}

void Player::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
	SetModel("player_walk", 0.54);
	SetOrigin(Vector2(0,22));
	PhysicsHullFromModel();
}

Player::~Player(void)
{
}

void Player::Draw()
{
	DrawModel();
	//mText->Draw();
}

void Player::GiveWeapon(BaseObject* ent)
{
	ent->SetOwner(this);
	Vector2 shoot_pos = GetPos() + GetForward() * 40.f + GetRight() * 10.f;
	ent->SetPos(shoot_pos);
	ent->SetAngle(GetAngle());
	ent->SetParent(this);
	mWeapons.Append(ent);
	mActiveWeapon = dynamic_cast<weapon_pistol*>(ent);
}

void Player::TakeDamage(const DamageInfo &info)
{
	SetHealth(GetHealth() - info.Amount);
	SetWalkSpeed(ig::Approach(GetWalkSpeed(), 20.f, -15.f));
	mLastTakeDamage = gGlobals.CurTime;
}

void Player::ChooseWeapon(const char* name)
{
	EntityList<BaseObject*>::iter CurEnt = mWeapons.FirstEnt();
	while (CurEnt != mWeapons.End())
	{
		if ((*CurEnt)->GetClassName() == name)
		{
			mActiveWeapon = dynamic_cast<weapon_pistol*>(*CurEnt);
			break;
		}
		CurEnt = mWeapons.NextEnt(CurEnt);
	}
}

void Player::Think()
{	
	if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left) && !InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		VariantMap dat;
		mActiveWeapon->Fire("fire1", dat);
	}

	if (mLastTakeDamage + HEALTH_REGEN_TIME < gGlobals.CurTime)
	{
		SetHealth(ig::Approach(GetHealth(), GetMaxHealth(), 0.1f));
	}
	if (mLastTakeDamage + 0.5f < gGlobals.CurTime)
	{
		SetWalkSpeed(ig::Approach(GetWalkSpeed(), DEFAULT_WALK_SPEED, 0.5f));
	}

	if (InputHandler::IsKeyPressed(sf::Keyboard::E) && mNextUse < gGlobals.CurTime)
	{
		TraceInfo info;
		info.mStartPoint = GetPos();
		info.mEndPoint = GetPos() + (GetForward() * USE_RANGE);
		TraceResult TraceRes;
		PhysicsQueries::TraceLine(info, &TraceRes);
		if (TraceRes.mHitEnt != NULL && TraceRes.mHitEnt != this)
		{
			TraceRes.mHitEnt->Use(this);
		}
		mNextUse = gGlobals.CurTime + USE_DELAY;
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::R))
	{
		mActiveWeapon->Reload();
	}
}

void Player::PhysicsSimulate(float delta)
{
	//Player movement code
	mText->SetPosition(GetPos());
	//Point player towards mouse
	if (!InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		Vector2 MousePos = InputHandler::GetMousePosWorld();
		Vector2 MouseDirHat = (MousePos - GetPos()).Normalize();
		float TargetAngle = ig::RadToDeg(std::atan2(MouseDirHat.y, MouseDirHat.x)) - 90.f;
		GetPhysObj()->ApplyTorque(ig::NormalizeAngle(TargetAngle - GetAngle()) * GetPhysObj()->GetMass());
	}

	Vector2 MoveVector;
	if (InputHandler::IsKeyPressed(sf::Keyboard::W))
	{
		MoveVector.y = 1;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::S))
	{
		MoveVector.y = -0.8f;
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::A))
	{
		MoveVector.x = -0.7f;
	}
	else if (InputHandler::IsKeyPressed(sf::Keyboard::D))
	{
		MoveVector.x = 0.7f;
	}
	if (MoveVector.Length() > 1.f)
		MoveVector = MoveVector.Normalize();
	if (MoveVector.Length() > 0)
	{
		//TEMP ANIM CODE
		if (mLastFrameChange + (1/12.f) < gGlobals.CurTime)
		{
			sf::IntRect rect;
			rect.height = 128;
			rect.width = 128;
			rect.top = 0;
			rect.left = mSequence[mCurFrame] * 128;
			mSprite->SetTextureRect(rect);
			mCurFrame++;
			mLastFrameChange = gGlobals.CurTime;
		}
		if (mCurFrame > 7)
			mCurFrame = 0;
	}
	MoveVector = MoveVector * GetWalkSpeed();
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
}
