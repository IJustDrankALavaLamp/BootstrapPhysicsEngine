#pragma once
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Rigidbody.h"
#include "Plane.h"
#include "Box.h"
#include "glm/glm.hpp"
#include "vector"
#include <cmath>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addPhysicsObject(PhysicsObject* object);
	void removePhysicsObject(PhysicsObject* object);

	void Update(float deltaTime);
	void Draw();

	void setGravity(glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; };
	float getTimeStep() const { return m_timeStep; };

	void checkCollisions();

	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool sphere2Box(PhysicsObject*, PhysicsObject*);

	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool plane2Box(PhysicsObject*, PhysicsObject*);
	
	static bool box2Box(PhysicsObject*, PhysicsObject*);
	static bool box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool box2Plane(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_physicsObjects;
};

