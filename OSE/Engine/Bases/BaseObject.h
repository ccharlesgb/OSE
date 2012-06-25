#pragma once
#include "../Utilities/MathUtils.h"
#include "../Utilities/VariantMap.h"
#include <iostream>
#include "../EntityCreator.h"
#include "../Utilities/Vector2.h"
#include "../Utilities/Matrix3.hpp"
#include "../Render/Sprite.h"
#include "../Sound.h"
#include "../Model.h"

class PhysicsDef;
class BaseObject;
class CollisionInfo;

typedef void (*InputFunc)(BaseObject* ent, VariantMap &Data);

enum DamageType
{
	DAMAGETYPE_GENERIC,
	DAMAGETYPE_BULLET,
	DAMAGETYPE_BURNING,
	DAMAGETYPE_DROWNING,
	DAMAGETYPE_PHYSICS,
	DAMAGETYPE_ENVIRONMENT
};

class BaseObject;

class DamageInfo
{
public:
	DamageInfo() {Amount = 0.f; Inflictor = NULL; type = DAMAGETYPE_GENERIC;};
	BaseObject* Inflictor;
	float Amount;
	DamageType type;
};

enum RenderGroup
{
	RENDERGROUP_BACKGROUND, //Draw first
	RENDERGROUP_ENTITIES,
	RENDERGROUP_PLAYER,
	RENDERGROUP_HUD //Draw Last
};

/**
 * Base entity that all entities in game are derived from.
 *
 * @ingroup engine
 */
class BaseObject
{
private:
	std::map<std::string, InputFunc> Inputs; //Entity IO function pointers
	VariantMap mMetaData; //Variables that other entities can access
	bool mDeleteMe; //Should the entity be deleted on the next entity purge?

	//Thinking
	float mNextThink;
	float mLastThink;
	bool mShouldThink;

	std::string mClassName; //The class name of the entity eg "player"

	std::map<char, Sound*> mSounds;
	
	//Rendering
	Colour mColour; //The colour of the entity, only affects Model automatically
	Sprite* mSprite; //The sprite object used in DrawModel/SetModel etc
	bool mNoDraw; //Enable disable calling of Draw();


	BaseObject *mParent; //Move relative to this entity
	Vector2 mParentRelativePos; //The position that we should force to maintain with parent

	BaseObject* mOwner; //What entity 'owns' us? Doesn't actually do anything just a useful thing to have
protected:
	PhysicsDef *mPhysObj; //Pointer to the physics object for this entity (NOT NECCESSARILY VALID)
	Matrix3 mMatrix; //Transformation matrix used by ToGlobal and ToLocal
	bool mMatrixNeedsUpdate; //If the matrix needs to be updated (Position/rotation etc has changed)

	//MODEL
	const char* mModel; //Model path (Sprite image)
	Model mModelInfo;

	Vector2 mPosition;
	Vector2 mOrigin;
	Vector2 mScale;
	float mAngle;

	//Health
	float mHealth;
	float mMaxHealth;

	RenderGroup mDrawOrder;
	bool mIsRenderable; //Do we have a renderer?
	bool mIsPhysics; //Do we have a physics object?
public:

	/**
	 * Default constructor
	 */
	BaseObject(void);
	virtual ~BaseObject(void);

	/**
	 * Internal function that sets the class name of the entity,
	 * DO NOT CALL THIS MANUALLY!
	 * @param name The classname of the entity
	 */
	void SetClassName(const char* name)
	{
		if (mClassName == "ERROR") //Only do this once!
			mClassName = name;
	};
	/**
	 * Returns the class name of the entity. eg "ent_prop"
	 */
	std::string GetClassName() {return mClassName;};

	/**
	 * Returns the transformation matrix of the entity
	 */
	Matrix3 GetMatrix();
	/**
	 * Convert a vector in the entities local coordinate system to
	 * the global coordinate system.
	 * @param point The local vector that needs to be converted
	 */
	Vector2 ToGlobal(Vector2 &point);
	/**
	 * Convert a vector in the global coordinate system to
	 * the entities local coordinate system
	 * @param point The global vector that needs to be converted
	 */
	Vector2 ToLocal(Vector2 &point);
	/**
	* Get the unit vector pointing in the forward directon of
	* the entity in global space
	*/
	Vector2 GetForward()
	{
		Vector2 vec = Vector2(0, 1);
		return ToGlobal(vec) - GetPos();
	};
	/**
	* Get the unit vector pointing in the right directon of
	* the entity in global space
	*/
	Vector2 GetRight()
	{
		Vector2 vec = Vector2(1, 0);
		return ToGlobal(vec) - GetPos();
	}
	/**
	* Set the position of the entity (and its physics object if it
	* exists)
	* @param p The position to set the entity to
	*/
	virtual void SetPos(Vector2 p) {mPosition = p; mMatrixNeedsUpdate = true;};
	void SetPos(float x, float y) {SetPos(Vector2(x,y)); mMatrixNeedsUpdate = true;};
	/**
	* Return the position of the entity
	*/
	Vector2 GetPos() {return mPosition;};

