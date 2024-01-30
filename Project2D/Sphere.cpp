#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
	: Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::Draw()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 1000U, m_colour);
}
