#include "Lighting.h"
#include "../Bases/BaseObject.h"
#include "../PhysicsDef.h"
#include "../Profiler.h"
#include "../Camera.h"
#include "../../Game/effect_light.h"


#define AMBIENT_LIGHT 100.f
#define MAX_LIGHTS 8 //If this is high LOW FRAMES :<
#define LIGHT_TEX_SCALE 4.f //The higher this is the lower resolution the lighting images are
#define FINAL_TEX_SCALE 2.f //DOESNT WORK YET

Lighting::Lighting(void)
{
	//Texture used to draw shadows
	sf::Image BlackImg;
	BlackImg.create(1,1, sf::Color::Black);
	mBlackTex.loadFromImage(BlackImg);
	BlackImg.create(1,1, sf::Color(AMBIENT_LIGHT,AMBIENT_LIGHT,AMBIENT_LIGHT,255));

	mFinalTexture.create(gGlobals.GameWidth / FINAL_TEX_SCALE, gGlobals.GameHeight / FINAL_TEX_SCALE, false);
	mFinalTexture.setSmooth(true);
	mFinalSprite.setTexture(mFinalTexture.getTexture());
	mFinalSprite.setScale(FINAL_TEX_SCALE,FINAL_TEX_SCALE);
	mFinalSprite.setOrigin(gGlobals.GameWidth/2, gGlobals.GameHeight/2);
	mFinalSprite.setPosition(Vector2(gGlobals.GameWidth, gGlobals.GameHeight).SF());

	mBlurShader.loadFromFile("shaders/blur.frag", sf::Shader::Fragment);
	mLightShader.loadFromFile("shaders/light_falloff.frag", sf::Shader::Fragment);
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

void Lighting::DrawLight(LightInfo *light, sf::RenderTexture* tex)
{
	//Draw Lights
	sf::RenderStates light_state;
	light_state.shader = &mLightShader;
	light_state.blendMode = sf::BlendAdd;

	Vector2 coord = light->GetPosition();
	coord = ig::GameToSFML(coord);
	light->mRealTimeSprite.setColor(light->GetColour());
	light->mRealTimeSprite.setPosition(coord.SF());
	light->mLightSprite.setRotation(-light->GetAngle());
	light->mRealTimeTexture.draw(*light->GetSprite(),light_state);
}

sf::Vector2f ConvertCoords(Vector2 pos, Vector2 spr_pos)
{
	pos.y *= -1.f;
	pos = pos * 0.5f;
	pos = pos - (spr_pos * 0.5f);
	pos = pos + Vector2(615,482); // THIS IS JUST AWFUL DONT DO THIS EVER OH DEAR
	return pos.SF();
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
		vert.position = ConvertCoords(vertex_pos, light->mRealTimeSprite.getPosition());
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MinDotPos + MinDotDir * (512.f / (1-ig::Abs(MinDot)));
		vert.position = ConvertCoords(vertex_pos, light->mRealTimeSprite.getPosition());
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MaxDotPos + MaxDotDir * (512.f / (1-ig::Abs(MaxDot)));
		vert.position = ConvertCoords(vertex_pos, light->mRealTimeSprite.getPosition());
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		vertex_pos = MaxDotPos;
		vert.position = ConvertCoords(vertex_pos, light->mRealTimeSprite.getPosition());
		vert.texCoords = sf::Vector2f(0,0);
		shadowhull.append(vert);

		light->mRealTimeTexture.draw(shadowhull,state);
		i = ShadowCasters.NextEnt(i);
	}
}

void Lighting::UpdateLightingTexture(sf::View &view)
{
	mView = &view;
	Profiler::StartRecord(PROFILE_TEMPORARY_1);

	if (ig::RandomInt(0,100) == 0) // THIS IS UNFORGIVABLE
		std::cout << "LIGHT COUNT: " << mLights.GetSize() << "\n";

	mFinalTexture.setView(view);

	float speed = 1.f / (3.f * 60.f);
	float progress = std::cos(gGlobals.CurTime * speed * 3.14159265f * 2.f);

	Colour DayColour = Colour(245,235,190);
	Colour NightColour = Colour(35,35,125);
	DayColour = NightColour;

	float day_night = (0.5f * progress) + 0.5f;
	//std::cout << day_night << "\n";

	Colour LerpColour;
	LerpColour.r = (DayColour.r * day_night) + (NightColour.r * (1 - day_night));
	LerpColour.g = (DayColour.g * day_night) + (NightColour.g * (1 - day_night));
	LerpColour.b = (DayColour.b * day_night) + (NightColour.b * (1 - day_night));

	mFinalTexture.clear(LerpColour.SF()); // Clear the main texture to ambient

	// This is pretty messy to use 3 iterators...
	EntityList<BaseObject*>::iter CurPos = mLights.FirstEnt();

	sf::RenderStates blend_add;
	blend_add.blendMode = sf::BlendAdd; // Additive so one lights shadows dont overwrite another lights luminosity

	while (CurPos != mLights.End())
	{
		effect_light* CurLight = dynamic_cast<effect_light*>(*CurPos);
		DrawLight(CurLight->GetLight(), NULL);
		DrawShadows(CurLight->GetLight(), NULL);

		CurLight->GetLight()->mRealTimeTexture.display(); // Swap the frame buffers to make this light display properly

		// Draw the texture to the final lightmap
		mFinalTexture.draw(CurLight->GetLight()->mRealTimeSprite, blend_add);
		
		CurLight->GetLight()->mRealTimeTexture.clear(sf::Color::Black);
		CurPos = mLights.NextEnt(CurPos);
	}

	mFinalTexture.display(); // Swap framebuffer
	// Clear all references ready to be rebuilt for the next frame
	// We do this because the lighting engine only handles on screen ents, this is rebuilt everyframe
	ShadowCasters.ClearDontDelete();
	mLights.ClearDontDelete();
	Profiler::StopRecord(PROFILE_TEMPORARY_1);
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