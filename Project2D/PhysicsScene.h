#pragma once
#include "PhysicsObject.h"
#include "glm/glm.hpp"
#include "vector"
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addPhysicsObject(PhysicsObject* object);
	void removePhysicsObject(PhysicsObject* object);

	void Update(float deltaTime);
	void Draw();



protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_physicsObjects;

};

