#pragma once

#include <SFML/Graphics.hpp>
#include "../GameGlobals.h"
#include "../EntityList.h"

class BaseObject;

class Lighting : public IEntityListener<BaseObject*>
{
private:
	sf::Texture mBlackTex;
	sf::Texture mLightingFinal;
	sf::RenderTexture mCasterTexture;
	sf::Sprite mLightingSprite;
	EntityList<BaseObject*> ShadowCasters;
	
	sf::Sprite mLightSprite;
	sf::RenderTexture rendertex;
	sf::Shader mLightShader;

public:
	sf::Shader mBlurShader;
	sf::RenderWindow *mRender;
	Lighting(void);
	~Lighting(void);

	//IEntityListener Implementation
	void OnEntityAdded(BaseObject* ent);
	void OnEntityRemoved(BaseObject* ent);

	sf::Texture* GetLightingTexture() {return &mLightingFinal;};
	sf::Sprite* GetLightingSprite() {return &mLightingSprite;};

	void UpdateLightingTexture(sf::View &view);

	void DrawLight(Vector2 pos);
	void DrawShadows(Vector2 LightPos);
};

