#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
	: Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;

	m_moment = 0.05f * mass * radius * radius;
}

Sphere::~Sphere()
{
}

void Sphere::Draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 10000, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
	
}
