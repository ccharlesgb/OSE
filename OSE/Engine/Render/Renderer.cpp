#include "Renderer.h"
#include "../IGameState.h"
#include "../InputHandler.h"
#include "../Camera.h"
#include <Box2D/Collision/b2Collision.h>
#include "../Bases/BaseObject.h"
#include "../Profiler.h"

Renderer* Inst = NULL;

#define BASECOLOUR sf::Color::Black

Renderer* Renderer::Instance()
{
	if (Inst == NULL)
	{
		Inst = new Renderer;
	}
	return Inst;
}

Renderer::Renderer(void)
{
	//mLightSystem = new ltbl::LightSystem();
	gGlobals.gEntList.RegisterListener(this);
}

Renderer::~Renderer(void)
{
	Renderables.Clear();
}

void Renderer::SetWindow(sf::RenderWindow *wind)
{
	mRender = wind;
	mView.setSize((float)gGlobals.GameWidth, (float)gGlobals.GameHeight);
	//mView.Zoom(1/RENDER_SCALE);
	mPhysDebug = new DebugDraw(wind);
	gGlobals.PhysicsDebugDraw = mPhysDebug;
};

void Renderer::Cleanup()
{
	delete Inst;
}

Vector2 Renderer::GameToSFML(Vector2 Pos)
{
	return ig::GameToSFML(Pos);
}

Vector2 Renderer::ToScreen(Vector2 pos)
{
	Vector2 ScreenCentre = sf::Vector2f((float)mRender->getSize().x, (float)mRender->getSize().y);
	pos.y *= -1.f;
	pos = pos + ScreenCentre - mView.getCenter();
	return pos;
}

void Renderer::OnResize()
{
	gGlobals.GameWidth = mRender->getSize().x;
	gGlobals.GameHeight = mRender->getSize().y;
	Vector2 Size = Vector2(gGlobals.GameWidth, gGlobals.GameHeight);
};

//Called whenever ANY entity is added to the game. Check if its renderable then add it to a list
void Renderer::OnEntityAdded(BaseObject* ent)
{
	if (ent->IsRenderable())
	{
		AddEntity(ent);
	}
	//BaseRender* render = ent->GetRenderer();
	//AddRenderer(render);
}

void Renderer::AddEntity(BaseObject* Ent)
{
	//Insert the renderer into its correct draw position
	BaseObject* iter = Renderables.FirstEnt();
	bool FoundGroupBelow = false; //Have we found the correct position?
	
	if (Renderables.GetSize() > 0)
	{
		do
		{
			//If we should be drawing after the current renderer
			if (Ent->GetDrawOrder() >= iter->GetDrawOrder())
			{
				iter = Renderables.NextEnt(); //Move on we want to draw after this one
			}
			else
			{
				FoundGroupBelow = true;
			}
		} 
		while (iter != NULL && !FoundGroupBelow);
	}
	
	// FoundGroupBelow means iterator is at the next group, so we need to move it back 1 step.
	if (FoundGroupBelow)
	{
		--iter;
	}
	Renderables.InsertAtCurrent(Ent);
}

void Renderer::OnEntityRemoved(BaseObject* ent)
{
	if (ent->IsRenderable())
	{
		Renderables.Delete(ent);
	}
}

#define GRID_SIZE 256

void Renderer::UpdateOnScreenList()
{
	Profiler::StartRecord(PROFILE_RENDER_PURGE);
	OnScreenEnts.ClearDontDelete();
	BaseObject* CurEnt = Renderables.FirstEnt();
	while(CurEnt != NULL)
	{
		if (!CurEnt->GetNoDraw())
		{
			Vector2 EntPos = CurEnt->GetPos();
			Vector2 Quadrant; //Quantise the position a grid size TEXTURE_SIZE
			Quadrant.x = std::floor((EntPos.x / GRID_SIZE) + 0.5f) - 0.5f;
			Quadrant.y = std::floor((EntPos.y / GRID_SIZE) + 0.5f) - 0.5f;

			Vector2 CamPos = sCamera::GetCentre();
			Vector2 ScreenQuadrant;
			ScreenQuadrant.x = std::floor((CamPos.x / GRID_SIZE) + 0.5f) - 0.5f;
			ScreenQuadrant.y = std::floor((CamPos.y / GRID_SIZE) + 0.5f) - 0.5f;

			if (std::abs(ScreenQuadrant.x - Quadrant.x) < 3 && std::abs(ScreenQuadrant.y - Quadrant.y) < 3)
			{
				OnScreenEnts.Append(CurEnt);
			}
		}
		CurEnt = Renderables.NextEnt();
	}
	Profiler::StopRecord(PROFILE_RENDER_PURGE);
}

void Renderer::Draw(IGameState *State)
{
	Clear();
	sCamera::UpdateView();
	mView.setCenter(GameToSFML(sCamera::GetCentre()).SF());
	mView.setRotation(sCamera::GetRotation());
	mView.setSize(Vector2(mRender->getSize().x * sCamera::GetZoom(),mRender->getSize().y * sCamera::GetZoom()).SF());

	mRender->setView(mView);

	UpdateOnScreenList();//Update the OnScreenEntityList

	BaseObject* CurEnt = OnScreenEnts.FirstEnt();
	while(CurEnt != NULL)
	{
		if (!CurEnt->GetNoDraw())
		{
			CurEnt->Draw();
		}
		CurEnt = OnScreenEnts.NextEnt();
	}
	//mLightSystem->RenderLights();
	//mLightSystem->RenderLightTexture();
	if (InputHandler::IsKeyPressed(sf::Keyboard::F3))
		State->DrawDebugData();
	mRender->setView(mRender->getDefaultView());
}
