#include "Resource.h"
#include "GameGlobals.h"

std::map<std::string, sf::Texture*> Resource::ImageCache;

std::string Resource::GetImagePath(const char* path)
{
	return ("images/" + std::string(path) + ".png").c_str();
}

void Resource::Precache(const char* path)
{
	std::string FILE_PATH = GetImagePath(path);
	
	sf::Texture Texture;
	
	// Load the error texture if file was not found.
	if (!Texture.loadFromFile(FILE_PATH))
	{
		Texture.loadFromFile(GetImagePath("Ship"));
	}
	
	ImageCache[path] = &Texture;
}

sf::Texture* Resource::RequestImage(const char* path)
{
	if (ImageCache[path] == NULL)
	{
		Precache(path);
	}
	return ImageCache[path];
}