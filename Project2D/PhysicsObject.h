#pragma once
#include "glm/glm.hpp"
#include "Gizmos.h"

enum ShapeType { PLANE = 0, SPHERE, BOX };
class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {};

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void resetPosition() {};
	
protected:
	ShapeType m_shapeID;

};

