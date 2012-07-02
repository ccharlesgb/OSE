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

	sf::RenderTexture mFinalTexture; //Our Final texture for ALL lights
	sf::Sprite mFinalSprite; //The sprite that draws the final texture
	EntityList<BaseObject*> ShadowCasters; //Entities that should cast shadows
	
	sf::Shader mLightShader;

	EntityList<BaseObject*> mLights; //effect_lights
	std::vector<sf::RenderTexture*> mLightTextures; //Textures for each light in the screen
	float light_tex_size;
public:
	sf::Shader mBlurShader;
	sf::RenderWindow *mRender;
	Lighting(void);
	~Lighting(void);

	//IEntityListener Implementation
	void OnEntityAdded(BaseObject* ent);
	void OnEntityRemoved(BaseObject* ent);

	//sf::Texture* GetLightingTexture() {return &mLightingFinal;};
	sf::Sprite* GetLightingSprite() {return &mFinalSprite;};

	void UpdateLightingTexture(sf::View &view);

	void DrawLight(LightInfo *light, sf::RenderTexture* tex);
	void DrawShadows(LightInfo *light, sf::RenderTexture* tex);
};

