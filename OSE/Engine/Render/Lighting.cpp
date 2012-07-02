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

sf::Vector2f ConvertCoords(Vector2 coord)
{
	coord = coord + Vector2(1440/2, 450);
	coord = coord + Vector2(0.f, -2 * sCamera::GetCentre().y);
	return coord.SF();
}

void Lighting::UpdateLightingTexture(sf::View &view)
{
	mCasterTexture.setView(view);
	Profiler::StartRecord(PROFILE_RENDER_LIGHTS);
	mCasterTexture.clear(sf::Color(100,100,100));
	EntityList<BaseObject*>::iter i = ShadowCasters.FirstEnt();
	sf::RenderStates state;
	state.texture = &mBlackTex;

	Vector2 LightPos = sCamera::GetCentre();
	while(i != ShadowCasters.End())
	{
		sf::VertexArray shadowhull;
		shadowhull.setPrimitiveType(sf::TrianglesFan);

		BaseObject* CurEnt = *i;
		Vector2 LightToEnt = (CurEnt->GetPos() - LightPos).Normalize();

		float MaxDot = -1000;
		Vector2 MaxDotPos, MinDotPos;
		Vector2 MaxDotDir, MinDotDir;
		float MinDot = 1000;

		PolygonShape* Hull = CurEnt->GetPhysObj()->mHullShape;
		for (int vert_ind=0; vert_ind < Hull->GetVertexCount(); vert_ind++)
		{
			Vector2 vert_pos = Hull->mVertices[vert_ind];
			vert_pos = CurEnt->ToGlobal(vert_pos);
			Vector2 LightToVert = (vert_pos - LightPos).Normalize();

			float VertDotPos = LightToEnt.Cross(LightToVert);
			if (VertDotPos < MinDot)
			{
				MinDot = VertDotPos;
				MinDotPos = vert_pos;
				MinDotDir = LightToVert;
			}
			if (VertDotPos > MaxDot)
			{
				MaxDot = VertDotPos;
				MaxDotPos = vert_pos;
				MaxDotDir = LightToVert;
			}
		}
		//std::cout << MinDot << "   " << MaxDot << "\n";

		sf::Vertex vert;
		Vector2 vertex_pos = MinDotPos;
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MinDotPos + MinDotDir * 200.f;
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MaxDotPos + MaxDotDir * 200.f;
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MaxDotPos;
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		//mRender->draw(shadowhull,state);
		mCasterTexture.draw(shadowhull,state);
		i = ShadowCasters.NextEnt(i);
	}
	Profiler::StopRecord(PROFILE_RENDER_LIGHTS);
	mLightingSprite.setPosition(Vector2(1440/2, 450).SF());
}


//Code for shader based lighting may come in useful one day!
/*
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
*/