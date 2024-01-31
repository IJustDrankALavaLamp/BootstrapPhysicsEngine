#include "Rigidbody.h"
#pragma region
Rigidbody::Rigidbody(ShapeType shapeId, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) 
	: PhysicsObject(shapeId)
{
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{
}
#pragma endregion

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep);
}

#pragma region Forces
void Rigidbody::resolveCollision(Rigidbody* other) {
	glm::vec2 normal = glm::normalize(other->getPosition() - m_position); // the normal of the plane the collision occurs on
	glm::vec2 relativeVelocity = other->getVelocity() - m_velocity; // the difference between velocity

	if (glm::dot(normal, relativeVelocity) >= 0)
		return; //objects are already moving apart

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		((1 / m_mass) + (1 / other->getMass()));

	glm::vec2 force = normal * j;

	float preKe = getKineticEnergy() + other->getKineticEnergy();

	applyForce(-force);
	applyForceToOther(other, force);
	
	float postKe = getKineticEnergy() + other->getKineticEnergy();
	float deltaKe = postKe - preKe;
	if (deltaKe > postKe * 0.01f)
		std::cout << "Kinetic Energy lost or gained, fatal physics flaw" << std::endl;
}

void Rigidbody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToOther(Rigidbody* other, glm::vec2 force)
{
	other->applyForce(force);
}
#pragma endregion
#pragma region KineticEnergy
float Rigidbody::getKineticEnergy() {
	float velocityTotal = sqrtf(powf(m_velocity.x, 2) + powf(m_velocity.y, 2));
	float energy = (m_mass * 0.5) * powf(velocityTotal, 2);
	return energy;
}
#pragma endregion