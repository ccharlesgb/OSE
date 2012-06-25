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
	 * @param char Path
	 */
	static void Precache(const char *path);
	
	/**
	 * Get a cached resource.
	 *
	 * @param char Path
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
	 * @param char Path
	 */
	static void Precache(const char *path);
	
	/**
	 * Get a cached resource.
	 *
	 * @param char Path
	 */
	static sf::Sound* GetSound(const char *path);
	
	/**
	 * Cleanup the sound buffers from memory.
	 */
	static void Cleanup();
};

/**
 * Font resource loader
 *
 * @ingroup engine
 */
class FontResource
{
private:
	
	static std::map<std::string, sf::Font*> mFonts;
	
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
	static sf::Font* GetFont(const char *path);
	
	/**
	 * Cleanup the sound buffers from memory.
	 */
	static void Cleanup();
};
