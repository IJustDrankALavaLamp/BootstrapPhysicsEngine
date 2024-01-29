#include "PhysicsScene.h"
#pragma region Constructors
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}
PhysicsScene::~PhysicsScene() {

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
#pragma Update/Draw
void PhysicsScene::Update(float deltaTime) {
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep) {
		for (auto pObject : m_physicsObjects) {
			pObject->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
	}

}

void PhysicsScene::Draw() {
	for (auto pObject : m_physicsObjects) {
		pObject->Draw();
	}
}
#pragma endregion