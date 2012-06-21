#include "Resource.h"
#include "GameGlobals.h"

std::map<std::string, sf::Texture*> Resource::TextureCache;
sf::Texture* Resource::ErrorTex = NULL;

void Resource::Cleanup()
{
	std::cout << "CLEANING UP RESOURCES \n";
	
	std::map<std::string, sf::Texture*>::iterator it;
	for(it = TextureCache.begin(); it != TextureCache.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete TextureCache[it->first];
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

sf::Sound* Resource::RequestSound(const char* path)
{
	if (SoundCache[path] == NULL)
	{
		PrecacheSound(path);
	}
	
	return new sf::Sound(*SoundCache[path]);
}
