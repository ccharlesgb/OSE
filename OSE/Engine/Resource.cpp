#include "Resource.h"
#include "GameGlobals.h"

std::map<std::string, sf::Texture*> Resource::TextureCache;
sf::Texture* Resource::ErrorTex = NULL;

Resource::Resource()
{

}

Resource::~Resource()
{
	std::map<std::string, sf::Texture*>::iterator it;
	for(it = TextureCache.begin(); it != TextureCache.end(); it++) {
		std::cout << "DELETEING " << it->first << "\n";
		delete TextureCache[it->first];
	}
	
	//CLEAN UP YOUR TEXTURES HERE
}

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
		}
		TextureCache[path] = Texture;
	}
	else
	{
		TextureCache[path] = Texture;
	}
}

sf::Texture* Resource::RequestImage(const char* path)
{
	if (TextureCache[path] == NULL)
	{
		PrecacheTexture(path);
	}
	return TextureCache[path];
}
