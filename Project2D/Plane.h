#pragma once
#include "PhysicsObject.h"
class Plane : public PhysicsObject
{
public:
	Plane(glm::vec2 normal, float distance);
	Plane(glm::vec2 normal, float distance, glm::vec4 colour);
	~Plane();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw();
	virtual void resetPosition();

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }
protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm:: vec4 m_colour;
};