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
	mParent = NULL;
	mOwner = NULL;
	mIsRenderable = false;
	mIsPhysics = false;
	mNoDraw = false;
	mNextThink = 0.f;
	mLastThink = 0.f;
	mDeleteMe = false;
	mAngle = 0.f;
	mPosition = Vector2(0.f,0.f);
	mScale = Vector2(1.f,1.f);
	SetHealth(100.f);
	SetMaxHealth(100.f);
	mNextThink = gGlobals.CurTime;
	mLastThink = gGlobals.CurTime;
	mShouldThink = true;
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

Vector2_Rect BaseObject::GetAABB()
{
	if (mAABBNeedsUpdate)
	{
		Vector2 TL, TR, BL, BR; // The four coordinate of an OOBB
		Vector2_Rect bounds = GetRenderBounds(); // Render bounds in local space
		TL = ToGlobal(bounds.Position); // Convert the render bounds to global space
		TR = ToGlobal(Vector2(bounds.Position.x + bounds.Size.x, bounds.Position.y));
		BL = ToGlobal(Vector2(bounds.Position.x, bounds.Position.y + bounds.Size.y));
		BR = ToGlobal(bounds.Position + bounds.Size);

		Vector2 AABB_TL, AABB_BR, AABB_TR, AABB_BL; // The coordinates of the AABB

		AABB_TL.x = std::min(TL.x, std::min(TR.x, std::min(BL.x, BR.x))); // Find the minimum X value of the corners
		AABB_TL.y = std::max(TL.y, std::max(TR.y, std::max(BL.y, BR.y))); // Find the maximum Y value of the corners
		AABB_BR.x = std::max(TL.x, std::max(TR.x, std::max(BL.x, BR.x))); // Find the maximum X value of the corners
		AABB_BR.y = std::min(TL.y, std::min(TR.y, std::min(BL.y, BR.y))); // Find the minimum Y vaule of the corners
		// Only found the Top-Left and Bottom-Right corners, but from these
		// We can deduce the Top-Right and Bottom-Left
		AABB_TR.x = AABB_BR.x;
		AABB_TR.y = AABB_TL.y;
		AABB_BL.x = AABB_TL.x;
		AABB_BL.y = AABB_BR.y;

		mAABB = Vector2_Rect(AABB_BL,AABB_TR-AABB_BL);
		mAABBNeedsUpdate = false;
	}
	return mAABB;
}

void BaseObject::SetParent(BaseObject* parent)
{
	if (parent == NULL || parent == this)
	{
		//if (mIsPhysics)
		//{
		//	GetPhysObj()->SetNoCollide(false);
		//}
		return;
	}
	if (mIsPhysics)
	{
		//GetPhysObj()->SetNoCollide(true);
		//Disable physics object somehow?
	}
	mParent = parent;
	Vector2 position = GetPos();
	mParentRelativePos = mParent->ToLocal(position);
	std::cout << "RELATIVE POSITION: " << mParentRelativePos.ToString() << "\n";
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
	mSprite = new Sprite(gGlobals.RenderWindow);
	/*if (mModelInfo.LoadFromFile(path))
	{
		mModel = mModelInfo.mTexturePath;
		mScale = Vector2(mModelInfo.mScale, mModelInfo.mScale);
		mSprite->SetTexture(mModelInfo.mTexturePath);
	}
	else*/
	{
		mModel = path;
		mScale = Vector2(scale, scale);
		mSprite->SetTexture(path);
	}
	SetRenderBounds(Vector2_Rect(mSprite->GetSize() * -0.5f, mSprite->GetSize()));
	mIsRenderable = true;
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
	if (mParent != NULL)
	{
		SetPos(mParent->ToGlobal(mParentRelativePos));
		//std::cout << GetPos().ToString() << "\n";
		SetAngle(mParent->GetAngle());
	}
	if (IsThinking() && mNextThink < gGlobals.CurTime)
	{
		std::map<char, Sound*>::iterator i;
		
		for(i = mSounds.begin(); i != mSounds.end(); i++) {
			i->second->SetPosition(GetPos());
		}
		
		mLastThink = (float)gGlobals.CurTime;
		//mNextThink = mLastThink + (1.f/60.f); //By default think at 60HZ but child classes can override this
		this->Think();
	}
}
