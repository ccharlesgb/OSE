#include "Player.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"

#define USE_RANGE 50.f

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
}

void Player::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
	SetModel("player2", 0.28);
	SetOrigin(Vector2(0,45));
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
	float player_walk_speed = 50.f;
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
		Vector2 orig_offset = Vector2((float)std::sin(gGlobals.CurTime * 6) * 5.f, (float)std::cos(gGlobals.CurTime * 6) * 5.f);
		SetOrigin(Vector2(0,45) + orig_offset);
	}
	MoveVector = MoveVector * player_walk_speed;
	MoveVector = ToGlobal(MoveVector) - GetPos();
	ApplyForceCenter(MoveVector * GetPhysObj()->GetMass());
}
