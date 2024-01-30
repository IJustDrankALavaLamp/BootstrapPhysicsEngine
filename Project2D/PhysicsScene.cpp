#include "PhysicsScene.h"
#pragma region Constructors
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
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
		// check for collisions
		int objectcount = m_physicsObjects.size();
		for (int x = 0; x < objectcount - 1; x++) {
			for (int y = x + 1; y < objectcount; y++) {
				PhysicsObject* object1 = m_physicsObjects[x];
				PhysicsObject* object2 = m_physicsObjects[y];

				// TEMPORARY COLLISION BECAUSE ONLY HAVE SPHERES
				sphere2Sphere(object1, object2);
			}
		}
	}

}

void PhysicsScene::Draw() {
	for (auto pObject : m_physicsObjects) {
		pObject->Draw();
	}
}
#pragma endregion
#pragma region CollisionHandling
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr) {
		float requiredDistance = sphere1->getRadius() + sphere2->getRadius(); // the minimum distance between to not collide
		float xDist = fabsf(sphere1->getPosition().x - sphere2->getPosition().x);
		float yDist = fabsf(sphere1->getPosition().y - sphere2->getPosition().y);

		float distance = sqrtf(powf(xDist, 2) + powf(yDist, 2));
		if (distance <= requiredDistance) {
			sphere1->Stop();
			sphere2->Stop();
		}

	}
	
	return false;
}
#pragma endregion