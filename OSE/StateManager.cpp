#include "StateManager.h"
#include "IGameState.h"
#include "Renderer.h"
#include "GameGlobals.h"
#include "MathUtils.h"
#include "InputHandler.h"

StateManager* Inst = NULL;

StateManager* StateManager::Instance()
{
	if (Inst == NULL)
	{
		Inst = new StateManager;
	}
	return Inst;
}

StateManager::StateManager(void)
{
	ig::SeedRandom();
	int width = 1024;
	int height = 768;
	gGlobals.GameWidth = width;
	gGlobals.GameHeight = height;
	std::string title = "Intergalactic";
	Window.create(sf::VideoMode(width, height, 32), title);
	Window.setFramerateLimit(120);
	//Window.EnableVerticalSync(true);
	RENDERER->SetWindow(&Window);
	mStateCount = 0;
	mCurrentState = NULL;
	bRunning = true;
	gGlobals.InFocus = true;
	InputHandler::SetWindow(&Window);
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
	delete Inst;
}

void StateManager::Begin()
{
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
			if (!mCurrentState->GetPaused())
			{
				mCurrentState->_Tick();
			}
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
					else
					{
						if (Event.type == sf::Event::Resized)
						{
							RENDERER->OnResize();
						}
						mCurrentState->_OnEvent(Event);
					}
				}
			}
			RENDERER->Draw(mCurrentState);
			RENDERER->Display();
			mLastFrame = GetRealTime();
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