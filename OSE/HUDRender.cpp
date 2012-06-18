#include "HUDRender.h"
#include "GameGlobals.h"
#include "Renderer.h"
#include "PhysicsDef.h"
#include "InputHandler.h"

HUDRender::HUDRender(void)
{
}


HUDRender::~HUDRender(void)
{
}

void HUDRender::AddShapeElement(const char* ID)
{
	//mElements[ID] = new sf::Shape();
}

sf::Shape* HUDRender::GetShapeElement(const char*ID)
{
	return dynamic_cast<sf::Shape*>(mElements[ID]);
}

void HUDRender::AddTextElement(const char* ID)
{
	mElements[ID] = new sf::Text();
}

sf::Text* HUDRender::GetTextElement(const char*ID)
{
	return dynamic_cast<sf::Text*>(mElements[ID]);
}

void HUDRender::DrawText(const char* txt, Vector2 pos, int size, sf::Color col)
{
	mText.setPosition(pos.SF());
	mText.setCharacterSize(size);
	mText.setColor(col);
	mText.setString(txt);
	mRend->draw(mText);
}

//void HUDRender::DrawCircle(Vector2 pos, float radius, sf::Color col, float outline, sf::Color out_col)
//{
	//mCircle.SetPosition(pos);
	//mCircle.
	//mRend->Draw(mCircle);
//}

void HUDRender::Draw(sf::RenderWindow *rend)
{
	mRend = rend; //HACK

	//Draw FrameTime
	char Buffer[64];
	#ifdef __APPLE__
		sprintf(Buffer, "Frame Time: %f (%iFPS)", gGlobals.FrameTime, (int)std::floor(1.f/gGlobals.FrameTime));
	#else
		sprintf_s(Buffer, "Frame Time: %f (%iFPS)", gGlobals.FrameTime, (int)std::floor(1.f/gGlobals.FrameTime));
	#endif
	mText.setString(Buffer);
	DrawText(Buffer, Vector2(20,20), 20, sf::Color::Red);

	ObjList *Players = gGlobals.gEntList.FindByClass("player");
	if (Players->GetSize() > 0)
	{
		BaseObject* ply = Players->FirstEnt();

		DrawText("Player Position:", Vector2(20, 45), 20, sf::Color::Red);
		//sprintf_s(Buffer, "X : %f    Y : %f", InputHandler::GetMousePosWorld().x, InputHandler::GetMousePosWorld().y);
		#ifdef __APPLE__
			sprintf(Buffer, "X : %f    Y : %f", ply->GetPos().x, ply->GetPos().y);
		#else
			sprintf_s(Buffer, "X : %f    Y : %f", ply->GetPos().x, ply->GetPos().y);
		#endif
		DrawText(Buffer, Vector2(20,70), 20, sf::Color::Red);

		int limit = ply->GetPhysObj()->GetLinearVelocity().Length() / 5.f;
		for (int i = 0; i < limit; i+=1)
		{
			Vector2 pos = ply->GetRenderer()->ToScreen((ply->GetPhysObj()->GetLinearVelocity().Normalize() * (i + 10)) + ply->GetPos());
			//DrawCircle(pos, 2, sf::Color(200,50,50,200), 0.f, sf::Color::White);
		}
	}
}

//void HUDRender::DrawText(std::string &txt, float x, float y, sf::Color col)
//{
	
//}
