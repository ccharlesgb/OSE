#pragma once
#include "MathUtils.h"
#include "VariantMap.h"
#include <iostream>
#include "EntityCreator.h"
#include "Vector2.h"
#include "SFML/Graphics.hpp"

class PhysicsDef;
class BaseRender;
class BaseObject;

typedef void (*InputFunc)(BaseObject* ent, VariantMap &Data);

class BaseObject
{
private:
	std::map<std::string, InputFunc> Inputs;
	VariantMap mMetaData;
	bool mDeleteMe;
	float mNextThink;
	float mLastThink;
	std::string mClassName;
	float mHealth;
	float mMaxHealth;
	bool mAlive;
protected:
	PhysicsDef *mPhysObj;
	BaseRender* mRender;
	sf::Matrix3 mMatrix;
	bool mMatrixNeedsUpdate;

	Vector2 mPosition;
	Vector2 mOrigin;
	float mAngle;
	bool mIsRenderable;
	bool mIsPhysics;
public:
	BaseObject(void);
	virtual ~BaseObject(void);

	void SetClassName(const char* ID)
	{
		if (mClassName == "ERROR")
			mClassName = ID;
	};
	std::string GetClassName() {return mClassName;};

	sf::Matrix3 GetMatrix();
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
	
	//MetaData
	void SetMetaData(const char* ID, float dat);
	void SetMetaData(const char* ID, int dat);
	void SetMetaData(const char* ID, BaseObject* dat);


	//Deletion
	void Delete() {mDeleteMe = true;};
	bool FlaggedForDeletion() {return mDeleteMe;};

	//Rendering
	bool IsRenderable() {return mIsRenderable;};
	BaseRender* GetRenderer() {return mRender;};

	//Physics
	bool IsPhysicsEnabled() {return mIsPhysics;};
	PhysicsDef* GetPhysObj() {return mPhysObj;};

	//Health
	void SetMaxHealth(float H) {mMaxHealth = H;};
	float GetMaxHealth() {return mMaxHealth;};
	void SetHealth(float H) {mHealth = H;};
	float GetHealth() {return mHealth;};
	void AddHealth(float am) {mHealth += am; if (mHealth > mMaxHealth) mHealth = mMaxHealth;};

	//IO
	void Fire(const char* Name, VariantMap &Data);
	void RegisterInput(const char* Name, InputFunc Func);

	//Hooks
	virtual void Spawn() {};
	virtual void Tick();
	virtual void Think() {};
	virtual void OnDelete() {}; //Called before the entity is deleted
	virtual void OnTakeDamage(BaseObject* inf, float Amount) {};
	virtual void PreDraw() {};
	virtual void PostDraw() {};

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
