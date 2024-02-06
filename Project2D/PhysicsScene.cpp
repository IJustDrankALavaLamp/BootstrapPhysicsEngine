#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctions[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box
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

#pragma region SphereCollisions
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
			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
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
		glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
		if (intersection >= 0 && velocityOutOfPlane < 0)
		{
			plane->resolveCollision((Rigidbody*)sphereOBJ, contact);
			return true;
		}
	}

	return false; // objects invalid
}
bool PhysicsScene::sphere2Box(PhysicsObject* sphereObj, PhysicsObject* boxObj) {
	return box2Sphere(boxObj, sphereObj);
}
#pragma endregion
#pragma region PlaneCollisions
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
bool PhysicsScene::plane2Box(PhysicsObject* planeObj, PhysicsObject* boxObj) {
	Box* box = dynamic_cast<Box*>(boxObj);
	Plane* plane = dynamic_cast<Plane*>(planeObj);

	if (box != nullptr && plane != nullptr) // check objects exist
	{
		int numContacts = 0;
		vec2 contact(0, 0);
		float contactV = 0;

		//get point on the plane
		vec2 planeOrigin = plane->getNormal() * plane->getDistance();

		for (float x = -box->getExtents().y; x < box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight()) {
				vec2 point = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
				float distFromPlane = dot(point - planeOrigin, plane->getNormal());

				vec2 displacement = x * box->getLocalX() + y * box->getLocalY();
				vec2 pointVel = box->getVelocity() + -box->getAngularVel() * vec2(-displacement.y, displacement.x);

				float velocityIntoPlane = dot(pointVel, plane->getNormal());

				if (distFromPlane < 0 && velocityIntoPlane <= 0) {
					numContacts++;
					contact += point;
					contactV += velocityIntoPlane;
				}
			}
			if (numContacts > 0) {
				plane->resolveCollision(box, contact / (float)numContacts);
				return true;
			}
		}
	}
	return false;
}
#pragma endregion
#pragma region BoxCollisions
bool PhysicsScene::box2Box(PhysicsObject* boxObj1, PhysicsObject* boxObj2) {
	Box* box1 = dynamic_cast<Box*>(boxObj1);
	Box* box2 = dynamic_cast<Box*>(boxObj2);
	if (box1 != nullptr && box2 != nullptr) 
	{
		vec2 boxPos = box2->getPosition() - box1->getPosition();
		vec2 norm(0, 0);
		vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->BoxCornerCheck(*box2, contact, numContacts, pen, norm);
		if (box2->BoxCornerCheck(*box1, contact, numContacts, pen, norm)) 
			norm = -norm;
		if (pen > 0)
			box1->resolveCollision(box2, contact / float(numContacts), &norm);
		return true;
	}
	return false;
}
bool PhysicsScene::box2Sphere(PhysicsObject* boxObj, PhysicsObject* sphereObj) {

	Box* box = dynamic_cast<Box*>(boxObj);
	Sphere* sphere = dynamic_cast<Sphere*>(sphereObj);

	if (box != nullptr && sphere != nullptr) {
		// circles position relative to box space
		vec2 circleWorldPos = sphere->getPosition() - box->getPosition();
		vec2 circleBoxPos = vec2(dot(circleWorldPos, box->getLocalX()), dot(circleWorldPos, box->getLocalY()));

		// find closest point to the circle centre
		vec2 closestPointBox = circleBoxPos;
		vec2 extents = box->getExtents();

		if (closestPointBox.x < -extents.x)
			closestPointBox.x = -extents.x;

		if (closestPointBox.x > extents.x)
			closestPointBox.x = extents.x;

		if (closestPointBox.y < -extents.y)
			closestPointBox.y = -extents.y;

		if (closestPointBox.y > extents.y)
			closestPointBox.y = extents.y;

		vec2 closestPointWorld = box->getPosition() + closestPointBox.x * box->getLocalX() + closestPointBox.y * box->getLocalY();
		vec2 circleToBox = sphere->getPosition() - closestPointWorld;
	
		if (length(circleToBox) < sphere->getRadius()) {
			vec2 direction = normalize(circleToBox);
			vec2 contact = closestPointBox;

			box->resolveCollision(sphere, contact, &direction);
		}

	}

	return false;
}
bool PhysicsScene::box2Plane(PhysicsObject* boxObj, PhysicsObject* planeObj) {
	return plane2Box(planeObj, boxObj);
}

#pragma endregion

#pragma endregion
