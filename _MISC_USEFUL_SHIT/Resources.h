#pragma once

#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>

class Resources
{
private:
	std::map<std::string, sf::Texture*> ImageCache;
	sf::Texture ErrorTex;
public:
	static Resources* Instance();
	Resources(void);
	~Resources(void);
	sf::Texture* RequestImage(std::string Filename);
	sf::Texture* RequestImage(std::string Filename, bool Smooth);
};
