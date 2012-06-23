#pragma once

#include "entitylist.h"
#include "Bases/BaseObject.h"

typedef EntityList<BaseObject*> ObjList;

class EntityListGlobal : public EntityList<BaseObject*>
{
public:
	EntityListGlobal(void);
	~EntityListGlobal(void);
	ObjList* FindInCircle(Vector2 origin, float radius);
	ObjList* FindInBox(Vector2 origin, float height, float width);
	ObjList* FindByClass(const char* classname);
};

