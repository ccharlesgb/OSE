#pragma once

#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "Utilities/XMLParser.h"

/**
 * Texture resource loader.
 *
 * @ingroup engine
 */
class TextureResource
{
private:
	
	static std::map<std::string, sf::Texture*> mTextures;
	static sf::Texture *ErrorTex;
	
public:
	
	/**
	 * Precache the resource.
	 *
	 * @param char Path to texture.
	 */
	static void Precache(const char *path);
	
	/**
	 * Get a cached resource.
	 *
	 * @param char Path to texture.
	 */
	static sf::Texture* GetTexture(const char *path);
	
	/**
	 * Cleanup the textures from memory.
	 */
	static void Cleanup();
};

/**
 * Sound resource loader.
 *
 * @ingroup engine
 */
class SoundResource
{
private:

	static std::map<std::string, sf::SoundBuffer*> mSounds;
	
public:
	/**
	 * Precache the resource.
	 *
	 * @param char Path to sound.
	 */
	static void Precache(const char *path);
	
	/**
	 * Get a cached resource.
	 *
	 * @param char Path to texture.
	 */
	static sf::Sound* GetSound(const char *path);
	
	/**
	 * Cleanup the sound buffers from memory.
	 */
	static void Cleanup();
};

/*
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
	
	// Fonts
	static void PrecacheFont(const char *path);
	static sf::Font* RequestFont(const char *path);
};
*/