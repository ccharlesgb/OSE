#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <map>
#include <string>
#include "EntityCreator.h"

class IGameState;

class BaseStateFactory
{
public:
	virtual IGameState* Create() = 0;
};

template <class T>
class StateFactory : public BaseStateFactory
{
public:
	IGameState* Create() {return new T;}
};

#define STATEMANAGER StateManager::Instance()
class StateManager
{
private:
	sf::Clock mClock;
	IGameState* mCurrentState;
	std::map<std::string, IGameState*> mStates;
	int mStateCount;
	bool bRunning;
	sf::RenderWindow Window;
	EntityCreator EntityOverlord;
	double mLastFrame;
public:
	static void Cleanup();
	static StateManager* Instance();
	StateManager(void);
	~StateManager(void);
	double GetRealTime() {return double(mClock.getElapsedTime().asSeconds());};
	void Begin();
	void Run();
	void SetState(std::string ID);
	void RegisterState(std::string ID, BaseStateFactory* F);
};

