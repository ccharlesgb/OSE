#pragma once

#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>

class Resource
{
private:
	static std::map<std::string, sf::Texture*> ImageCache;
	static sf::Texture ErrorTex;
public:
	static std::string GetImagePath(const char* path);
	static void Precache(const char* path);
	static sf::Texture* RequestImage(const char* path);
};
