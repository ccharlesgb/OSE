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
	std::cout << "CLEANING UP TEXTURES \n";
	std::map<std::string, sf::SoundBuffer*>::iterator it;
	
	for(it = mSounds.begin(); it != mSounds.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete mSounds[it->first];
	}
}


/*
std::map<std::string, ModelData*> Resource::ModelCache;
std::map<std::string, sf::Texture*> Resource::TextureCache;
sf::Texture* Resource::ErrorTex = NULL;

/**
 * Cleanup the resources.
 *
 * Loop all the resources and delete them.
 */
/*
void Resource::Cleanup()
{
	std::cout << "CLEANING UP MODELS \n";
	std::map<std::string, ModelData*>::iterator ModelIterator;
	for(ModelIterator = ModelCache.begin(); ModelIterator != ModelCache.end(); ModelIterator++) {
		std::cout << "DELETING " << ModelIterator->first << "\n";
		delete ModelCache[ModelIterator->first];
	}
	
	std::cout << "CLEANING UP TEXTURES \n";
	std::map<std::string, sf::Texture*>::iterator it;
	for(it = TextureCache.begin(); it != TextureCache.end(); it++) {
		std::cout << "DELETING " << it->first << "\n";
		delete TextureCache[it->first];
	}
	
	std::cout << "CLEANING UP SOUNDS \n";
	std::map<std::string, sf::SoundBuffer*>::iterator SoundIterator;
	for(SoundIterator = SoundCache.begin(); SoundIterator != SoundCache.end(); SoundIterator++) {
		std::cout << "DELETING " << SoundIterator->first << "\n";
		delete SoundCache[SoundIterator->first];
	}
}

// Models

std::string Resource::GetModelPath(const char* path)
{
	return ("images/" + std::string(path) + ".png").c_str();
}

/**
 * Precache model.
 *
 * Load the model and parse it. Also request the textures to be precached.
 */
/*
void Resource::PrecacheModel(const char *path)
{
	std::string FILE_PATH = GetModelPath(path);
	//std::cout << "LOADING: " << FILE_PATH << "\n";
	ModelData* Model = new ModelData();
	
	//if (!Buffer->loadFromFile(FILE_PATH)) {
		// TODO: Some error managment
	//}
	
	ModelCache[path] = Model;
}

/**
 * Get the model file.
 *
 * If the model is not already precached, precache it.
 */
/*
ModelData* Resource::RequestModel(const char* path)
{
	if (ModelCache[path] == NULL)
	{
		PrecacheModel(path);
	}
	
	return ModelCache[path];
}




// Texture
std::string Resource::GetImagePath(const char* path)
{
	return ("images/" + std::string(path) + ".png").c_str();
}

void Resource::PrecacheTexture(const char* path)
{
	std::string FILE_PATH = GetImagePath(path);
	//std::cout << "LOADING: " << FILE_PATH << "\n";
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
/*
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
 * Get the sound file.
 *
 * If the sound is not already precached, precache it.
 */
/*
sf::Sound* Resource::RequestSound(const char* path)
{
	if (SoundCache[path] == NULL)
	{
		PrecacheSound(path);
	}
	
	return new sf::Sound(*SoundCache[path]);
}


// Fonts


void Resource::PrecacheFont(const char* path)
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
 * Get the sound file.
 *
 * If the sound is not already precached, precache it.
 */
/*
sf::Font* Resource::RequestFont(const char* path)
{
	if (SoundCache[path] == NULL)
	{
		PrecacheSound(path);
	}
	
	//return new sf::Sound(*SoundCache[path]);
}
*/