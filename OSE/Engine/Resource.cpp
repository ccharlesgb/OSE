#include "Resource.h"
#include "GameGlobals.h"

std::map<std::string, sf::Texture*> Resource::TextureCache;
sf::Texture* Resource::ErrorTex = NULL;

/**
 * Cleanup the resources.
 *
 * Loop all the resources and delete them.
 */
void Resource::Cleanup()
{
	std::cout << "CLEANING UP RESOURCES \n";
	
	std::cout << "CLEANING UP TEXTURES \n";
	std::map<std::string, sf::Texture*>::iterator it;
	for(it = TextureCache.begin(); it != TextureCache.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete TextureCache[it->first];
	}
	
	std::cout << "CLEANING UP SOUNDS \n";
	std::map<std::string, sf::SoundBuffer*>::iterator SoundIterator;
	for(SoundIterator = SoundCache.begin(); SoundIterator != SoundCache.end(); it++) {
		std::cout << "DELETING " << SoundIterator->first << "\n";
		delete SoundCache[SoundIterator->first];
	}
}

// Texture
std::string Resource::GetImagePath(const char* path)
{
	return ("images/" + std::string(path) + ".png").c_str();
}

void Resource::PrecacheTexture(const char* path)
{
	std::string FILE_PATH = GetImagePath(path);
	std::cout << "LOADING: " << FILE_PATH << "\n";
	sf::Texture* Texture = new sf::Texture();
	
	// Load the error texture if file was not found.
	if (!Texture->loadFromFile(FILE_PATH))
	{
		std::cout << "Error\n";
		if (ErrorTex == NULL) {
			Texture->loadFromFile(GetImagePath("ship"));
			ErrorTex = Texture;
		} else {
			Texture = ErrorTex;
		}
	}
	
	TextureCache[path] = Texture;
}

sf::Texture* Resource::RequestImage(const char* path)
{
	if (TextureCache[path] == NULL)
	{
		PrecacheTexture(path);
	}
	return TextureCache[path];
}

// Sound
std::map<std::string, sf::SoundBuffer*> Resource::SoundCache;

std::string Resource::GetSoundPath(const char* path)
{
	return ("images/" + std::string(path) + ".wav").c_str();
}

/**
 * Precache sound.
 *
 * Load the sound from the file path and insert the SoundBuffer into the SoundCache map.
 */
void Resource::PrecacheSound(const char* path)
{
	std::string FILE_PATH = GetSoundPath(path);
	std::cout << "LOADING: " << FILE_PATH << "\n";
	sf::SoundBuffer* Buffer = new sf::SoundBuffer();
	
	if (!Buffer->loadFromFile(FILE_PATH)) {
		// TODO: Some error managment
	}
	
	SoundCache[path] = Buffer;
}

/**
 * Get a sound file.
 *
 * If the sound is not already precached, precache it and then serve it,
 * otherwise use the precached value.
 */
sf::Sound* Resource::RequestSound(const char* path)
{
	if (SoundCache[path] == NULL)
	{
		PrecacheSound(path);
	}
	
	return new sf::Sound(*SoundCache[path]);
}
