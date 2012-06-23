#pragma once

#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "Utilities/XMLParser.h"

class Resource
{
private:
	// Model
	static std::map<std::string, ModelData*> ModelCache;
	static std::string GetModelPath(const char* path);
	
	// Texture
	static std::map<std::string, sf::Texture*> TextureCache;
	static sf::Texture* ErrorTex;
	
	static std::string GetImagePath(const char* path);
	
	// Sound
	static std::map<std::string, sf::SoundBuffer*> SoundCache;
	static std::string GetSoundPath(const char* path);
public:
	static void Cleanup();
	
	// Models
	static void PrecacheModel(const char* path);
	static ModelData* RequestModel(const char* path);
	
	// Textures
	static void PrecacheTexture(const char* path);
	static sf::Texture* RequestImage(const char* path);
	
	// Sound
	static void PrecacheSound(const char* path);
	static sf::Sound* RequestSound(const char* path);
};
