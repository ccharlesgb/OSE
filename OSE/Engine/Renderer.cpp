#include "Renderer.h"
#include "IGameState.h"
#include "InputHandler.h"
#include "Camera.h"
#include <Box2D/Collision/b2Collision.h>
#include "BaseObject.h"

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
	gGlobals.gEntList.RegisterListener(this);
}

Renderer::~Renderer(void)
{
	Renderables.Clear();
}

void Renderer::SetWindow(sf::RenderWindow *wind)
{
	mRender = wind;
	mView.setSize(gGlobals.GameWidth, gGlobals.GameHeight);
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
	if (mRender == NULL)
	{
		return sf::Vector2f(0,0);
	}
	sf::Vector2<double> dPos;
	dPos.x = Pos.x;
	dPos.y = Pos.y;
	dPos.y *= -1;
	sf::Vector2<double> ScreenCentre = sf::Vector2<double>(gGlobals.GameWidth / 2.f, gGlobals.GameHeight/ 2.f);
	dPos = dPos + ScreenCentre;

	Pos.x = dPos.x;
	Pos.y = dPos.y;

	return Pos;
}

Vector2 Renderer::ToScreen(Vector2 pos)
{
	Vector2 ScreenCentre = sf::Vector2f(mRender->getSize().x, mRender->getSize().y);
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

void Renderer::Draw(IGameState *State)
{
	Clear();
	sCamera::UpdateView();
	mView.setCenter(GameToSFML(sCamera::GetCentre()).SF());
	mView.setRotation(sCamera::GetRotation());
	mView.setSize(Vector2(mRender->getSize().x * sCamera::GetZoom(),mRender->getSize().y * sCamera::GetZoom()).SF());

	mRender->setView(mView);

	BaseObject* CurEnt = Renderables.FirstEnt();
	while(CurEnt != NULL)
	{
		if (!CurEnt->GetNoDraw())
		{
			CurEnt->Draw();
			CurEnt = Renderables.NextEnt();
		}
	}
	if (InputHandler::IsKeyPressed(sf::Keyboard::F3))
		State->DrawDebugData();
	mRender->setView(mRender->getDefaultView());
}
