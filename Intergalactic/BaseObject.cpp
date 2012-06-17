#include "BaseObject.h"
#include "EntityCreator.h"
#include "GameGlobals.h"

BaseObject::BaseObject(void)
{
	mClassName = "ERROR";
	mIsRenderable = false;
	mNextThink = 0.f;
	mLastThink = 0.f;
	mDeleteMe = false;
}

BaseObject::~BaseObject(void)
{
	Inputs.clear();
}

BaseObject* BaseObject::CreateEntity(const char* classname)
{
	BaseObject* ent = ENTITYCREATOR->CreateEntity(classname);
	gGlobals.gEntList.Append(ent);
	return ent;
}

sf::Matrix3 BaseObject::GetMatrix()
 {
	 if (mMatrixNeedsUpdate)
	 {
		 mMatrix = sf::Matrix3::Transformation(GetOrigin().SF(), GetPos().SF(), GetAngle(), sf::Vector2f(1.f,1.f));
		 mMatrixNeedsUpdate = false;
	 }
	 return mMatrix;
}

Vector2 BaseObject::ToGlobal(Vector2 &point)
{
	return GetMatrix().Transform(point.SF());
}

Vector2 BaseObject::ToLocal(Vector2 &point)
{
	return GetMatrix().GetInverse().Transform(point.SF());
}

void BaseObject::Fire(const char* Name, VariantMap &Data)
{
	if (Inputs[Name] == NULL)
	{
		std::cout << "Invalid input '" << Name << "' invoked!\n";
		return;
	}
	(*Inputs[Name])(this, Data);
}

void BaseObject::RegisterInput(const char* Name, InputFunc Func)
{
	Inputs[Name] = Func;
}

void BaseObject::Tick()
{
	if (mDeleteMe)
	{
		return;
	}
	if (mNextThink < gGlobals.CurTime)
	{
		mLastThink = gGlobals.CurTime;
		mNextThink = mLastThink + (1.f/60.f); //By default think at 60HZ but child classes can override this
		this->Think();
	}
}
