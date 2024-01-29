#include "Rigidbody.h"
#pragma region
Rigidbody::Rigidbody(ShapeType shapeId, glm::vec2 position, glm::vec2 velocity, float orientation, float mass)
{

}

Rigidbody::~Rigidbody()
{
}
#pragma endregion

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep);

}
#pragma region
void Rigidbody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToOther(Rigidbody* other, glm::vec2 force)
{
	other->applyForce(force);


}
#pragma endregion