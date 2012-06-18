#pragma once

#include "Vector2.h"
#include "MathUtils.h"
#include "SFML/Graphics.hpp"
#include "VariantMap.h"
#include "BaseObject.h"
#include "BaseRender.h"

enum RenderType
{
	RENDER_SHAPE,
	RENDER_SPRITE,
	RENDER_CUSTOM
};

class BaseRenderable : public BaseObject
{
private:
	BaseRenderable(const BaseRenderable&);
	BaseRenderable& operator=(const BaseRenderable&);
	
	RenderType mRenderType;
public:
	BaseRenderable(void);
	virtual ~BaseRenderable(void);

	//Rendering
	void SetRenderType(RenderType T) {mRenderType = T;};
	RenderType GetRenderType() {return mRenderType;};
	void InitRenderer();
	void SetRenderer(BaseRender* rend) {mRender = rend;};

	Vector2 GetScreenPos() {return GetRenderer()->ToScreen(GetPos());};

	void SetDrawPriority(int prior) {if (IsRenderable()) mRender->SetDrawOrder(prior);};
	int GetDrawPriotity() {if (IsRenderable()) return mRender->GetDrawOrder(); return -1;};

	//Interface to renderer
	void SetColour(sf::Color col) {if (IsRenderable()) mRender->SetColour(col);};
	sf::Color GetColour() {if (IsRenderable()) mRender->GetColour(); return sf::Color(0,0,0);};

	void Tick();
};
