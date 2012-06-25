#pragma once

#include <SFML/System.hpp>

#define PROFILER_ENABLED 1

enum ProfileType
{
	PROFILE_WHOLE_FRAME,
	PROFILE_STATE_TICK,
	PROFILE_RENDER_FRAME,
	PROFILE_RENDER_MAP,
	PROFILE_RENDER_PURGE,
	PROFILE_PHYSICS_STEP,
	PROFILE_EVENT_HANDLE,
	PROFILE_ENTITY_TICK,
	PROFILE_ENTITY_CREATION,
	COUNT
};

class TreeNode
{
public:
	TreeNode() {for (int i=0; i < 7; i++) mChildren[i] = NULL; mChildCount = 0;};

	ProfileType mType;
	sf::Time mStartTime;
	sf::Time mStopTime;
	sf::Time mExecTime;
	TreeNode* mParent;
	TreeNode* mChildren[8];
	int mChildCount;
};

class Profiler
{
private:
	static sf::Clock mClock;

	static TreeNode* mTreeRoot;
	static TreeNode* mLastStart;
public:
	static void StartFrame() {mLastStart = NULL;};
	static void PrintProfile();
	static void PrintNode(TreeNode* root, int depth);

	static sf::Time GetTime() {return mClock.getElapsedTime();};
	static TreeNode* GetNode(ProfileType type, TreeNode* root);

	static void StartRecord(ProfileType type);
	static void StopRecord(ProfileType type);

	static const char* EnumName(ProfileType type);
};

