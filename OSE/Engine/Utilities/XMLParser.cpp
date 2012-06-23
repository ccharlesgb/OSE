#include "XMLParser.h"

void XMLParser::LoadFile(const char* path)
{
	ModelData CurrentModel;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("test.xml");
	if (result)
		std::cout << "XML [" << "test.xml" << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
	else
	{
		std::cout << "XML [" << "test.xml" << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << ("test.xml" + result.offset) << "]\n\n";
	}

	//Pass Texture Data
	pugi::xml_node ModelRoot = doc.root().first_child();
	CurrentModel.mTexturePath = ModelRoot.child("texture").text().as_string();
	CurrentModel.mScale = ModelRoot.child("scale").text().as_float();

	//Parse Physics Data
	pugi::xml_node PhysicsNode = ModelRoot.child("physics_hull");
	if (PhysicsNode)
	{
		CurrentModel.mVertexCount = PhysicsNode.child("density").text().as_int();
		CurrentModel.mDensity = PhysicsNode.child("density").text().as_float();
		pugi::xml_node VertexList = PhysicsNode.child("vertices");
		pugi::xml_node CurVertex = VertexList.first_child();
		int id = 0;
		while (CurVertex != VertexList.last_child())
		{
			std::string line = CurVertex.text().as_string();
			std::string X_COORD, Y_COORD;
			X_COORD = line.substr(0, line.find(" "));
			Y_COORD = line.substr(line.find(" "), line.length());
			int x,y;
			x = atoi(X_COORD.c_str());
			y = atoi(Y_COORD.c_str());
			CurrentModel.mVertices[id] = Vector2(x,y);
			std::cout << "Adding Vertex: " << Vector2(x,y).ToString() << "\n";
			CurVertex = CurVertex.next_sibling();
		}
	}
	
	std::cout << "Finished Loading Model:"						<< "\n";
	std::cout << "Texture Path: " << CurrentModel.mTexturePath	<< "\n";
	std::cout << "Scale		  : " << CurrentModel.mScale		<< "\n";
	std::cout << "Density     : "	  << CurrentModel.mDensity	<< "\n";
	std::cout << "Vertex Count: " << CurrentModel.mVertexCount	<< "\n";
}
/*
<model>
  <texture>images/crate</texture>
  <physics_hull>
    <density>4</density>
    <vertex_count>4</vertex_count>
    <vertices>
      <vertex>50 50</vertex>
      <vertex>-50 50</vertex>
      <vertex>-50 -50</vertex>
      <vertex>50 -50</vertex>
    </vertices>
  </physics_hull>
  
</model>
*/