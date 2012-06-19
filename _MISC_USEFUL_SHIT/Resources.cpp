#include "Resources.h"

Resources* Inst;

Resources* Resources::Instance()
{
	if (Inst == NULL)
		Inst = new Resources;
	return Inst;
}

Resources::Resources(void)
{
	ErrorTex.LoadFromFile("images/error.png");
}

Resources::~Resources(void)
{
}

sf::Texture* Resources::RequestImage(std::string Filename, bool Smooth)
{
	Filename = "images/" + Filename + ".png";
	if (ImageCache[Filename] == NULL)
	{
		bool Success;
		sf::Texture* tex = new sf::Texture();
		Success = tex->LoadFromFile(Filename);
		tex->SetSmooth(Smooth);
		if (Success)
		{
			ImageCache[Filename] = tex;
		}
		else
		{
			ImageCache[Filename] = &ErrorTex;
			std::cout << "Error: Couldn't find image: " << Filename << "\n";
		}
	}
	return ImageCache[Filename];
}

sf::Texture* Resources::RequestImage(std::string Filename)
{
	return RequestImage(Filename, false);
}
