#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctions[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere,
};

#pragma region Constructors
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}
PhysicsScene::~PhysicsScene() {
	for (auto pObject : m_physicsObjects) {
		delete pObject;
	}
}
#pragma endregion
#pragma region Add/Remove Physics Objects
void PhysicsScene::addPhysicsObject(PhysicsObject* object) {
	m_physicsObjects.push_back(object);
}

void PhysicsScene::removePhysicsObject(PhysicsObject* object) {
	for (std::vector<PhysicsObject*>::iterator i = m_physicsObjects.begin(); i != m_physicsObjects.end(); i++) {
		if (*i == object) {
			m_physicsObjects.erase(i);
			return;
		}
	}
}
#pragma endregion
#pragma region Update/Draw
void PhysicsScene::Update(float deltaTime) {
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	// update each physics object a fixed amount of times each second
	while (accumulatedTime >= m_timeStep) {
		for (auto pObject : m_physicsObjects) {
			pObject->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		checkCollisions();
	}

}

void PhysicsScene::Draw() {
	for (auto pObject : m_physicsObjects) {
		pObject->Draw();
	}
}
#pragma endregion
#pragma region CollisionHandling

void PhysicsScene::checkCollisions() {
	// check for collisions
	int objectcount = m_physicsObjects.size();
	for (int x = 0; x < objectcount - 1; x++) {
		for (int y = x + 1; y < objectcount; y++) {
			PhysicsObject* object1 = m_physicsObjects[x];
			PhysicsObject* object2 = m_physicsObjects[y];

			int id1 = object1->getShapeID();
			int id2 = object2->getShapeID();
			// choose the function via pointer
			int funcIdx = (id1 * SHAPE_COUNT) + id2;
			fn collisionFunction = collisionFunctions[funcIdx];
			if (collisionFunction != nullptr) { // if collision function points to an actual function
				collisionFunction(object1, object2);
			}
		}
	}
}
/// <summary>
/// handles collision of two spheres hitting each other
/// </summary>
/// <param name="sphere1"></param>
/// <param name="sphere2"></param>
/// <returns>true if there is a collision, false if there isn't</returns>
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr) { // double check if both items worked
		float requiredDistance = sphere1->getRadius() + sphere2->getRadius(); // the minimum distance between to not collide
		float xDist = fabsf(sphere1->getPosition().x - sphere2->getPosition().x);
		float yDist = fabsf(sphere1->getPosition().y - sphere2->getPosition().y);

		float distance = sqrtf(powf(xDist, 2) + powf(yDist, 2));

		if (distance <= requiredDistance) {
			sphere1->resolveCollision(sphere2);
			return true;
		}
	}
	return false;
}
/// <summary>
/// handles collision of a sphere hitting a plane
/// </summary>
/// <param name="sphereOBJ"></param>
/// <param name="planeOBJ"></param>
/// <returns></returns>
bool PhysicsScene::sphere2Plane(PhysicsObject* sphereOBJ, PhysicsObject* planeOBJ)
{
	Sphere* sphere = dynamic_cast<Sphere*>(sphereOBJ);
	Plane* plane = dynamic_cast<Plane*>(planeOBJ);
	if (sphere != nullptr && plane != nullptr) {
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();

		float intersection = sphere->getRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(sphere->getVelocity(), plane->getNormal());

		if (intersection >= 0 && velocityOutOfPlane < 0) {
			plane->resolveCollision((Rigidbody*)sphereOBJ);
			return true;
		}
	}

	return false; // objects invalid
}

/// <summary>
/// Planes cannot collide
/// </summary>
/// <returns> false </returns>
bool PhysicsScene::plane2Plane(PhysicsObject* plane1, PhysicsObject* plane2)
{
	return false;
}
/// <summary>
/// calls sphere 2 plane
/// </summary>
/// <param name="plane"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool PhysicsScene::plane2Sphere(PhysicsObject* plane, PhysicsObject* sphere)
{
	return sphere2Plane(sphere, plane);
}
#pragma endregion
