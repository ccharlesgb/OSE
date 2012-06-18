#pragma once
#include "MathUtils.h"
#include "VariantMap.h"
#include <iostream>
#include "EntityCreator.h"
#include "Vector2.h"
#include "SFML/Graphics.hpp"
#include "Matrix3.hpp"
//#include "Sprite.h"

class PhysicsDef;
class BaseRender;
class BaseObject;

typedef void (*InputFunc)(BaseObject* ent, VariantMap &Data);

class BaseObject
{
private:
	std::map<std::string, InputFunc> Inputs; //Entity IO function pointers
	VariantMap mMetaData; //Variables that other entities can access
	bool mDeleteMe; //Should the entity be deleted on the next entity purge?
	float mNextThink;
	float mLastThink;
	std::string mClassName; //The class name of the entity eg "player"

	//TEMPORARY
	sf::Texture* mTexture;
	sf::Sprite* mSprite;
protected:
	PhysicsDef *mPhysObj; //Pointer to the physics object for this entity (NOT NECCESSARILY VALID)
	Matrix3 mMatrix; //Transformation matrix used by ToGlobal and ToLocal
	bool mMatrixNeedsUpdate; //If the matrix needs to be updated (Position/rotation etc has changed)
	const char* mModel; //Model path (Sprite image)

	Vector2 mPosition;
	Vector2 mOrigin;
	float mAngle;

	int mDrawOrder;
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

	virtual void SetPos(Vector2 p) {mPosition = p; mMatrixNeedsUpdate = true;};
	void SetPos(float x, float y) {SetPos(Vector2(x,y)); mMatrixNeedsUpdate = true;};
	Vector2 GetPos() {return mPosition;};

	void SetOrigin(Vector2 Or) {mOrigin = Or; mMatrixNeedsUpdate = true;};
	Vector2 GetOrigin() {return mOrigin;};

	virtual void SetAngle(float a) {mAngle = a; mMatrixNeedsUpdate = true;};
	float GetAngle() {return mAngle;};

	BaseObject* CreateEntity(const char* classname);

	//Model
	void SetModel(const char* path);
	const char* GetModel() {return mModel;};

	//MetaData (UNFINISHED)
	void SetMetaData(const char* ID, float dat);
	void SetMetaData(const char* ID, int dat);
	void SetMetaData(const char* ID, BaseObject* dat);

	//Deletion
	void Delete() {mDeleteMe = true;};
	bool FlaggedForDeletion() {return mDeleteMe;};

	//Physics
	bool IsPhysicsEnabled() {return mIsPhysics;};
	PhysicsDef* GetPhysObj() {return mPhysObj;};

	//Rendering
	bool IsRenderable() {return mIsRenderable;};
	void SetDrawOrder(int o) {mDrawOrder = o;};
	int GetDrawOrder() {return mDrawOrder;};

	//IO
	void Fire(const char* Name, VariantMap &Data);
	void RegisterInput(const char* Name, InputFunc Func);

	void DrawModel();

	//Hooks
	virtual void Spawn() {};
	virtual void Tick();
	virtual void Think() {};
	virtual void OnDelete() {}; //Called before the entity is deleted
	virtual void Draw();

};

template<class T>
struct RegisterEnt
{
	RegisterEnt(char* name)
	{
		BaseRenderableFactory* F = new EntityFactory<T>;
		ENTITYCREATOR->RegisterEntity(name, F);
	}
};

#define LINKCLASSTONAME(name, t) \
	static RegisterEnt<t> Regi(name);
