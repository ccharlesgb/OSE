#include "MainGameState.h"
#include "../Engine/BaseObject.h"
#include "../Engine/VariantMap.h"
#include "../Engine/Camera.h"
#include "../Engine/BasePhysics.h"
#include "../Engine/PhysicsDef.h"
#include "../Engine/GameGlobals.h"
#include "../Engine/InputHandler.h"

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
	Map = CreateEntity("world");
	Map->Spawn();
	
	Player = CreateEntity("player");
	Player->Spawn();
	gGlobals.Player = Player;
	sCamera::FollowEntity(Player);
	sCamera::SetDamping(0);
	sCamera::SetZoom(1.5);
	sCamera::SetZoomDamping(0.2);
}

/*
NAME	: ShutDown
NOTES	: Remove all physics entities before the Box2D world is removed
		  in the destructor.
*/
void MainGameState::ShutDown()
{
	BaseObject* CurEnt = gGlobals.gEntList.FirstEnt();
	while(gGlobals.gEntList.CurrentIsValid())
	{
		if (CurEnt->IsPhysicsEnabled())
		{
			gGlobals.gEntList.DeleteCurrent();
			CurEnt = gGlobals.gEntList.CurrentEnt();
		}
		else
			CurEnt = gGlobals.gEntList.NextEnt();
	}
}

/*
NAME	: Tick()
NOTES	: Loop through all physics entities and apply simulated gravity
		  to the player ship. Also step the physics world by GetDelta()
*/
void MainGameState::Tick()
{
	BaseObject* CurEnt = gGlobals.gEntList.FirstEnt();
	while(CurEnt != NULL)
	{
		CurEnt = gGlobals.gEntList.NextEnt();
	}

	if (mLastPhysics == -1)
		mLastPhysics = gGlobals.RealTime;
	if (mLastPhysics + GetDelta() < gGlobals.RealTime)
	{
		float delta = gGlobals.RealTime - mLastPhysics;
		mPhysicsWorld.Step(delta);
		mLastPhysics = gGlobals.RealTime;
	}
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
		float del = Event.mouseWheel.delta;
		std::cout << del << "\n";
		float fac = 1.f;
		fac = 1.f + (del / 5.f);
		float zoom = sCamera::GetZoom() * fac;
		zoom = ig::Limit(zoom, 1, 3.f);
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
}

/*
NAME	: OnKeyPressed
NOTES	: 
*/
void MainGameState::OnKeyPressed(sf::Keyboard::Key Key, bool Pressed)
{
	if (Pressed && Key == sf::Keyboard::O)
	{
		sCamera::SetZoom(ig::Limit(sCamera::GetZoom() + 0.5, 1, 3));
	}
	if (Pressed && Key == sf::Keyboard::L)
	{
		sCamera::SetZoom(ig::Limit(sCamera::GetZoom() - 0.5, 1, 3));
	}
}

/*
NAME	: OnMouseButtonPressed
NOTES	: 
*/
void MainGameState::OnMouseButtonPressed(sf::Mouse::Button Button, bool Pressed)
{
	if (Pressed && Button == sf::Mouse::Button::Right)
	{
		BaseObject* MouseEnt = mPhysicsWorld.QueryPoint(InputHandler::GetMousePosWorld());
		if (MouseEnt)
		{
			if (MouseEnt->GetClassName() != "player")
				MouseEnt->Delete();
		}
	}
	if (Pressed && Button == sf::Mouse::Button::Left)
	{
		BaseObject* crate = CreateEntity("ent_prop");
		crate->SetModel("crate", ig::Random(0.5,0.7));
		crate->SetPos(InputHandler::GetMousePosWorld());
		crate->Spawn();
	}
}