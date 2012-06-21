#pragma once
#include "Utilities/MathUtils.h"
#include "Utilities/VariantMap.h"
#include <iostream>
#include "EntityCreator.h"
#include "Utilities/Vector2.h"
#include "SFML/Graphics.hpp"
#include "Utilities/Matrix3.hpp"
#include "Sprite.h"
#include "Sound.h"

class PhysicsDef;
class BaseObject;
class CollisionInfo;

typedef void (*InputFunc)(BaseObject* ent, VariantMap &Data);

class Colour
{
public:
	int r;
	int g;
	int b;
	int a;
	Colour() {r=g=b=a=255;};
	Colour(int R, int G, int B) {r=R; g = G; b = B; a = 255;};
	Colour(int R, int G, int B, int A) {r=R; g = G; b = B; a = A;};
	Colour(const Colour& col) {r = col.r; g = col.g; b = col.b; a = col.a;};
};

enum RenderGroup
{
	RENDERGROUP_BACKGROUND, //Draw first
	RENDERGROUP_ENTITIES,
	RENDERGROUP_PLAYER,
	RENDERGROUP_HUD //Draw Last
};

class BaseObject
{
private:
	std::map<std::string, InputFunc> Inputs; //Entity IO function pointers
	VariantMap mMetaData; //Variables that other entities can access
	bool mDeleteMe; //Should the entity be deleted on the next entity purge?
	float mNextThink;
	float mLastThink;
	std::string mClassName; //The class name of the entity eg "player"

	//Rendering
	Colour mColour;
	Sprite* mSprite;
	bool mNoDraw;
	BaseObject* mOwner;
protected:
	PhysicsDef *mPhysObj; //Pointer to the physics object for this entity (NOT NECCESSARILY VALID)
	Matrix3 mMatrix; //Transformation matrix used by ToGlobal and ToLocal
	bool mMatrixNeedsUpdate; //If the matrix needs to be updated (Position/rotation etc has changed)
	const char* mModel; //Model path (Sprite image)
	
	std::map<char, Sound*> mSounds;
	
	Vector2 mPosition;
	Vector2 mOrigin;
	Vector2 mScale;
	float mAngle;

	RenderGroup mDrawOrder;
	bool mIsRenderable; //Do we have a renderer?
	bool mIsPhysics; //Do we have a physics object?
public:
	BaseObject(void);
	virtual ~BaseObject(void);

	void SetClassName(const char* ID)
	{
		if (mClassName == "ERROR") //Only do this once!
			mClassName = ID;
	};
	std::string GetClassName() {return mClassName;};

	Matrix3 GetMatrix();
	Vector2 ToGlobal(Vector2 &point);
	Vector2 ToLocal(Vector2 &point);
	//Get the unit vector pointing in the forward dir of the entity
	Vector2 GetForward()
	{
		Vector2 vec = Vector2(0, 1);
		return ToGlobal(vec) - GetPos();
	};
	//Unit Vector pointing in the right dir of the entity
	Vector2 GetRight()
	{
		Vector2 vec = Vector2(1, 0);
		return ToGlobal(vec) - GetPos();
	}

	virtual void SetPos(Vector2 p) {mPosition = p; mMatrixNeedsUpdate = true;};
	void SetPos(float x, float y) {SetPos(Vector2(x,y)); mMatrixNeedsUpdate = true;};
	Vector2 GetPos() {return mPosition;};

	void SetOrigin(Vector2 Or) {mOrigin = Or; mMatrixNeedsUpdate = true;};
	Vector2 GetOrigin() {return mOrigin;};

	virtual void SetAngle(float a) {mAngle = a; mMatrixNeedsUpdate = true;};
	float GetAngle() {return mAngle;};

	Vector2 GetSize() {return mSprite->GetSize();};

	BaseObject* CreateEntity(const char* classname);
	void SetOwner(BaseObject* own) {mOwner = own;};
	BaseObject* GetOwner() {return mOwner;};

	//Model
	void SetModel(const char* path);
	void SetModel(const char* path, float scale);
	const char* GetModel() {return mModel;};

	//Sound
	void CreateSound(const char *name, const char *path);
	void EmitSound(const char *name);
	
	//MetaData (UNFINISHED)
	void SetMetaData(const char* ID, float dat);
	void SetMetaData(const char* ID, int dat);
	void SetMetaData(const char* ID, BaseObject* dat);

	//Deletion
	void Delete() {
		mDeleteMe = true;
		OnDelete();
	};
	bool FlaggedForDeletion() {return mDeleteMe;};

	//Physics
	bool IsPhysicsEnabled() {return mIsPhysics;};
	PhysicsDef* GetPhysObj() {return mPhysObj;};

	//Rendering
	void RenderInit() {mIsRenderable = true;};
	bool IsRenderable() {return mIsRenderable;};
	void SetDrawOrder(RenderGroup o) {mDrawOrder = o;};
	RenderGroup GetDrawOrder() {return mDrawOrder;};
	void SetColor(Colour col) {mColour = col;};
	Colour GetColor() {return mColour;};
	void SetNoDraw(bool nodraw) {mNoDraw = nodraw;};
	bool GetNoDraw() {return mNoDraw;};

	//IO
	void Fire(const char* Name, VariantMap &Data);
	void RegisterInput(const char* Name, InputFunc Func);

	void DrawModel();

	//Hooks
	virtual void Spawn() {};
	virtual void Tick();
	virtual void Think() {};
	virtual void OnDelete() {}; //Called before the entity is deleted
	virtual void Draw(); //Draw your sprites/Drawables here
	virtual void StartTouch(CollisionInfo* info) {}; //When another entity begins to touch us
	virtual void Use(BaseObject* ply) {}; //Player has pressed E on us!

};

template<class T>
struct RegisterEnt
{
	RegisterEnt(const char* name)
	{
		BaseRenderableFactory* F = new EntityFactory<T>;
		ENTITYCREATOR->RegisterEntity(name, F);
	}
};

#define LINKCLASSTONAME(name, t) \
	static RegisterEnt<t> Regi(name);
