#pragma once

#include <string>
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

class Resource
{
private:
	// Texture
	static std::map<std::string, sf::Texture*> TextureCache;
	static sf::Texture* ErrorTex;
	
	static std::string GetImagePath(const char* path);
	
	// Sound
public:
	Resource();
	~Resource();
	
	// Textures
	static void PrecacheTexture(const char* path);
	static sf::Texture* RequestImage(const char* path);
	
	// Sound
	static void PrecacheSound(const char* path);
	static sf::Sound* RequestSoundBuffer(const char* path);
};
