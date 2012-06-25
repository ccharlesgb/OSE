#include "Model.h"
#include "Utilities/pugixml.hpp"

Model::Model()
{
	Loaded = false;
	
	mVertexCount = 0;
	mDensity = 0.f;
	mTexturePath = NULL;
	mScale = 1.f;
}

bool Model::LoadFromFile(const char *path)
{
	path = ("images/" + std::string(path) + ".xml").c_str();
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path);
	
	if (result)
		std::cout << "XML [" << path << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
	else
	{
		std::cout << "XML [" << path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << ("test.xml" + result.offset) << "]\n\n";
		
		return false;
	}
	
	//Pass Texture Data
	pugi::xml_node ModelRoot = doc.root().first_child();
	mTexturePath = ModelRoot.child("texture").text().as_string();
	mScale = ModelRoot.child("scale").text().as_float();
	
	//Parse Physics Data
	pugi::xml_node PhysicsNode = ModelRoot.child("physics_hull");
	if (PhysicsNode)
	{
		mVertexCount = PhysicsNode.child("density").text().as_int();
		mDensity = PhysicsNode.child("density").text().as_float();
		
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
			
			mVertices[id] = Vector2(x,y);
			
			std::cout << "Adding Vertex: " << Vector2(x,y).ToString() << "\n";
			CurVertex = CurVertex.next_sibling();
			id++;
		}
	}
	
	std::cout << "Finished Loading Model:"        << "\n";
	std::cout << "Texture Path: " << mTexturePath << "\n";
	std::cout << "Scale       : " << mScale       << "\n";
	std::cout << "Density     : " << mDensity     << "\n";
	std::cout << "Vertex Count: " << mVertexCount << "\n";

	Loaded = true;
	
	return true;
}