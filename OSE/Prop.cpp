#include "Prop.h"
#include <iostream>
#include <fstream>
#include <iostream>

LINKCLASSTONAME("ent_prop", Prop)
#define GRAVITY_STRENGTH 150

Prop::Prop(void)
{
	RegisterInput("SetSprite", SetSprite);
	SetPos(Vector2::Random(-80.f,80.f));

	SetRenderType(RENDER_SPRITE);
	InitRenderer();
	GetRenderer()->SetDrawOrder(RENDERGROUP_ENTITIES);

	PhysicsInit(DYNAMIC_BODY);
}

void Prop::SetSprite(BaseObject* ent, VariantMap &Data)
{
	Prop* me = dynamic_cast<Prop*>(ent);
	me->mPath = Data.ReadString("path");
}


void Prop::Spawn()
{
	//TODO: Add error handling
	GetRenderer()->SetTexture((mPath + ".png").c_str(), true);
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
				shape.AddVertex(Vector2(x,y));
			}
		}
		myfile.close();
		GetPhysObj()->AddPhysicsShape(&shape);

		GetPhysObj()->SetAngularDamping(1);
		GetPhysObj()->SetLinearDamping(1);
	}
	else
	{
		std::cout <<
			"Physics Information missing for this sprite!\n";
	}
}


Prop::~Prop(void)
{
}

void Prop::Think()
{
}
