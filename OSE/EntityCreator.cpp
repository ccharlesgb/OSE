#include "EntityCreator.h"
#include <iostream>
#include "BaseObject.h"

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
		return ent;
	}
	return NULL;
}

void EntityCreator::RegisterEntity(const char* &ID, BaseRenderableFactory* F)
{
	std::cout << "Registering: " << ID << "\n";
	FactoryMap[ID] = F;
}
