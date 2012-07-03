#include "StateManager.h"
#include "IGameState.h"
#include "Render/Renderer.h"
#include "GameGlobals.h"
#include "Utilities/MathUtils.h"
#include "InputHandler.h"
#include "Profiler.h"

StateManager* Inst3 = NULL;

#define FULLSCREEN false

StateManager* StateManager::Instance()
{
	if (Inst3 == NULL)
	{
		Inst3 = new StateManager;
	}
	return Inst3;
}

StateManager::StateManager(void)
{
	Profiler::Init();
	ig::SeedRandom();

	mStateCount = 0;
	mCurrentState = NULL;
	bRunning = true;
	gGlobals.InFocus = true;
	mLastProfilePrint = 0.f;
}

StateManager::~StateManager(void)
{
	std::map<std::string, IGameState*>::iterator i;
	for (i = mStates.begin(); i != mStates.end(); i++)
	{
		delete i->second;
	}
	mStates.clear();
	mCurrentState = NULL;
	ENTITYCREATOR->Cleanup();
	RENDERER->Cleanup();
}

void StateManager::Cleanup()
{
	delete Inst3;
}

void StateManager::Begin()
{
	int width = 1920;
	int height = 1080;
	gGlobals.GameWidth = width;
	gGlobals.GameHeight = height;
	std::string title = "Onslaught";
	if (FULLSCREEN)
	{
		sf::VideoMode mode = sf::VideoMode::getFullscreenModes().front();
		Window.create(mode, title,sf::Style::Fullscreen);
	}
	else
	{
		Window.create(sf::VideoMode(width,height, 32),title);
	}
	//Window.setFramerateLimit(120);
	gGlobals.RenderWindow = &Window;
	//Window.EnableVerticalSync(true);
	RENDERER->SetWindow(&Window);
	InputHandler::SetWindow(&Window);
	//BEGIN GAME LOOP
	SetState("game");
	Run();
}

void StateManager::Run()
{
	while (bRunning)
	{	
		double StartFrame = GetRealTime();
		gGlobals.FrameTime = StartFrame - gGlobals.RealTime;
		gGlobals.RealTime = StartFrame;
		if (mCurrentState != NULL)
		{
			Profiler::StartFrame();
			Profiler::StartRecord(PROFILE_WHOLE_FRAME);
			if (!mCurrentState->GetPaused())
			{
				Profiler::StartRecord(PROFILE_STATE_TICK);
				mCurrentState->_Tick();
				Profiler::StopRecord(PROFILE_STATE_TICK);
			}
			Profiler::StartRecord(PROFILE_EVENT_HANDLE);
			HandleEvents();
			Profiler::StopRecord(PROFILE_EVENT_HANDLE);
			Profiler::StartRecord(PROFILE_RENDER_FRAME);
			RENDERER->Draw(mCurrentState);
			Profiler::StopRecord(PROFILE_RENDER_FRAME);
			mLastFrame = GetRealTime();
			Profiler::StopRecord(PROFILE_WHOLE_FRAME);
			if (mLastProfilePrint + 1.f < gGlobals.RealTime)
			{
				Profiler::PrintProfile();
				mLastProfilePrint = gGlobals.RealTime;
			}
		}
	}
}

void StateManager::HandleEvents()
{
	sf::Event Event;
	while(Window.pollEvent(Event))
	{
		if (Event.type == sf::Event::LostFocus)
		{
			mCurrentState->Pause();
			gGlobals.InFocus = false;
			std::cout << "pause\n";
		}
		else if (Event.type == sf::Event::GainedFocus)
		{
			mCurrentState->UnPause();
			gGlobals.InFocus = true;
			std::cout << "un pause\n";
		}
		else if (Event.type == sf::Event::Closed)
		{
			mCurrentState->_ShutDown();
			bRunning = false;
		}
		if (!mCurrentState->GetPaused())
		{
			if (Event.type == sf::Event::KeyPressed)
			{
				mCurrentState->_OnKeyPressed(Event.key.code, true);
			}
			else if (Event.type == sf::Event::KeyReleased)
			{
				mCurrentState->_OnKeyPressed(Event.key.code, false);
			}
			else if (Event.type == sf::Event::MouseButtonPressed)
			{
				mCurrentState->_OnMouseButtonPressed(Event.mouseButton.button, true);
			}
			else if (Event.type == sf::Event::MouseButtonReleased)
			{
				mCurrentState->_OnMouseButtonPressed(Event.mouseButton.button, false);
			}
			else if (Event.type == sf::Event::Resized)
			{
					RENDERER->OnResize();
			}
			else
				mCurrentState->_OnEvent(Event);
		}
	}
}

void StateManager::SetState(std::string ID)
{
	mCurrentState = mStates[ID];
	if (!mCurrentState->GetStarted())
	{
		//:TODO: What happens if the state has already started?
		//		 Initialize probably needs to be called every time?
		mCurrentState->_Initialize();
		mCurrentState->mStarted = true;
		mCurrentState->mPaused = false;
	}
}

void StateManager::RegisterState(std::string ID, BaseStateFactory* F)
{
	mStates[ID] = F->Create();
	delete F;
}