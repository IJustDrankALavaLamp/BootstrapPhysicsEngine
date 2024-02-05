#include "Plane.h"
using namespace glm;

Plane::Plane(glm::vec2 normal, float dTT) : PhysicsObject(ShapeType::PLANE){
	m_distanceToOrigin = dTT;
	m_normal = glm::normalize(normal);
	m_colour = vec4(1, 1, 1, 1);
}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = colour;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw() {
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = {.5,.5,.5,1};

	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);

	//aie::Gizmos::add2DLine(start, end, m_colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 5.0f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 5.0f, start - m_normal * 5.0f, m_colour, colourFade, colourFade);

}

void Plane::resetPosition()
{
}

void Plane::resolveCollision(Rigidbody* other, glm::vec2 contact)
{
	glm::vec2 relativeVelocity = other->getVelocity(); // the difference between velocity

	if (glm::dot(m_normal, relativeVelocity) >= 0)
		return; //objects are already moving apart

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), m_normal) /
		((1 / other->getMass()));

	glm::vec2 force = m_normal * j;

	other->applyForce(force, contact - other->getPosition());
}
