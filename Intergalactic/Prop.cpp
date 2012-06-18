#include "Prop.h"
#include <iostream>
#include <fstream>

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
	std::ifstream myfile("images/" + mPath + ".txt");
	if (myfile.is_open())
	{
		PolygonShape* shape = new PolygonShape;
		shape->mDensity = 1.f;

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
				std::cout << "X:" << x << " Y:" << y << "\n";
				shape->AddVertex(Vector2(x,y));
			}
		}
		myfile.close();
		GetPhysObj()->AddPhysicsShape(shape);

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
