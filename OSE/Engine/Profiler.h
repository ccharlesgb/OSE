#pragma once

#include <SFML/System.hpp>
#include <iostream>

#define PROFILER_ENABLED 1

enum ProfileType
{
	PROFILE_WHOLE_FRAME,
	PROFILE_STATE_TICK,
	PROFILE_RENDER_FRAME,
	PROFILE_RENDER_DRAWCALL,
	PROFILE_ENTITY_DRAW,
	PROFILE_RENDER_MAP,
	PROFILE_RENDER_PURGE,
	PROFILE_PHYSICS_STEP,
	PROFILE_EVENT_HANDLE,
	PROFILE_ENTITY_TICK,
	PROFILE_ENTITY_CREATION,
	PROFILE_TEMPORARY_1,
	PROFILE_TEMPORARY_2,
	PROFILE_TYPE_COUNT
};

class ProfileInformation
{
public:
	std::string NAME[PROFILE_TYPE_COUNT];
	float TIME[PROFILE_TYPE_COUNT];
	int DEPTH[PROFILE_TYPE_COUNT];
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
	static int mPrintOrder;
public:
	static ProfileInformation *mLastFrameInfo;
	static void StartFrame() {mLastStart = NULL;};
	static void PrintProfile();
	static void PrintNode(TreeNode* root, int depth);

	static sf::Time GetTime() {return mClock.getElapsedTime();};
	static TreeNode* GetNode(ProfileType type, TreeNode* root);

	static void StartRecord(ProfileType type);
	static void StopRecord(ProfileType type);

	static const char* EnumName(ProfileType type);

	static void Init() {mLastFrameInfo = new ProfileInformation();};

	static ProfileInformation* GetLastFrameInfo() {return mLastFrameInfo;};
};

