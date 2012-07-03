#include "MainGameState.h"
#include "../Engine/Bases/BaseObject.h"
#include "../Engine/Utilities/VariantMap.h"
#include "../Engine/Camera.h"
#include "../Engine/Bases/BasePhysics.h"
#include "../Engine/PhysicsDef.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/InputHandler.h"
#include "../Engine/PhysicsQueries.h"
#include "../Engine/AudioEnvironment.h"
#include "../Engine/AudioEnvironment.h"
#include "../Engine/Profiler.h"
#include "../Engine/EntityList.h"

#define GRAVITY_STRENGTH 500

REGISTERSTATE("game", MainGameState);

/*
NAME	: Constructor
NOTES	: 
*/
MainGameState::MainGameState(void)
{
	mLastPhysics = -1;
	mPhysAcc = 0;
	mNextHorde = 0;
}

/*
NAME	: Destructor
NOTES	: Free the box 2d world, entities are freed up in IGameState's
		  destructor
*/
MainGameState::~MainGameState(void)
{
	std::cout << "DELETING GAME STATE " << gGlobals.gEntList.GetSize() << "\n";
}

/*
:TODO: Is this the best way of doing this?
NAME	: DrawDebugData()
NOTES	: Draw the B2D world debug draw
*/
void MainGameState::DrawDebugData()
{
	mPhysicsWorld.DrawDebug();
}

/*
NAME	: Initialize()
NOTES	: Create default entities, generate the map (eg solar systems)
*/
void MainGameState::Initialize()
{
	Hud = new HUD();
	
	Map = CreateEntity("world");
	//Map->Spawn();
	
	Player = CreateEntity("player");
	//Player->Spawn();
	gGlobals.Player = Player;
	sCamera::FollowEntity(Player);
	sCamera::SetDamping(0.f);
	sCamera::SetZoom(1.5f);
	sCamera::SetZoomDamping(0.2f);
	
	sAudioEnvironment::SetListener(Player);
	
	BaseObject* car = CreateEntity("car");
	car->SetPos(Vector2(100, 0));

	BaseObject* crate;
	int crate_count = 200;
	int tree_count = 100;
	float map_size = 4500.f;
	for (int i=0; i < crate_count; i++)
	{
		Vector2 pos = Vector2::Random(-map_size,map_size);
		int Chooser = ig::RandomInt(0,10);
		int Amount = ig::RandomInt(1,3);
		for (int i=0; i < Amount; i++)
		{
			crate = CreateEntity("ent_prop");
			crate->SetModel("props/crate2", ig::Random(0.3f,0.5f));
			crate->SetPos(pos + Vector2::Random(-20.f,20.f));
		}
	}
	for (int i=0; i < tree_count; i++)
	{
		float tree_size = ig::Random(0.6f,0.9f);
		Vector2 tree_pos = Vector2::Random(-map_size, map_size);
		crate = CreateEntity("ent_prop_static");
		crate->SetModel("props/tree1", tree_size);
		crate->SetDrawOrder(RENDERGROUP_TOP);
		crate->SetPos(tree_pos);

		BaseObject* trunk = CreateEntity("ent_decal");
		trunk->SetModel("props/tree_trunk", tree_size);
		trunk->SetPos(tree_pos);
	}
	crate = CreateEntity("primitive");
	crate->SetPos(Vector2());

}

/*
NAME	: ShutDown
NOTES	: Remove all physics entities before the Box2D world is removed
		  in the destructor.
*/
void MainGameState::ShutDown()
{
	EntityList<BaseObject*>::iter CurEnt = gGlobals.gEntList.FirstEnt();
	while(CurEnt != gGlobals.gEntList.End())
	{
		if ((*CurEnt)->IsPhysicsEnabled())
		{
			CurEnt = gGlobals.gEntList.DeleteHard(CurEnt);
		}
		else
			CurEnt = gGlobals.gEntList.NextEnt(CurEnt);
	}
}

