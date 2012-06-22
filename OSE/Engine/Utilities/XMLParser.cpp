#include "XMLParser.h"

FILE*			XMLParser::mFile;
mxml_node_t*	XMLParser::mTree;
ModelData		XMLParser::CurrentModel;

void XMLParser::LoadFile(const char* path)
{
	mFile = std::fopen(path, "r");
	mTree = mxmlLoadFile(NULL, mFile, LoadCallBack);
	fclose(mFile);
	
	mxml_node_t* next_node = mTree;
	while  (next_node != NULL)
	{
		std::cout << next_node->value.text.string << "\n";
		//std::string type(mxmlGetText(next_node,0));
		//std::cout << type << "\n";
		next_node = mxmlWalkNext(next_node, mTree, MXML_DESCEND);
	}

	std::cout << "Finished Loading Model:"						<< "\n";
	//std::cout << "Texture Path: " << CurrentModel.mTexturePath	<< "\n";
	//std::cout << "Density: "	  << CurrentModel.mDensity		<< "\n";
	//std::cout << "Vertex Count: " << CurrentModel.mVertexCount	<< "\n";
}

mxml_type_t XMLParser::LoadCallBack(mxml_node_t *node)
{	
	std::string type(mxmlGetElement(node));
	//Troll Code
	//std::cout << ":" << type << ":\n";
	if (type == "texture")
	{
		return MXML_TEXT;
	}
	else if (type == "density")
	{
		return MXML_REAL;
	}
	else if (type == "vertex")
	{
		return MXML_TEXT;
	}
	return MXML_TEXT;
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