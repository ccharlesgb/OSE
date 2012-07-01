#include "BaseHuman.h"
#include "../Engine/InputHandler.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/Render/Sprite.h"

#define USE_RANGE 50.f
#define DEFAULT_WALK_SPEED 50.f
#define HEALTH_REGEN_TIME 5.f

BaseHuman::BaseHuman(void)
{
	SetPos(Vector2(0,0));
	RenderInit();
	SetDrawOrder(RENDERGROUP_PLAYER);
	PhysicsInit(DYNAMIC_BODY);
	SetWalkSpeed(DEFAULT_WALK_SPEED);

	mHeadAngle = 0.f;
	
	mHead = NULL;
	mFeet = NULL;
}

void BaseHuman::CreateHead(const char* texture_path)
{
	mHead = new Sprite(gGlobals.RenderWindow);
	mHead->SetAngle(GetAngle());
	mHead->SetPosition(GetPos());
	mHead->SetTexture(texture_path);
	mHead->SetOrigin(Vector2(0,60));
	mHead->SetScale(0.27f);
}

void BaseHuman::CreateFeet(const char* texture_path)
{
	mFeet = new Sprite(gGlobals.RenderWindow);
	mFeet->SetAngle(GetAngle());
	mFeet->SetPosition(GetPos());
	mFeet->SetTexture(texture_path);
	mFeet->SetOrigin(Vector2(0,50));
	mFeet->SetScale(0.27f);
}

void BaseHuman::Spawn()
{
	GetPhysObj()->SetAngularDamping(25);
	GetPhysObj()->SetLinearDamping(10);
}

BaseHuman::~BaseHuman(void)
{
	delete mHead;
	delete mFeet;
}

void BaseHuman::Draw()
{
	if (mFeet)
		DrawFeet();
	DrawModel();
	if (mHead)
		DrawHead();
}

void BaseHuman::DrawHead()
{	
	//Draw Head on top
	mHead->SetPosition(GetPos());
	mHead->SetAngle(GetAngle() + GetHeadAngle());
	mHead->Draw();
}

void BaseHuman::DrawFeet()
{
	mFeet->SetPosition(GetPos());
	mFeet->SetAngle(GetAngle());
	mFeet->Draw();
}

void BaseHuman::PhysicsSimulate(float delta)
{

}
