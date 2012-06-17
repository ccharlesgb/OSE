#include "Renderer.h"
#include "IGameState.h"
#include "InputHandler.h"
#include "Camera.h"
#include <Box2D/Collision/b2Collision.h>
#include "HUDRender.h"

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
	Renderers.Clear();
}

void Renderer::SetWindow(sf::RenderWindow *wind) 
{
	mRender = wind;
	mView.SetSize(mRender->GetWidth(), mRender->GetHeight());
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
	sf::Vector2<double> ScreenCentre = sf::Vector2<double>(mRender->GetWidth() / 2.f, mRender->GetHeight() / 2.f);
	dPos = dPos + ScreenCentre;

	Pos.x = dPos.x;
	Pos.y = dPos.y;

	return Pos;
}

Vector2 Renderer::ToScreen(Vector2 pos)
{
	Vector2 ScreenCentre = sf::Vector2f(mRender->GetWidth(), mRender->GetHeight());
	pos.y *= -1.f;
	pos = pos + ScreenCentre - mView.GetCenter();
	return pos;
}

bool Renderer::IsVisible(BaseRender* rend)
{
	sf::IntRect rend_bbi = rend->GetBB();
	sf::FloatRect rend_bb(rend_bbi.Left, rend_bbi.Top, rend_bbi.Width, rend_bbi.Height);
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
	}
}

void Renderer::OnResize()
{
	Vector2 Size = Vector2(mRender->GetWidth(), mRender->GetHeight());
};

void Renderer::OnEntityAdded(BaseObject* ent)
{
	BaseRender* render = ent->GetRenderer();
	AddRenderer(render);
}

void Renderer::AddRenderer(BaseRender* render)
{
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
	Renderers.InsertAtCurrent(render);
}

void Renderer::OnEntityRemoved(BaseObject* ent)
{
	BaseRender* render = ent->GetRenderer();
	Renderers.Delete(render);
}

void Renderer::Draw(IGameState *State)
{
	Clear();
	sCamera::UpdateView();
	mView.SetCenter(GameToSFML(sCamera::GetCentre()).SF());
	mView.SetRotation(sCamera::GetRotation());
	mView.SetSize(Vector2(mRender->GetWidth() * sCamera::GetZoom(),mRender->GetHeight() * sCamera::GetZoom()).SF());

	mRender->SetView(mView);

	BaseRender* CurEnt = Renderers.FirstEnt();
	while(CurEnt != NULL)
	{
		CurEnt->PreDraw(mRender);
		CurEnt->Draw(mRender);
		CurEnt = Renderers.NextEnt();
	}
	//std::cout << "Culled this frame: " << cull << "\n";
	if (InputHandler::IsKeyPressed(sf::Keyboard::F3))
		State->DrawDebugData();
	mRender->SetView(mRender->GetDefaultView());
	State->GetHUD()->Draw(mRender);
}
