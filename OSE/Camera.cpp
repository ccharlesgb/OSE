#include "Camera.h"
#include "GameGlobals.h"

BaseObject* sCamera::mTarget;
Vector2 sCamera::mTargetPos;
float sCamera::mDamping;
float sCamera::mFollowSpeed;
bool sCamera::mIsFollowing;
float sCamera::mRot;
float sCamera::mZoom;
float sCamera::mTargetZoom;
float sCamera::mZoomDamping;
Vector2 sCamera::mCentre;

void sCamera::UpdateView()
{
	if (IsFollowing())
	{
		mTargetPos = mTarget->GetPos();
	}
	if (mZoomDamping = 0)
	{
		mZoom = mTargetZoom;
	}
	else
	{
		float speed = std::abs(mZoom - mTargetZoom);
		mZoom = ig::Approach(mZoom, mTargetZoom, speed * gGlobals.FrameTime * 8.f);
	}
	if (mDamping > 0)
	{
		double Rate = (GetCentre()-mTargetPos).Length() * 0.1f;
		Vector2 NewCentre = GetCentre().Approach(mTargetPos, Rate);
		//std::cout << gGlobals.FrameTime << "\n";
		SetCentre(NewCentre);
	}
	else
		SetCentre(mTargetPos);
}
