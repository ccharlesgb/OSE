#include "Lighting.h"
#include "../Bases/BaseObject.h"
#include "../PhysicsDef.h"
#include "../Profiler.h"
#include "../Camera.h"
#include "../../Game/effect_light.h"


#define AMBIENT_LIGHT 100.f
#define MAX_LIGHTS 3
#define LIGHT_TEX_SCALE 1.f //The higher this is the lower resolution the lighting images are
#define FINAL_TEX_SCALE 1.f //DOESNT WORK YET

Lighting::Lighting(void)
{
	float Ambient = 50;
	sf::Image BlackImg;
	BlackImg.create(1,1, sf::Color::Black);
	mBlackTex.loadFromImage(BlackImg);
	BlackImg.create(1,1, sf::Color(Ambient,Ambient,Ambient,255));

	float width = gGlobals.GameWidth;
	float height = gGlobals.GameHeight;
	mFinalTexture.create(gGlobals.GameWidth / FINAL_TEX_SCALE, gGlobals.GameHeight / FINAL_TEX_SCALE, false);
	mFinalTexture.setSmooth(true);
	mFinalSprite.setTexture(mFinalTexture.getTexture());
	mFinalSprite.setScale(FINAL_TEX_SCALE,FINAL_TEX_SCALE);
	mFinalSprite.setOrigin(gGlobals.GameWidth/2, gGlobals.GameHeight/2);
	mFinalSprite.setPosition(Vector2(gGlobals.GameWidth/2, gGlobals.GameHeight/2).SF());

	mBlurShader.loadFromFile("shaders/blur.frag", sf::Shader::Fragment);
	mLightShader.loadFromFile("shaders/light_falloff.frag", sf::Shader::Fragment);

	//Create 3 light textures for lights to draw their shit on
	//These will be merged into one and then displayed by the renderer
	sf::RenderTexture* rend_tex;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		rend_tex = new sf::RenderTexture();
		rend_tex->create(gGlobals.GameWidth / LIGHT_TEX_SCALE, gGlobals.GameHeight / LIGHT_TEX_SCALE, false);
		rend_tex->clear(sf::Color::Black);
		rend_tex->setSmooth(true);
		mLightTextures.push_back(rend_tex);
	}
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
	if (ent->GetClassName() == "effect_light")
	{
		mLights.Append(ent);
	}
}

void Lighting::OnEntityRemoved(BaseObject* ent)
{
	if (ent->GetClassName() == "effect_light")
	{
		EntityList<BaseObject*>::iter CurIter = mLights.FirstEnt();
		while (CurIter != mLights.End())
		{
			if (ent == (*CurIter))
			{
				CurIter = mLights.DeleteSoft(CurIter);
				return;
			}
			else
				CurIter = mLights.NextEnt(CurIter);
		}
	}
}

sf::Vector2f ConvertCoords(Vector2 coord)
{
	coord.y *= -1;
	coord = coord + Vector2(gGlobals.GameWidth/2, gGlobals.GameHeight/2);
	return coord.SF();
}

void Lighting::DrawLight(LightInfo *light, sf::RenderTexture* tex)
{
	//Draw Lights
	sf::RenderStates light_state;
	light_state.shader = &mLightShader;
	light_state.blendMode = sf::BlendAdd;
	light->GetSprite()->setColor(light->GetColour());
	light->GetSprite()->setPosition(ConvertCoords(light->GetPosition()));
	tex->draw(*light->GetSprite(),light_state);
}

void Lighting::DrawShadows(LightInfo *light, sf::RenderTexture* tex)
{
	EntityList<BaseObject*>::iter i = ShadowCasters.FirstEnt();
	sf::RenderStates state;
	state.texture = &mBlackTex;
	while(i != ShadowCasters.End())
	{	
		BaseObject* CurEnt = *i;

		float dist_sqr = (CurEnt->GetPos() - light->GetPosition()).LengthSquared(); //Avoid root!
		if (dist_sqr > light->GetRadius() * light->GetRadius()) //We arent giong to cast a shadow so dont bother
		{
			i = ShadowCasters.NextEnt(i);
			continue;
		}

		Vector2 LightToEnt = (CurEnt->GetPos() - light->GetPosition()).Normalize();

		float MaxDot = -1.f;
		Vector2 MaxDotPos, MinDotPos;
		Vector2 MaxDotDir, MinDotDir;
		float MinDot = 1.f;

		PolygonShape* Hull = CurEnt->GetPhysObj()->mHullShape;
		for (int vert_ind=0; vert_ind < Hull->GetVertexCount(); vert_ind++)
		{
			Vector2 vert_pos = Hull->mVertices[vert_ind];
			vert_pos = CurEnt->ToGlobal(vert_pos);
			Vector2 LightToVert = (vert_pos - light->GetPosition()).Normalize();

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
	
		//Define the coordinates for the shadow volume
		sf::VertexArray shadowhull;
		shadowhull.setPrimitiveType(sf::TrianglesFan);
		sf::Vertex vert;
		Vector2 vertex_pos = MinDotPos;
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MinDotPos + MinDotDir * (512.f / (1-std::abs(MinDot)));
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MaxDotPos + MaxDotDir * (512.f / (1-std::abs(MaxDot)));
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MaxDotPos;
		vert.position = ConvertCoords(vertex_pos);
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		tex->draw(shadowhull,state);
		i = ShadowCasters.NextEnt(i);
	}
}

void Lighting::UpdateLightingTexture(sf::View &view)
{

	mFinalTexture.clear(sf::Color(AMBIENT_LIGHT,AMBIENT_LIGHT,AMBIENT_LIGHT)); //Clear the main texture to ambient

	EntityList<BaseObject*>::iter CurPos = mLights.FirstEnt();
	std::vector<sf::RenderTexture*>::iterator CurTexPos = mLightTextures.begin();
	Profiler::StartRecord(PROFILE_TEMPORARY_1);
	while (CurPos != mLights.End() && CurTexPos != mLightTextures.end())
	{
		effect_light* CurLight = dynamic_cast<effect_light*>(*CurPos);
		(*CurTexPos)->setView(view);
		DrawLight(CurLight->GetLight(), *CurTexPos);
		DrawShadows(CurLight->GetLight(), *CurTexPos);

		CurTexPos++;
		CurPos = mLights.NextEnt(CurPos);
	}
	Profiler::StopRecord(PROFILE_TEMPORARY_1);
	//Render all the light textures to the final image
	Profiler::StartRecord(PROFILE_TEMPORARY_2); //THIS IS SLOW FIGURE OUT WHY LATER MAYBE PREMAKE ALL SPRITES?
	sf::RenderStates states;
	states.blendMode = sf::BlendAdd;
	CurTexPos = mLightTextures.begin();
	while (CurTexPos != mLightTextures.end())
	{
		sf::Sprite mDrawingSprite;
		(*CurTexPos)->display();
		mDrawingSprite.setTexture((*CurTexPos)->getTexture());
		//mDrawingSprite.setScale(LIGHT_TEX_SCALE, LIGHT_TEX_SCALE);

	
		mFinalTexture.draw(mDrawingSprite,states);
		
		(*CurTexPos)->clear(sf::Color::Black);

		CurTexPos++;
	}
	mFinalTexture.display();
	Profiler::StopRecord(PROFILE_TEMPORARY_2);
	ShadowCasters.ClearDontDelete();
	mLights.ClearDontDelete();
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