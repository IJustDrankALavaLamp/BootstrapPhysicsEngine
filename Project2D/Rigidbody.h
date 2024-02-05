#pragma once
#include "PhysicsObject.h"
#include <iostream>
class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeId, glm::vec2 position, glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force, glm::vec2 pos);
	void applyForceToOther(Rigidbody* other, glm::vec2 force, glm::vec2 pos);

	void resolveCollision(Rigidbody* other, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	glm::vec2 getPosition() { return m_position; }
	float getOrientation() { return m_orientation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void Stop() { m_velocity = { 0,0 }; }

	float getKineticEnergy();
protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D so only need rotation on one angle
	float m_angularVelocity;
	float m_moment;
};