	/**
	* Set what the entity deems to be its center. The entity will
	* rotate around this point
	* @param origin The origin of the entity in LOCAL space
	*/
	void SetOrigin(Vector2 origin) {mOrigin = origin; mMatrixNeedsUpdate = true;};
	/**
	* Get the origin of the entity returns the LOCAL origin.
	*/
	Vector2 GetOrigin() {return mOrigin;};
	/**
	* Set the angle of the entity
	* @param angle The desired angle in degrees
	*/
	virtual void SetAngle(float angle) {mAngle = angle; mMatrixNeedsUpdate = true;};
	/**
	* Get the angle of the entity in degrees
	*/
	float GetAngle() {return mAngle;};

	/**
	* Get the local AABB of the entities model
	*/
	Vector2 GetSize() {return mSprite->GetSize();};

	/**
	* Create an entity and return a reference to the entity if
	* successful. Returns NULL if not.
	* @param classname The classname of the entity to create
	* eg: 'ent_prop'
	*/
	BaseObject* CreateEntity(const char* classname);

	/**
	* Set the entity that 'owns' this entity. This doesn't actually do
	* anything yet, however some entities can use this property to
	* keep track of which entity is controllin them/created them.
	* @param owner A pointer to the entity that will be the owner.
	*/
	void SetOwner(BaseObject* owner) {mOwner = owner;};
	/**
	* Returns the entity that 'owns' the entity. NULL if invalid.
	*/
	BaseObject* GetOwner() {return mOwner;};

	//Parent
	/**
	* Sets the parent. The entity will move with its parent, and
	* maintain relative position and angle with the parent.
	* @param parent The parent entity, pass NULL to remove parent.
	*/
	void SetParent(BaseObject* parent);
	/**
	* Returns the parent entity.
	*/
	BaseObject* GetParent() {return mParent;};

	//Model
	/**
	* Sets the model of an entity. TODO: MODEL FORMAT
	* @param path the filepath of the model
	*/
	void SetModel(const char* path);
	/**
	* Sets the model of an entity. TODO: MODEL FORMAT
	*/
	void SetModel(const char* path, float scale);
	/**
	* Gets the model of an entity. TODO: MODEL FORMAT
	*/
	const char* GetModel() {return mModel;};

	//Sound
	/**
	* Create a sound object that can be played by the entity.
	* @param name The ID name that the sound will be referred to as.
	* @param path The filepath of the sound file.
	*/
	void CreateSound(const char *name, const char *path);
	/**
	* Emit a sound originating from the entity. Soud must have been
	* created by CreateSound() before this function can be called.
	* @param name The ID of the sound to be played
	*/
	void EmitSound(const char *name);
	/**
	* Returns the Sound object associated with this name
	* @param name The ID of the sound to be retrieved
	*/
	Sound* GetSound(const char *name);

	//Health
	void SetHealth(const float hlt) {mHealth = hlt;};
	float GetHealth() {return mHealth;};
	void SetMaxHealth(const float mhlt) {mMaxHealth = mhlt;};
	float GetMaxHealth() {return mMaxHealth;};

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

	//Thinking
	void SetNextThink(float time) {mNextThink = time;};
	float GetNextThink() {return mNextThink;};
	void ShouldThink(bool think) {mShouldThink = think;};
	bool IsThinking() {return mShouldThink;};

	//Physics
	bool IsPhysicsEnabled() {return mIsPhysics;};
	PhysicsDef* GetPhysObj() {return mPhysObj;};
	virtual void PhysicsSimulate(float delta) {};

	//Rendering
	void RenderInit() {mIsRenderable = true;};
	bool IsRenderable() {return mIsRenderable;};
	void SetDrawOrder(RenderGroup o) {mDrawOrder = o;};
	RenderGroup GetDrawOrder() {return mDrawOrder;};
	void SetColour(Colour col) {mColour = col;};
	Colour GetColour() {return mColour;};
	void SetNoDraw(bool nodraw) {mNoDraw = nodraw;};
	bool GetNoDraw() {return mNoDraw;};

	//IO
	void Fire(const char* Name, VariantMap &Data);
	void RegisterInput(const char* Name, InputFunc Func);

	void DrawModel();

	//DEBUG DRAWS (SLOW METHODS DONT USE THEM FOR RELEASE STUFF)
	void DebugDrawLine(Vector2 p1, Vector2 p2, const Colour &col);

	//Hooks
	virtual void Spawn() {};
	virtual void Tick();
	virtual void Think() {};
	virtual void OnDelete() {}; //Called before the entity is deleted
	virtual void Draw(); //Draw your sprites/Drawables here
	virtual void StartTouch(CollisionInfo* info) {}; //When another entity begins to touch us
	virtual void Use(BaseObject* ply) {}; //Player has pressed E on us!
	virtual void TakeDamage(const DamageInfo &info);
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
