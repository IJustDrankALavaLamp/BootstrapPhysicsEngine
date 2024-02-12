#pragma once
#include "glm/glm.hpp"
#include "vector"
#include <cmath>

class PhysicsObject;
class Rigidbody;
class MouseObj;
struct FrameInput {
public:
	bool LeftMouseDown = false;
};

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addPhysicsObject(PhysicsObject* object);
	void removePhysicsObject(PhysicsObject* object);

	void Update(float deltaTime);
	void Draw();
	void GatherInputs(); void HandleInputs();

#pragma region Getters/Setters
	void setGravity(glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; };
	float getTimeStep() const { return m_timeStep; };

	std::vector<PhysicsObject*> getObjects() { return m_physicsObjects; }
	std::vector<Rigidbody*> getSpheres();

	void setMousePos(glm::vec2 pos);

#pragma endregion
	void checkCollisions();

	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool sphere2Box(PhysicsObject*, PhysicsObject*);
	static bool sphere2Mouse(PhysicsObject* obj1, PhysicsObject* obj2) { return mouse2Sphere(obj1, obj2); }

	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool plane2Box(PhysicsObject*, PhysicsObject*);
	static bool plane2Mouse(PhysicsObject*, PhysicsObject*) { return false; };

	static bool box2Box(PhysicsObject*, PhysicsObject*);
	static bool box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool box2Plane(PhysicsObject*, PhysicsObject*);
	static bool box2Mouse(PhysicsObject* obj1, PhysicsObject* obj2) { return mouse2Box(obj1, obj2); }

	static bool mouse2Mouse(PhysicsObject*, PhysicsObject*) { return false; }
	static bool mouse2Sphere(PhysicsObject*, PhysicsObject*);
	static bool mouse2Plane(PhysicsObject*, PhysicsObject*) { return false; };
	static bool mouse2Box(PhysicsObject*, PhysicsObject*);

	static void ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_physicsObjects;
	MouseObj* m_mouse;
	FrameInput m_inputs;
};

