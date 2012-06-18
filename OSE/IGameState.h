#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include "StateManager.h"
#include "EntityListGlobal.h"

class BaseObject;

class IGameState : public IEntityListener<BaseObject*>
{
private:
	bool mPaused;
	bool mPausable;
	friend class Renderer;
	bool mStarted;
	double mDelta;
	double mFrameTime;
	double mLastFrame;
	double mStartTime;
	friend class StateManager;
protected:
	sf::Clock mClock;
public:
	IGameState(void);
	virtual ~IGameState();

	void Pause() {mPaused = true;};
	void UnPause() {mPaused = false;};
	bool GetPaused() {return mPaused;};
	void SetPausable(bool p) {mPausable = p;};
	bool GetPausable() {return mPausable;};
	void SetDelta(float d) {mDelta = d;};
	double GetDelta() {return mDelta;};
	void OnEntityAdded(BaseObject* ent);
	void OnEntityRemoved(BaseObject* ent);
	BaseObject* CreateEntity(const char* ID);

	bool GetStarted() {return mStarted;};

	virtual void Initialize() {};
	virtual void ShutDown() {};
	virtual void OnEntityCreated(BaseObject* ent) {};
	virtual void OnEntityDeleted(BaseObject* ent) {};
	virtual void Tick() {};
	virtual void OnEvent(sf::Event &Event) {};
	virtual void OnKeyPressed(sf::Keyboard::Key Key, bool Pressed) {};
	virtual void OnMouseButtonPressed(sf::Mouse::Button Button, bool Pressed) {};
	virtual void DrawDebugData() {};

	void _Initialize();
	void _ShutDown();
	void _OnEntityCreated(BaseObject* ent) {OnEntityCreated(ent);};
	void _OnEntityDeleted(BaseObject* ent) {};
	void _Tick(); //Declared in cpp
	void _OnEvent(sf::Event &Event);
	void _OnKeyPressed(sf::Keyboard::Key Key, bool Pressed) {OnKeyPressed(Key, Pressed);};
	void _OnMouseButtonPressed(sf::Mouse::Button Button, bool Pressed) {OnMouseButtonPressed(Button, Pressed);};
};

template<class T>
struct RegisterState
{
	RegisterState(char* name)
	{
		BaseStateFactory* F = new StateFactory<T>;
		STATEMANAGER->RegisterState(std::string(name), F);
	}
};

#define REGISTERSTATE(name, t) \
	static RegisterState<t> Regi(name);
