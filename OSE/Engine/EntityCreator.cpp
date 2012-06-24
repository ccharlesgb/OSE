#include "EntityCreator.h"
#include <iostream>
#include "Bases/BaseObject.h"
#include "GameGlobals.h"

EntityCreator* Inst2 = NULL;

EntityCreator* EntityCreator::Instance()
{
	if (Inst2 == NULL)
	{
		Inst2 = new EntityCreator;
	}
	return Inst2;
}

EntityCreator::EntityCreator(void)
{
}

EntityCreator::~EntityCreator()
{
	std::map<std::string, BaseRenderableFactory*>::iterator i;
	for (i = FactoryMap.begin(); i != FactoryMap.end(); i++)
	{
		delete i->second;
	}
	FactoryMap.clear();
}

void EntityCreator::ProcessAddQueue()
{
	std::vector<BaseObject*>::iterator i;
	i = mAddQueue.begin();
	while(i != mAddQueue.end())
	{
		gGlobals.gEntList.Append(*i);
		i++;
	}
	mAddQueue.clear();
}

void EntityCreator::Cleanup()
{
	delete Inst2;
}

BaseObject* EntityCreator::CreateEntity(const char* ID)
{
	BaseRenderableFactory* F = FactoryMap[ID];
	if (F != NULL)
	{
		BaseObject* ent = F->Create();
		ent->SetClassName(ID);
		mAddQueue.push_back(ent);
		return ent;
	}
	return NULL;
}

void EntityCreator::RegisterEntity(const char* &ID, BaseRenderableFactory* F)
{
	std::cout << "Registering: " << ID << "\n";
	FactoryMap[ID] = F;
}
