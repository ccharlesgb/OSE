#include "BaseObject.h"
#include "../EntityCreator.h"
#include "../GameGlobals.h"
#include "../Render/Line.h"

BaseObject::BaseObject(void)
{
	mClassName = "ERROR";
	mModel = "";
	mSprite = NULL;
	mPhysObj = NULL;
	mIsRenderable = false;
	mIsPhysics = false;
	mNoDraw = false;
	mNextThink = 0.f;
	mLastThink = 0.f;
	mDeleteMe = false;
	mScale = Vector2(1.f,1.f);
	SetHealth(100.f);
	SetMaxHealth(100.f);
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
	mSprite->SetColour(GetColour());
	mSprite->Draw();
}

void BaseObject::DebugDrawLine(Vector2 p1, Vector2 p2, const Colour &col)
{
	Line line(gGlobals.RenderWindow);
	line.mVerts[0] = p1;
	line.mVerts[1] = p2;
	line.SetColour(col);
	line.Draw();
}

void BaseObject::CreateSound(const char *name, const char *path)
{
	mSounds[*name] = new Sound(path);
}

void BaseObject::EmitSound(const char *name)
{
	if (mSounds[*name] != NULL)
	{
		mSounds[*name]->Play();
	}
}

Sound* BaseObject::GetSound(const char *name)
{
	if (mSounds[*name] != NULL)
	{
		return mSounds[*name];
	}
	return NULL;
}

void BaseObject::TakeDamage(const DamageInfo &info)
{
	SetHealth(GetHealth() - info.Amount);
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
		std::map<char, Sound*>::iterator i;
		
		for(i = mSounds.begin(); i != mSounds.end(); i++) {
			i->second->SetPosition(GetPos());
		}
		
		mLastThink = (float)gGlobals.CurTime;
		mNextThink = mLastThink + (1.f/60.f); //By default think at 60HZ but child classes can override this
		this->Think();
	}
}
