#include "Renderer.h"
#include "IGameState.h"
#include "InputHandler.h"
#include "Camera.h"
#include <Box2D/Collision/b2Collision.h>

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

bool Renderer::IsVisible(BaseRender* rend)
{
	/*
	sf::IntRect rend_bbi = rend->GetBB();
	sf::FloatRect rend_bb(rend_bbi.left, rend_bbi.top, rend_bbi.width, rend_bbi.height);
	sf::FloatRect rend_rect(
		mRender->ConvertCoords(0,0),
		sf::Vector2f(mRender->ConvertCoords(mRender->GetView().GetViewport().Width,
		mRender->GetView().GetViewport().Height)));
	//std::cout << rend_rect.Top << "\n";
	if (rend_rect.Intersects(rend_bb))
	{
		return true;
	}
	else
	{
		return false;
	}*/
	return true;
}

void Renderer::OnResize()
{
	gGlobals.GameWidth = mRender->getSize().x;
	gGlobals.GameHeight = mRender->getSize().y;
	Vector2 Size = Vector2(gGlobals.GameWidth, gGlobals.GameHeight);
};

void Renderer::OnEntityAdded(BaseObject* ent)
{
	std::cout << "Entity Added to Renderer: " << ent->GetClassName() << "\n";
	//BaseRender* render = ent->GetRenderer();
	//AddRenderer(render);
}

void Renderer::AddRenderer(BaseRender* render)
{
	/*
	BaseRender* rend = Renderers.FirstEnt();
	if (Renderers.GetSize() > 0)
	{
		bool FoundPos = false;
		do
		{
			if (render->GetDrawOrder() > rend->GetDrawOrder())
			{
				rend = Renderers.NextEnt();
			}
			else if (render->GetDrawOrder() <= rend->GetDrawOrder())
			{
				FoundPos = true;
			}
		} 
		while (rend != NULL && !FoundPos);
	}
	Renderers.InsertAtCurrent(render);*/
}

void Renderer::OnEntityRemoved(BaseObject* ent)
{
	
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
		CurEnt->Draw();
		CurEnt = Renderables.NextEnt();
	}
	//std::cout << "Culled this frame: " << cull << "\n";
	if (InputHandler::IsKeyPressed(sf::Keyboard::F3))
		State->DrawDebugData();
	mRender->setView(mRender->getDefaultView());
}
