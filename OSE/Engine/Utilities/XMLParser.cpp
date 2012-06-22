#include "XMLParser.h"

void XMLParser::LoadFile(const char* path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("test.xml");
	if (result)
		std::cout << "\XML [" << "test.xml" << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
	else
	{
		std::cout << "XML [" << "test.xml" << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << ("test.xml" + result.offset) << "]\n\n";
	}

	

	std::cout << "Finished Loading Model:"						<< "\n";
	//std::cout << "Texture Path: " << CurrentModel.mTexturePath	<< "\n";
	//std::cout << "Density: "	  << CurrentModel.mDensity		<< "\n";
	//std::cout << "Vertex Count: " << CurrentModel.mVertexCount	<< "\n";
}
/*
<model>
  <physics_hull>
    <texture>images/crate</texture>
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