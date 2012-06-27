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
	ObjList* list = new ObjList;
	EntityList::iter CurEnt = FirstEnt();
	while(CurEnt != End())
	{
		float dist_squared = ((*CurEnt)->GetPos() - origin).LengthSquared();
		if (dist_squared <= radius_squared)
		{
			list->Append(*CurEnt);
		}
		CurEnt = gGlobals.gEntList.NextEnt(CurEnt);
	}
	return list;
}

ObjList* EntityListGlobal::FindInBox(Vector2 origin, float height, float width)
{
	ObjList* list = NULL;
	return list;
}

ObjList* EntityListGlobal::FindByClass(const char* classname)
{
	ObjList* list = new ObjList();
	EntityList::iter CurEnt = gGlobals.gEntList.FirstEnt();
	while(CurEnt != gGlobals.gEntList.End())
	{
		if ((*CurEnt)->GetClassName() == classname)
		{
			list->Append(*CurEnt);
		}
		CurEnt = gGlobals.gEntList.NextEnt(CurEnt);
	}
	return list;
}