/*
NAME	: Tick()
NOTES	: Loop through all physics entities and apply simulated gravity
		  to the player ship. Also step the physics world by GetDelta()
*/
void MainGameState::Tick()
{
	Profiler::StartRecord(PROFILE_PHYSICS_STEP);
	EntityList<BaseObject*>::iter CurEnt = gGlobals.gEntList.FirstEnt();
	while(CurEnt != gGlobals.gEntList.End())
	{
		//std::cout << "CUR: " << CurEnt << "\n";
		if ((*CurEnt)->IsPhysicsEnabled() && mLastPhysics + GetDelta() < gGlobals.CurTime)
		{
			(*CurEnt)->PhysicsSimulate(gGlobals.CurTime - mLastPhysics);
		}
		CurEnt = gGlobals.gEntList.NextEnt(CurEnt);
	}

	if (mNextHorde < gGlobals.CurTime)
	{
		BaseObject* zombie;
		int horde_size = ig::RandomInt(3,15);
		float ang = ig::Random(0,6.82f);
		Vector2 offset = Vector2(std::cos(ang), std::sin(ang)) * ig::Random(2000,2500);
		Vector2 pos = Player->GetPos() + offset;
		for (int i=0; i < horde_size; i++)
		{
			zombie = CreateEntity("enemy");
			zombie->SetPos(pos + Vector2::Random(-50,50));
			zombie->SetAngle(ig::Random(0,360));
		}
		mNextHorde = gGlobals.CurTime + ig::Random(10.f,20.f);
	}

	if (mLastPhysics == -1)
		mLastPhysics = gGlobals.RealTime;
	if (mLastPhysics + GetDelta() < gGlobals.CurTime)
	{
		float delta = gGlobals.CurTime - mLastPhysics;
		mPhysicsWorld.Step(delta);

		mLastPhysics = gGlobals.CurTime;
	}
	Profiler::StopRecord(PROFILE_PHYSICS_STEP);
}

/*
NAME	: OnEvent()
NOTES	: Handles zooming in and out the camera when the mouse wheel is
		  moved.
*/
void MainGameState::OnEvent(sf::Event &Event)
{
	if (Event.type == sf::Event::MouseWheelMoved)
	{
		float del = (float)Event.mouseWheel.delta;
		float fac = 1.f;
		fac = 1.f + (del / 5.f);
		float zoom = sCamera::GetZoom() * fac;
		zoom = ig::Limit(zoom, 0.5, 1.5f);
		sCamera::SetZoom(zoom);
	}
}

void MainGameState::AdjustZoom(float zoom_target)
{

}

/*
NAME	: OnEntityCreated()
NOTES	: If the entity is a physics entity add it to the Box2D world
*/
void MainGameState::OnEntityCreated(BaseObject* ent)
{
	if (ent->IsPhysicsEnabled())
	{
		BasePhysics* physent = dynamic_cast<BasePhysics*>(ent);
		mPhysicsWorld.AddPhysicsDef(physent->GetPhysObj());
	}
	ent->Spawn();
}

/*
NAME	: OnKeyPressed
NOTES	: 
*/
void MainGameState::OnKeyPressed(sf::Keyboard::Key Key, bool Pressed)
{
	if (Pressed && Key == sf::Keyboard::O)
	{
		sAudioEnvironment::SetMasterVolume(sAudioEnvironment::GetMasterVolume() + 10.f);
	}
	if (Pressed && Key == sf::Keyboard::L)
	{
		sAudioEnvironment::SetMasterVolume(sAudioEnvironment::GetMasterVolume() - 10.f);
	}
	if (Pressed && Key == sf::Keyboard::F2)
	{
		gGlobals.EnableRenderCulling = !gGlobals.EnableRenderCulling;
		std::string state = "ENABLED";
		if (!gGlobals.EnableRenderCulling)
			state = "DISABLED";
		std::cout << "RENDER CULLING " << state << "\n";
	}
}

/*
NAME	: OnMouseButtonPressed
NOTES	: 
*/
void MainGameState::OnMouseButtonPressed(sf::Mouse::Button Button, bool Pressed)
{
	if (InputHandler::IsKeyPressed(sf::Keyboard::LShift))
	{
		if (Pressed && Button == sf::Mouse::Right)
		{
			BaseObject* MouseEnt = PhysicsQueries::QueryPoint(InputHandler::GetMousePosWorld());
			if (MouseEnt)
			{
				if (MouseEnt->GetClassName() != "player")
					MouseEnt->Delete();
			}
		}
		if (Pressed && Button == sf::Mouse::Left)
		{
			BaseObject* crate;
			crate = CreateEntity("effect_light");
			crate->SetPos(InputHandler::GetMousePosWorld());
		}
	}
}