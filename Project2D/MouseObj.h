#pragma once
#include "PhysicsObject.h"
class MouseObj : public PhysicsObject
{
private:
	vec2 pos;
public:
	MouseObj();

	virtual void Draw();
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void setPos(vec2 npos);
	void setPos(int, int);
};

