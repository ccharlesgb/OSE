#include "EntityListGlobal.h"
#include "GameGlobals.h"

EntityListGlobal::EntityListGlobal(void)
{
}

EntityListGlobal::~EntityListGlobal(void)
{
}

ObjList* EntityListGlobal::FindInCircle(Vector2 origin, float radius)
{
	float radius_squared = radius * radius; //Avoid the square root!
	ObjList* list;
	BaseObject* CurEnt = gGlobals.gEntList.FirstEnt();
	while(CurrentIsValid())
	{
		float dist_squared = (CurEnt->GetPos() - origin).LengthSquared();
		if (dist_squared <= radius_squared)
		{
			list->Append(CurEnt);
		}
		CurEnt = gGlobals.gEntList.NextEnt();
	}
	return list;
}

ObjList* EntityListGlobal::FindInBox(Vector2 origin, float height, float width)
{

}

ObjList* EntityListGlobal::FindByClass(const char* classname)
{
	ObjList* list = new ObjList();
	BaseObject* CurEnt = gGlobals.gEntList.FirstEnt();
	while(CurrentIsValid())
	{
		if (CurEnt->GetClassName() == classname)
		{
			list->Append(CurEnt);
		}
		CurEnt = gGlobals.gEntList.NextEnt();
	}
	return list;
}