#include "Box.h"

Box::Box(glm::vec2 pos, glm::vec2 velocity, glm::vec2 extents, float orientation, float mass, glm::vec4 colour)
	: Rigidbody(BOX, pos, velocity, orientation, mass)
{
	m_extents = extents;
	m_colour = colour;
}

void Box::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);

	// store local axes
	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = glm::normalize(glm::vec2(cs,sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::Draw() {
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y; // top left?
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y; // top right?
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y; // bottom left
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y; // bottom right

	aie::Gizmos::add2DTri(p1,p2,p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}
