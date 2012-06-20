#include "BaseObject.h"
#include "EntityCreator.h"
#include "GameGlobals.h"
#include "Renderer.h"

BaseObject::BaseObject(void)
{
	mClassName = "ERROR";
	mModel = "";
	mIsRenderable = false;
	mIsPhysics = false;
	mNextThink = 0.f;
	mLastThink = 0.f;
	mDeleteMe = false;
	mScale = Vector2(1.f,1.f);
}

BaseObject::~BaseObject(void)
{
	Inputs.clear();
	if (strlen(mModel) != 0) {
		delete mSprite;
	}
}

BaseObject* BaseObject::CreateEntity(const char* classname)
{
	//Create an entity and add it to the global entity list
	BaseObject* ent = ENTITYCREATOR->CreateEntity(classname);
	gGlobals.gEntList.Append(ent);
	return ent;
}

Matrix3 BaseObject::GetMatrix()
{
	 if (mMatrixNeedsUpdate)
	 {
		 mMatrix = Matrix3::Transformation(Vector2().SF(), GetPos().SF(), GetAngle(), sf::Vector2f(1.f,1.f));
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
	//Fire an input
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

void BaseObject::SetModel(const char* path)
{
	SetModel(path, 1.f);
}

void BaseObject::SetModel(const char* path, float scale)
{
	mIsRenderable = true;
	mModel = path;
	mScale = Vector2(scale,scale);
	mSprite = new Sprite(gGlobals.RenderWindow);
	mSprite->SetTexture(path);
	mSprite->SetScale(scale);
	SetOrigin(Vector2());
}

void BaseObject::Draw()
{
	DrawModel();
}

void BaseObject::DrawModel()
{
	mSprite->SetPosition(GetPos().SF());
	mSprite->SetAngle(GetAngle());
	mSprite->SetOrigin(GetOrigin());
	mSprite->Draw();
}

//Tick: An INTERNAL function called by the game state every frame, only actually Thinks if NextThink < CurTime
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
