#include "BasePhysics.h"
#include <fstream>
#include <iostream>

BasePhysics::BasePhysics(void)
{
	mPhysObj = NULL;
}

void BasePhysics::PhysicsInit(BODY_TYPE typ)
{
	mIsPhysics = true;
	mPhysObj = new PhysicsDef;
	mPhysObj->SetOwner(this);
	mPhysObj->SetPos(GetPos());
	mPhysObj->SetAngle(0);
	mPhysObj->SetType(typ);
}

void BasePhysics::PhysicsHullFromModel()
{
	if (mModelInfo.Loaded)
	{
		PolygonShape shape;
		shape.mDensity = mModelInfo.mDensity;
		
		for (int i = 0; i < mModelInfo.mVertexCount; i++) {
			std::cout << "INSERT \n";
			shape.AddVertex(mModelInfo.mVertices[i]);
		}
		
		GetPhysObj()->AddPhysicsShape(&shape);
		return;
	}
	
	//TODO: Add error handling
	std::string mPath(GetModel());
	std::string file_name = "images/" + mPath + ".txt";
	std::ifstream myfile((char*)file_name.c_str());
	if (myfile.is_open())
	{
		PolygonShape shape;
		shape.mDensity = 1.f;

		std::string line;
		while (myfile.good())
		{
			std::getline(myfile, line);
			//std::cout << line << "\n";
			if (line.find("//") != 0) // Not a comment
			{
				std::string X_COORD, Y_COORD;
				X_COORD = line.substr(0, line.find(" "));
				Y_COORD = line.substr(line.find(" "), line.length());
				int x,y;
				x = atoi(X_COORD.c_str());
				y = atoi(Y_COORD.c_str());
				shape.AddVertex(Vector2(x * mScale.x,y * mScale.y));
				//std::cout << "ADDING VERTEX: " << x << " " << y << "\n";
			}
		}
		myfile.close();
		GetPhysObj()->AddPhysicsShape(&shape);
	}
	else
	{
		std::cout <<
			"Physics Information missing for model: " << file_name << "\n";
	}
}

void BasePhysics::Tick()
{
	if (mPhysObj != NULL)
	{
		SetPos(mPhysObj->GetPos());
		SetAngle(mPhysObj->GetAngle());
	}
	BaseObject::Tick();
}

BasePhysics::~BasePhysics(void)
{
	if (mIsPhysics)
		delete GetPhysObj();
}
