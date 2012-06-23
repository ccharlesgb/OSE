#pragma once

#include "Bases/BaseObject.h"

class sCamera
{
private:
	static BaseObject* mTarget;
	static Vector2 mTargetPos;
	static float mDamping;
	static float mFollowSpeed;
	static bool mIsFollowing;
	static float mRot;
	static float mZoom;
	static float mTargetZoom;
	static float mZoomDamping;
	static Vector2 mCentre;
public:
	static void SetZoom(float z) {mTargetZoom = z;};
	static float GetZoom() {return mZoom;};
	static void SetZoomDamping(float z) {mZoomDamping = z;};
	static float GetZoomDamping() {return mZoomDamping;};

	static void ReSize(Vector2 Size);
	static void SetCentre(Vector2 &pos) {mCentre = pos;};
	static Vector2 GetCentre() {return mCentre;};
	static void SetRotation(float rot) {mRot = rot;};
	static float GetRotation() {return mRot;};
	static void Rotate(float amount) {SetRotation(GetRotation() + amount);};
	static void FollowEntity(BaseObject* ent) {mTarget=ent; mIsFollowing = true;};
	static bool IsFollowing() {return mIsFollowing;};
	static void SetDamping(float damp) {mDamping = damp;};
	static float GetDamping() {return mDamping;};
	static void UpdateView();
};

