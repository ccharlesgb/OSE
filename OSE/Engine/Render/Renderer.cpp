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
	gGlobals.EnableRenderCulling = true;
}

Renderer::~Renderer(void)
{
	Renderables.ClearDontDelete();
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
	std::cout << "Resize\n";
	gGlobals.GameWidth = mRender->getSize().x;
	gGlobals.GameHeight = mRender->getSize().y;
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
	EntityList<BaseObject*>::iter CurIter = Renderables.FirstEnt();
	bool FoundGroupBelow = false; //Have we found the correct position?
	
	if (Renderables.GetSize() > 0)
	{
		do
		{
			//If we should be drawing after the current renderer
			if (Ent->GetDrawOrder() >= (*CurIter)->GetDrawOrder())
			{
				CurIter = Renderables.NextEnt(CurIter); //Move on we want to draw after this one
			}
			else
			{
				FoundGroupBelow = true;
			}
		} 
		while (CurIter != Renderables.End() && !FoundGroupBelow);
	}
	
	// FoundGroupBelow means iterator is at the next group, so we need to move it back 1 step.
	if (FoundGroupBelow)
	{
		//CurIter--;
	}
	Renderables.InsertAt(Ent, CurIter);
}

void Renderer::OnEntityRemoved(BaseObject* ent)
{
	if (ent->IsRenderable())
	{
		EntityList<BaseObject*>::iter CurIter = Renderables.FirstEnt();
		while (CurIter != Renderables.End())
		{
			if (ent == (*CurIter))
			{
				CurIter = Renderables.DeleteSoft(CurIter);
				return;
			}
			else
				CurIter = Renderables.NextEnt(CurIter);
		}
	}
}

void Renderer::UpdateOnScreenList()
{
	OnScreenEnts.ClearDontDelete();
	if (!gGlobals.EnableRenderCulling)
	{
		EntityList<BaseObject*>::iter CurEnt = Renderables.FirstEnt();
		while(CurEnt != Renderables.End())
		{
			OnScreenEnts.Append(*CurEnt);
			CurEnt = Renderables.NextEnt(CurEnt);
		}
		return;
	}

	EntityList<BaseObject*>::iter CurEnt = Renderables.FirstEnt();
	int GridsOnScreen = 1;
	int GRID_SIZE_X = gGlobals.GameWidth / GridsOnScreen;
	int GRID_SIZE_Y = gGlobals.GameHeight / GridsOnScreen;
	while(CurEnt != Renderables.End())
	{
		if (!(*CurEnt)->GetNoDraw())
		{
			Vector2_Rect ENT_AABB = (*CurEnt)->GetAABB();
			Vector2_Rect ENT_AABB_SCREENSPACE;
			ENT_AABB_SCREENSPACE.Position = ENT_AABB.Position - sCamera::GetCentre();
			ENT_AABB_SCREENSPACE.Size = ENT_AABB.Size; // / sCamera::GetZoom();

			Vector2 Bottom_Left_Quadrant = ig::NearestGrid(ENT_AABB_SCREENSPACE.Position, GRID_SIZE_X, GRID_SIZE_Y);
			Vector2 Top_Right_Quadrant	  = ig::NearestGrid(ENT_AABB_SCREENSPACE.Position + ENT_AABB_SCREENSPACE.Size, GRID_SIZE_X, GRID_SIZE_Y);
	
			//The size of the screen in our quantised coordinate system
			Vector2 SCREEN_BOTTOM_LEFT = Vector2(-GridsOnScreen, -GridsOnScreen) * sCamera::GetZoom();
			Vector2 SCREEN_TOP_RIGHT = Vector2(GridsOnScreen, GridsOnScreen) * sCamera::GetZoom();

			//Hideous Rectanlge intersection algorithm.
			//Check if the rectangles AABB is overlapping with the screens AABB
			if (!(SCREEN_BOTTOM_LEFT.x > Top_Right_Quadrant.x || SCREEN_TOP_RIGHT.x < Bottom_Left_Quadrant.x ||
				SCREEN_TOP_RIGHT.y < Bottom_Left_Quadrant.y || SCREEN_BOTTOM_LEFT.y > Top_Right_Quadrant.y))
			{
				OnScreenEnts.Append((*CurEnt));
			}
			else
			{
			}
		}
		CurEnt = Renderables.NextEnt(CurEnt);
	}
}

void Renderer::Draw(IGameState *State)
{
	Clear();

	sCamera::UpdateView();
	mView.setCenter(GameToSFML(sCamera::GetCentre()).SF());
	mView.setRotation(sCamera::GetRotation());
	mView.setSize(Vector2(mRender->getSize().x * sCamera::GetZoom(),mRender->getSize().y * sCamera::GetZoom()).SF());

	mRender->setView(mView);

	Profiler::StartRecord(PROFILE_RENDER_PURGE);
	UpdateOnScreenList(); // Update the OnScreenEntityList
	Profiler::StopRecord(PROFILE_RENDER_PURGE);

	Profiler::StartRecord(PROFILE_ENTITY_DRAW);
	//mLightSystem->RenderLights();
	//mLightSystem->RenderLightTexture();
	EntityList<BaseObject*>::iter CurEnt = OnScreenEnts.FirstEnt();
	while(CurEnt != OnScreenEnts.End())
	{
		if (!(*CurEnt)->GetNoDraw())
		{
			if ((*CurEnt)->GetDrawOrder() == RENDERGROUP_TOP)
			{
				float dist_squared = ((*CurEnt)->GetPos() - sCamera::GetCentre()).LengthSquared();
				Colour col = (*CurEnt)->GetColour();
				float alpha = 1 - (4000 / dist_squared);
				alpha  *= 255.f;
				col.a = ig::Limit(alpha, 0.f, 255.f);
				(*CurEnt)->SetColour(col);
			}
			(*CurEnt)->Draw();
		}
		CurEnt = OnScreenEnts.NextEnt(CurEnt);
	}
	Profiler::StopRecord(PROFILE_ENTITY_DRAW);

	if (InputHandler::IsKeyPressed(sf::Keyboard::F3))
		State->DrawDebugData();
	mRender->setView(mRender->getDefaultView());
	
	if (HUD != NULL)
	{
		HUD->Draw();
	}

	//DO THIS LAST UPDATE THE WHOLE SCREEN!
	Profiler::StartRecord(PROFILE_RENDER_DRAWCALL);
	Display();
	Profiler::StopRecord(PROFILE_RENDER_DRAWCALL);
}
