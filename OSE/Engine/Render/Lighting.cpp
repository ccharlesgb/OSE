#include "Lighting.h"
#include "../Bases/BaseObject.h"
#include "../PhysicsDef.h"
#include "../Profiler.h"
#include "../Camera.h"

Lighting::Lighting(void)
{
	float Ambient = 50;
	sf::Image BlackImg;
	BlackImg.create(1,1, sf::Color::Black);
	mBlackTex.loadFromImage(BlackImg);
	BlackImg.create(1440,900, sf::Color(Ambient,Ambient,Ambient,255));
	mLightingFinal.loadFromImage(BlackImg);

	//mLightingSprite.setTexture(mLightingFinal);

	mCasterTexture.create(1440,900, false);

	mLightingSprite.setTexture(mCasterTexture.getTexture());
	mLightingSprite.setOrigin(1440/2, 900/2);
	gGlobals.gEntList.RegisterListener(this);
}

Lighting::~Lighting(void)
{
}

void Lighting::OnEntityAdded(BaseObject* ent)
{
	if (ent->GetCastShadows())
	{
		ShadowCasters.Append(ent);
	}
}

void Lighting::OnEntityRemoved(BaseObject* ent)
{
	if (ent->GetCastShadows())
	{
		EntityList<BaseObject*>::iter CurIter = ShadowCasters.FirstEnt();
		while (CurIter != ShadowCasters.End())
		{
			if (ent == (*CurIter))
			{
				CurIter = ShadowCasters.DeleteSoft(CurIter);
				return;
			}
			else
				CurIter = ShadowCasters.NextEnt(CurIter);
		}
	}
}

void Lighting::UpdateLightingTexture(sf::View &view)
{
	mCasterTexture.setView(view);
	Profiler::StartRecord(PROFILE_RENDER_LIGHTS);
	mCasterTexture.clear(sf::Color::White);

	EntityList<BaseObject*>::iter i = ShadowCasters.FirstEnt();
	sf::RenderStates state;
	state.texture = &mBlackTex;
	while(i != ShadowCasters.End())
	{
		sf::VertexArray shadowhull;
		shadowhull.setPrimitiveType(sf::TrianglesFan);
		BaseObject* CurEnt = *i;

		PolygonShape* Hull = CurEnt->GetPhysObj()->mHullShape;
		for (int vert_ind=0; vert_ind < Hull->GetVertexCount(); vert_ind++)
		{
			Vector2 vert_pos = Hull->mVertices[vert_ind];
			vert_pos = CurEnt->ToGlobal(vert_pos);
			vert_pos = vert_pos + Vector2(1440/2, 450);
			vert_pos = vert_pos + Vector2(0.f, -2 * sCamera::GetCentre().y);
			sf::Vertex vert;
			vert.position = vert_pos.SF();
			vert.texCoords = sf::Vector2f(0,0);
			shadowhull.append(vert);
		}

		//mRender->draw(shadowhull,state);
		mCasterTexture.draw(shadowhull, state);
		i = ShadowCasters.NextEnt(i);
	}
	Profiler::StopRecord(PROFILE_RENDER_LIGHTS);
	mLightingSprite.setPosition(Vector2(1440/2, 450).SF());
}
