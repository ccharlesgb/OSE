#pragma once

#include <SFML/Graphics.hpp>
#include "../GameGlobals.h"
#include "../EntityList.h"
#include <vector>

class BaseObject;
class LightInfo;

class Lighting : public IEntityListener<BaseObject*>
{
private:
	sf::Texture mBlackTex; // Black texture for shadows

	sf::RenderTexture mCasterTexture;
	sf::Sprite mLightingSprite;
	EntityList<BaseObject*> ShadowCasters;
	
	sf::Shader mLightShader;

	EntityList<BaseObject*> mLights;
	std::vector<sf::RenderTexture*> mLightTextures;

public:
	sf::Shader mBlurShader;
	sf::RenderWindow *mRender;
	Lighting(void);
	~Lighting(void);

	//IEntityListener Implementation
	void OnEntityAdded(BaseObject* ent);
	void OnEntityRemoved(BaseObject* ent);

	//sf::Texture* GetLightingTexture() {return &mLightingFinal;};
	sf::Sprite* GetLightingSprite() {return &mLightingSprite;};

	void UpdateLightingTexture(sf::View &view);

	void DrawLight(LightInfo *light, sf::RenderTexture* tex);
	void DrawShadows(LightInfo *light, sf::RenderTexture* tex);
};

