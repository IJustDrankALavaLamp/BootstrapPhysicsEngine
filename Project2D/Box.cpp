#include "Box.h"

Box::Box(glm::vec2 pos, glm::vec2 velocity, glm::vec2 extents, float orientation, float mass, glm::vec4 colour)
	: Rigidbody(BOX, pos, velocity, orientation, mass)
{
	m_localX = vec2(0, 0);
	m_localY = vec2(0, 0);

	m_extents = extents;
	m_colour = colour;

	m_elasticity = 0.5f;

	m_moment = 1.0f / 12.0f * mass * getWidth() * getHeight();
	m_moment *= 10.0f;
}

void Box::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);
	// store local axes
	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));

}

void Box::Draw() {
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y; // bottom left
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y; // bottom right
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y; // top left
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y; // top right

	aie::Gizmos::add2DTri(p1,p2,p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);

	aie::Gizmos::add2DCircle(p1, 1, 100, vec4(1, 1, 1, 1)); // white
	aie::Gizmos::add2DCircle(p2, 1, 100, vec4(0, 1, 1, 1)); // yellow
	aie::Gizmos::add2DCircle(p3, 1, 100, vec4(1, 0, 1, 1)); // pink
	aie::Gizmos::add2DCircle(p4, 1, 100, vec4(1, 1, 0, 1)); // cyan
}
/// <summary>
/// check if another box is touching this
/// </summary>
/// <param name="box - the other box"></param>
/// <param name="contact"></param>
/// <param name="numContacts"></param>
/// <param name="pen"></param>
/// <param name="edgeNormal"></param>
/// <returns></returns>
bool Box::BoxCornerCheck(const Box& box, vec2& contact, int& numContacts, float& pen, vec2& edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = box.m_extents.x * 2;
	float boxH = box.m_extents.y * 2;
	int numLocalContacts = 0;
	vec2 localContact(0, 0);
	bool first = true;

	// loop all box corners
	for (float x = -box.m_extents.x; x < boxW; x += boxW) {
		for (float y = -box.m_extents.y; y < boxH; y += boxH) {
			// position in worldspace
			vec2 p = box.m_position + x * box.m_localX + y * box.m_localY;

			//position in box's space
			vec2 p0(dot(p - m_position, m_localX), dot(p - m_position, m_localY));

			// update extents in each direction in the box's space
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// if the corner is inside this box add to contact points
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y && p0.y <= m_extents.y) {
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}
	if(maxX <= -m_extents.x || minX >= m_extents.x || maxY <= -m_extents.y || minY >= m_extents.y)
		return false;

	if (numLocalContacts == 0)
		return false;

	bool res = false;
	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float) numLocalContacts;
	numContacts++;

	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}

	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}

	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}

	return res;

}
