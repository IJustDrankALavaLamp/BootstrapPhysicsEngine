#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
public:
	Box(glm::vec2 pos, glm::vec2 velocity, glm::vec2 extents,float orientation, float mass, glm::vec4 colour);
	Box(glm::vec2 pos, glm::vec2 velocity, glm::vec2 extents, float orientation, float mass, glm::vec4 colour, float AngularVel);
#pragma region Getters
	glm::vec2 getExtents() { return m_extents; }

	float getWidth() { return m_extents.x * 2; };
	float getHeight() { return m_extents.y * 2; };

	glm::vec2 getLocalX() { return m_localX; }
	glm::vec2 getLocalY() { return m_localY; }

	glm::vec4 getColour() { return m_colour; };
#pragma endregion

	void FixedUpdate(glm::vec2 gravity, float timeStep);
	void Draw();

	bool BoxCornerCheck(const Box& box, vec2& contact, int& numContacts, float& pen, vec2& edgeNormal);
};