#include "Profiler.h"
#include <iostream>

sf::Clock Profiler::mClock;
TreeNode* Profiler::mTreeRoot;
TreeNode* Profiler::mLastStart;

void Profiler::PrintProfile()
{
#if PROFILER_ENABLED
	TreeNode* CurNode = mTreeRoot;
	PrintNode(mTreeRoot,0);
	mLastStart = NULL;
#endif
}

const char* Profiler::EnumName(ProfileType type)
{
	const char* name;
	switch(type)
	{
	case PROFILE_WHOLE_FRAME:
		name = "FRAMETIME   ";
		break;
	case PROFILE_STATE_TICK:
		name = "STATE_TICK  ";
		break;
	case PROFILE_RENDER_FRAME:
		name = "RENDER      ";
		break;
	case PROFILE_PHYSICS_STEP:
		name = "PHYSICS     ";
		break;
	case PROFILE_EVENT_HANDLE:
		name = "EVENTS      ";
		break;
	case PROFILE_ENTITY_TICK:
		name = "ENTITY TICK ";
		break;
	case PROFILE_ENTITY_CREATION:
		name = "ENT CREATION";
		break;
	case PROFILE_RENDER_MAP:
		name = "MAP RENDER";
		break;
	default:
		name = "ERROR       ";
		break;
	}

	return name;
}

void Profiler::PrintNode(TreeNode* root, int depth)
{
	int i = 0;
	std::string INDENT;
	for (int ind = 0; ind < depth * 4; ind++)
		INDENT = INDENT + ' ';
	std::string INV_INDENT;
	for (int ind = 0; ind < 4 * (4 - depth); ind ++)
	{
		INV_INDENT = INV_INDENT + ' ';
	}
	float Percent = (root->mExecTime.asSeconds() / mTreeRoot->mExecTime.asSeconds()) * 100.f;
	std::cout << INDENT << EnumName(root->mType) << INV_INDENT << " Time: " << root->mExecTime.asSeconds() * 1000.f << "ms" 
		<< "         (" << std::floor(Percent + 0.5f) << "%)\n";
	TreeNode* CurChild = root->mChildren[0];
	do
	{
		if (CurChild == NULL)
		{

		}
		else
		{
			PrintNode(CurChild, depth + 1);
			i++;
			if (i >=7)
			{
				break;
			}
			CurChild = root->mChildren[i];
		}
	}
	while (CurChild != NULL);

	delete root;
}

void Profiler::StartRecord(ProfileType type) 
{
#if PROFILER_ENABLED
	TreeNode* node = new TreeNode();
	node->mStartTime = GetTime();
	node->mParent = mLastStart;
	if (mLastStart)
	{
		mLastStart->mChildren[mLastStart->mChildCount] = node;
		mLastStart->mChildCount++;
	}
	else
		mTreeRoot = node;
	node->mType = type;
	mLastStart = node;
#endif
}

void Profiler::StopRecord(ProfileType type) 
{
#if PROFILER_ENABLED
	mLastStart->mStopTime = GetTime();
	mLastStart->mExecTime = mLastStart->mStopTime - mLastStart->mStartTime;
	mLastStart = mLastStart->mParent;
#endif
};
