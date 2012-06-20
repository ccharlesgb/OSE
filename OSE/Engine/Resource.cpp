#include "Resource.h"
#include "GameGlobals.h"

std::map<std::string, sf::Texture*> Resource::ImageCache;

Resource::Resource()
{

}

Resource::~Resource()
{
	std::map<std::string, sf::Texture*>::iterator it;
	for(it = ImageCache.begin(); it != ImageCache.end(); it++) {
		std::cout << "DELETEING " << it->first << "\n";
		delete ImageCache[it->first];
	}
	
	//CLEAN UP YOUR TEXTURES HERE
}

std::string Resource::GetImagePath(const char* path)
{
	return ("images/" + std::string(path) + ".png").c_str();
}

void Resource::Precache(const char* path)
{
	std::string FILE_PATH = GetImagePath(path);
	std::cout << "LOADING: " << FILE_PATH << "\n";
	sf::Texture* Texture = new sf::Texture();
	
	// Load the error texture if file was not found.
	if (!Texture->loadFromFile(FILE_PATH))
	{
		std::cout << "Error\n";
		Texture->loadFromFile(GetImagePath("ship"));
		ImageCache[path] = Texture;
	}
	else
	{
		ImageCache[path] = Texture;
	}
}

sf::Texture* Resource::RequestImage(const char* path)
{
	if (ImageCache[path] == NULL)
	{
		Precache(path);
	}
	return ImageCache[path];
}