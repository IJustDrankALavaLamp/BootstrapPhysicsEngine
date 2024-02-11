#pragma once
#include "glm/glm.hpp"
#include "Gizmos.h"
using namespace glm;
enum ShapeType { PLANE = 0, SPHERE, BOX, MOUSE, SHAPE_COUNT };
class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {};

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void resetPosition() {};
	
	virtual int getShapeID() { return m_shapeID; }
protected:
	ShapeType m_shapeID;

};

