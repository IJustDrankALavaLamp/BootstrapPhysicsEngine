#pragma once
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include <iostream>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeId, glm::vec2 position, glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force, glm::vec2 pos);
	void applyForceToOther(Rigidbody* other, glm::vec2 force, glm::vec2 pos);

	void resolveCollision(Rigidbody* other, glm::vec2 contact, glm::vec2* collisionNormal = nullptr, float pen = 0);

#pragma region Getters/Setters
	// Getters -------------
	vec2 getPosition() { return m_position; }
	float getOrientation() { return m_orientation; }
	vec2 getVelocity() { return m_velocity; }
	float getAngularVel() { return m_angularVelocity; };
	float getMass() { return isKinematic() ? INT_MAX : m_mass; }
	float getMoment() { return isKinematic() ? INT_MAX : m_moment; }
	float getKineticEnergy();
	float getElasticity() { return m_elasticity; }
	bool isKinematic() { return m_isKinematic; }

	// Setters ----------
	void setPosition(vec2 newPos) { m_position = newPos; }
	void setKinematic(bool state) { m_isKinematic = state; }

#pragma endregion
	void Stop() { m_velocity = { 0,0 }; }


protected:
	vec2 m_position;
	vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D so only need rotation on one angle
	float m_angularVelocity;
	float m_moment;
	float m_linearDrag = 0.03f;
	float m_angularDrag = 0.03f;
	float m_elasticity;
	// statics
	const float minLinearThreshold = 0.01f;
	const float minAngularThreshold = 0.05f;

	bool m_isKinematic;
};

