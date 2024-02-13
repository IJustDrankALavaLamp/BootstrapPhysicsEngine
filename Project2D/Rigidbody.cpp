#include "Rigidbody.h"


#pragma region
Rigidbody::Rigidbody(ShapeType shapeId, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) 
	: PhysicsObject(shapeId)
{
	m_interactable = true;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation; 
	m_angularVelocity = 0;
	m_mass = mass;
	m_moment = 0;
	m_elasticity = 1;
	m_isKinematic = false;
}

Rigidbody::~Rigidbody()
{
}
#pragma endregion

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (isKinematic()) {
		m_velocity = vec2(0);
		m_angularVelocity = 0;
		return;
	}
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
	if (length(m_velocity) < minLinearThreshold) {
		m_velocity = vec2(0, 0);
	}
	if (abs(m_angularVelocity) < minAngularThreshold) {
		m_angularVelocity = 0;
	}

	m_orientation += m_angularVelocity * timeStep;
	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep, glm::vec2(0, 0));
}

#pragma region Forces
void Rigidbody::resolveCollision(Rigidbody* other, glm::vec2 contact, glm::vec2* collisionNormal, float pen) {
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : other->getPosition() - m_position); // the normal of the plane the collision occurs on
	
	glm::vec2 perp(normal.y, -normal.x); //the vector perpendicular to the collision normal


	// get the total velocity of the contact points for both objects
	// Linear (l), Rotational(r)
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - other->m_position, perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(other->m_velocity, normal) + r2 * other->m_angularVelocity;
	if (v1 > v2) {
		// effective mass at contact point
		float mass1 = 1.0f / (1.0f / m_mass + (r1*r2) / m_moment);
		float mass2 = 1.0f / (1.0f / other->m_mass + (r1 * r2) / other->m_moment);

		float elasticity = (getElasticity() + other->getElasticity() / 2.0f);

		glm::vec2 force = elasticity *mass1*mass2 /(mass1 + mass2) *(v1-v2) * normal;
		applyForce(-force, contact - m_position);
		other->applyForce(force, contact - other->m_position);
	}
	if (pen > 0)
		PhysicsScene::ApplyContactForces(this, other, normal, pen);

}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / getMoment();
}

void Rigidbody::applyForceToOther(Rigidbody* other, glm::vec2 force, glm::vec2 pos)
{
	other->applyForce(force, pos);
}
#pragma endregion
#pragma region KineticEnergy
float Rigidbody::getKineticEnergy() {
	return 0.05f * (m_mass * dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}
#pragma endregion