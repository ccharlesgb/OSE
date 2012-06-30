#include "Resource.h"
#include "Utilities/pugixml.hpp"
#include "Model.h"
#include "Animation.h"

#define ERROR_TEXTURE "images/car.png"

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
			Texture->loadFromFile(ERROR_TEXTURE);
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
	std::string FILE_PATH = ("models/" + std::string(path) + ".xml").c_str();
	std::cout << "LOADING: " << FILE_PATH << "\n";
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(FILE_PATH.c_str());
	
	if (result)
		std::cout << "XML [" << FILE_PATH << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
	else
	{
		std::cout << "XML [" << FILE_PATH << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << ("test.xml" + result.offset) << "]\n\n";
		
		return;
	}
	
	//Pass Texture Data
	Model* model = new Model();

	pugi::xml_node ModelRoot = doc.root().first_child();
	model->mTexturePath = ModelRoot.child("texture").text().as_string();
	model->mScale = ModelRoot.child("scale").text().as_float();
	
	//Parse animation data
	pugi::xml_node AnimationsNode = ModelRoot.child("animations");
	if (AnimationsNode)
	{
		pugi::xml_node CurAnim = AnimationsNode.first_child();
		int id = 0;
		//Loop through all the animations in the model file and load their data
		while (CurAnim != NULL)
		{
			model->mAnimations[id].mName = CurAnim.child("name").text().as_string();
			model->mAnimations[id].mFrameRate = CurAnim.child("framerate").text().as_float();
			model->mAnimations[id].mRow = CurAnim.child("row").text().as_int();

			//Sequence is formatted as a string sperated by spaces so split them up
			//and convert to integers into the array
			std::string SequenceString = CurAnim.child("sequence").text().as_string();
			
			int CurSearchPos = 0; //How far into the string are we?
			int SpacePos = 0; //Where is the space located?
			int CurFrameID = 0; //Current frame we are parsing
			std::cout << "Parsing: " << SequenceString << "\n";
			do
			{
				SpacePos = SequenceString.find(" ", CurSearchPos);
				std::string CurFrame = SequenceString.substr(CurSearchPos, SpacePos - CurSearchPos);
				CurSearchPos = SpacePos + 1;
				model->mAnimations[id].mSequence[CurFrameID] = atoi(CurFrame.c_str());
				std::cout << "Sequence adding frame: " << CurFrame.c_str() << "\n";
				CurFrameID++;
			} 
			while (SpacePos != std::string::npos);
			model->mAnimationCount = CurFrameID;
			std::cout << "LOADED " << CurFrameID << " animations.\n";

			id++;
			CurAnim = CurAnim.next_sibling();
		}
	}

	//Parse Physics Data
	pugi::xml_node PhysicsNode = ModelRoot.child("physics_hull");
	if (PhysicsNode)
	{
		model->mDensity = PhysicsNode.child("density").text().as_float();
		
		pugi::xml_node VertexList = PhysicsNode.child("vertices");
		pugi::xml_node CurVertex = VertexList.first_child();
		
		int id = 0;
		while (CurVertex != NULL)
		{
			std::string line = CurVertex.text().as_string();
			std::string X_COORD, Y_COORD;
			X_COORD = line.substr(0, line.find(" "));
			Y_COORD = line.substr(line.find(" "), line.length());
			int x,y;
			x = atoi(X_COORD.c_str());
			y = atoi(Y_COORD.c_str());
			
			model->mVertices[id] = Vector2(x,y);

			CurVertex = CurVertex.next_sibling();
			id++;
		}
		model->mVertexCount = id;
	}

	model->mLoaded = true;
	
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