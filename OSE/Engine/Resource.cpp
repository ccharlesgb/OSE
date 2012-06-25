#include "Resource.h"
//#include "GameGlobals.h"

/**
 * TextureResource
 */
std::map<std::string, sf::Texture*> TextureResource::mTextures;
sf::Texture* TextureResource::ErrorTex = NULL;

void TextureResource::Precache(const char *path)
{
	std::string FILE_PATH = ("images/" + std::string(path) + ".png").c_str();
	std::cout << "LOADING: " << FILE_PATH << "\n";
	
	sf::Texture* Texture = new sf::Texture();
	
	// Load the error texture if file was not found.
	if (!Texture->loadFromFile(FILE_PATH))
	{
		std::cout << "Error unable to find texture: " + FILE_PATH + "\n";
		if (ErrorTex == NULL) {
			Texture->loadFromFile("images/ship.png");
			ErrorTex = Texture;
		} else {
			Texture = ErrorTex;
		}
	}
	Texture->setSmooth(true);
	mTextures[path] = Texture;
}

sf::Texture* TextureResource::GetTexture(const char* path)
{
	if (mTextures[path] == NULL)
	{
		Precache(path);
	}
	return mTextures[path];
}

void TextureResource::Cleanup()
{
	std::cout << "CLEANING UP TEXTURES \n";
	std::map<std::string, sf::Texture*>::iterator it;
	
	for(it = mTextures.begin(); it != mTextures.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete mTextures[it->first];
	}
}

/**
 * SoundResource
 */
std::map<std::string, sf::SoundBuffer*> SoundResource::mSounds;

void SoundResource::Precache(const char *path)
{
	std::string FILE_PATH = ("images/" + std::string(path) + ".wav").c_str();
	std::cout << "LOADING: " << FILE_PATH << "\n";
	
	sf::SoundBuffer* Buffer = new sf::SoundBuffer();
	
	if (!Buffer->loadFromFile(FILE_PATH)) {
		// TODO: Some error managment
		std::cout << "ERROR! UNABLE TO FIND THE SONG: " + FILE_PATH + "\n";
	}
	
	mSounds[path] = Buffer;
}

sf::Sound* SoundResource::GetSound(const char* path)
{
	if (mSounds[path] == NULL)
	{
		Precache(path);
	}
	return new sf::Sound(*mSounds[path]);
}

void SoundResource::Cleanup()
{
	std::cout << "CLEANING UP SOUNDS \n";
	std::map<std::string, sf::SoundBuffer*>::iterator it;
	
	for(it = mSounds.begin(); it != mSounds.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete mSounds[it->first];
	}
}

/**
 * FontResource
 */
std::map<std::string, sf::Font*> FontResource::mFonts;

void FontResource::Precache(const char *path)
{
	std::string FILE_PATH = ("images/" + std::string(path) + ".ttf").c_str();
	std::cout << "LOADING: " << FILE_PATH << "\n";
	
	sf::Font* font = new sf::Font();
	
	if (!font->loadFromFile(FILE_PATH)) {
		// TODO: Some error managment
		std::cout << "ERROR! UNABLE TO FIND THE FONT: " + FILE_PATH + "\n";
	}
	
	mFonts[path] = font;
}

sf::Font* FontResource::GetFont(const char* path)
{
	if (mFonts[path] == NULL)
	{
		Precache(path);
	}
	return mFonts[path];
}

void FontResource::Cleanup()
{
	std::cout << "CLEANING UP FONTS \n";
	std::map<std::string, sf::Font*>::iterator it;
	
	for(it = mFonts.begin(); it != mFonts.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete mFonts[it->first];
	}
}

/**
 * ModelResource
 */
std::map<std::string, Model*> ModelResource::mModels;

void ModelResource::Precache(const char *path)
{
	const char* FILE_PATH = ("images/" + std::string(path) + ".xml").c_str();
	std::cout << "LOADING: " << FILE_PATH << "\n";
	
	Model *model = new Model();
	
	if (!model->LoadFromFile(FILE_PATH)) {
		// TODO: Some error managment
		std::cout << "ERROR! UNABLE TO FIND THE FONT: " + std::string(FILE_PATH) + "\n";
	}
	
	mModels[path] = model;
}

Model* ModelResource::GetModel(const char* path)
{
	if (mModels[path] == NULL)
	{
		Precache(path);
	}
	return mModels[path];
}

void ModelResource::Cleanup()
{
	std::cout << "CLEANING UP FONTS \n";
	std::map<std::string, Model*>::iterator it;
	
	for(it = mModels.begin(); it != mModels.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete mModels[it->first];
	}
}