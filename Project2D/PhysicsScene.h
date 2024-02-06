#pragma once
#include "Box.h"
#include "glm/glm.hpp"
#include "PhysicsObject.h"
#include "Plane.h"
#include "Rigidbody.h"
#include "Sphere.h"
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

	static void ApplyContactForces(Rigidbody* body1, Rigidbody* body2, vec2 norm, float pen);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_physicsObjects;
};

