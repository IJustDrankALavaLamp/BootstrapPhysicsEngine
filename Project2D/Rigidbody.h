#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeId, glm::vec2 position, glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force);
	void applyForceToOther(Rigidbody* other, glm::vec2 force);

	void resolveCollision(Rigidbody* other);

	glm::vec2 getPosition() { return m_position; }
	float getOrientation() { return m_orientation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void Stop() { m_velocity = { 0,0 }; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D so only need rotation on one angle

};

