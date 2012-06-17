#include "IGameState.h"
#include "BaseRenderable.h"
#include "DebugDraw.h"
#include "GameGlobals.h"

/*
NAME	: OnEntityAdded
NOTES	: IEntityListener implementation
*/
void IGameState::OnEntityAdded(BaseObject* ent)
{
	_OnEntityCreated(ent);
}

/*
NAME	: OnEntityRemoved
NOTES	: IEntityListener implementation
*/
void IGameState::OnEntityRemoved(BaseObject* ent)
{
	_OnEntityDeleted(ent);
}

/*
NAME	: CreateEntity
NOTES	: Allows game states to create entities. Returns BaseObject pointer
		  to allow state to further manipulate entity.
*/
BaseObject* IGameState::CreateEntity(const char* ID)
{
	BaseObject* ent = ENTITYCREATOR->CreateEntity(ID);
	gGlobals.gEntList.Append(ent);
	_OnEntityCreated(ent);
	return ent;
}

/*
NAME	: _Initialize()
NOTES	: Internal init function. Called when state is first run.
		  Override with Initialize()
*/
void IGameState::_Initialize()
{
	mStartTime = gGlobals.RealTime;
	Initialize();
}

/*
NAME	: _ShutDown()
NOTES	: Called before the destructor. Handle game logic stuff here.
		  DO NOT handle memory stuff ie "delete" calls
*/
void IGameState::_ShutDown()
{
	ShutDown();
}

/*
NAME	: _Tick()
NOTES	: Internal tick function. Called every frame by statemanager
		  Handles entity deletion in state, and entity think functions
		  Override with Tick()
*/
void IGameState::_Tick()
{
	gGlobals.CurTime = gGlobals.RealTime - mStartTime;
	Tick();

	BaseObject* CurEnt = gGlobals.gEntList.FirstEnt();
	while(gGlobals.gEntList.CurrentIsValid())
	{
		if (CurEnt->FlaggedForDeletion() == true)
		{
			_OnEntityDeleted(CurEnt);
			gGlobals.gEntList.DeleteCurrent();
			CurEnt = gGlobals.gEntList.CurrentEnt();
		}
		CurEnt->Tick();
		CurEnt = gGlobals.gEntList.NextEnt();
	}
}

/*
NAME	: _OnEvent()
NOTES	: When a non specific event happens, IE not a keyboard or mouse
		  event.
*/
void IGameState::_OnEvent(sf::Event &Event)
{
	OnEvent(Event);
}

/*
NAME	: Constructor
NOTES	: 
*/
IGameState::IGameState()
{
	mHUD = new HUDRender();
	mPaused = true;
	mStarted = false;
	mPausable = true;
	mDelta = 1/60.f;
	gGlobals.gEntList.RegisterListener(this);
}

/*
NAME	: Destructor
NOTES	: 
*/
IGameState::~IGameState()
{
	gGlobals.gEntList.Clear();
}
