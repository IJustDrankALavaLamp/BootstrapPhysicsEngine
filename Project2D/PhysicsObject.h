#pragma once
#include "glm/glm.hpp"
class PhysicsObject
{
protected:
	PhysicsObject() {};
public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void resetPosition() {};
};

