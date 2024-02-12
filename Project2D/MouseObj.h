#pragma once
#include "PhysicsObject.h"
class MouseObj : public PhysicsObject
{
private:
	vec2 pos;
	const float radius = 3;
	
public:
	MouseObj();

	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	bool Cutting = false;
	//Setters
	void setPos(vec2 npos);
	void setPos(int, int);

	//Getters
	int getX() { return pos.x; }
	int getY() { return pos.y; }

};

