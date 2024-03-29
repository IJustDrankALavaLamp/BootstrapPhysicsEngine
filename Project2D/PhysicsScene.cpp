#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "PhysicsObject.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "MouseObj.h"
#include "Input.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctions[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box, PhysicsScene::plane2Mouse,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,PhysicsScene::sphere2Mouse,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box, PhysicsScene::box2Mouse,
	PhysicsScene::mouse2Plane,PhysicsScene::mouse2Sphere,PhysicsScene::mouse2Box,PhysicsScene::mouse2Mouse,
};

#pragma region Constructors
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
	m_mouse = new MouseObj(this);
}
PhysicsScene::~PhysicsScene() {
	for (auto pObject : m_physicsObjects) {
		delete pObject;
	}
	delete m_mouse;
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
	HandleInputs();
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;
	Time += deltaTime;
	// update each physics object a fixed amount of times each second
	while (accumulatedTime >= m_timeStep) {
		for (auto pObject : m_physicsObjects) {
			pObject->FixedUpdate(m_gravity, m_timeStep);
			if (pObject->CheckDelete()) {
				removePhysicsObject(pObject);
				delete pObject;
			}
		}
		accumulatedTime -= m_timeStep;

		checkCollisions();
	}
	if (score != prevScore) {
		std::cout << "Score: " << score << std::endl;
		prevScore = score;
	}
	if (Time > lastSpawn + spawnPeriods)
		SpawnObject();
}

void PhysicsScene::SpawnObject() {
	for (int i = 0; i < 3; i++) {
		lastSpawn = Time;
		vec2 Velocity = vec2((-50 + rand() % 100), (20 + rand() % 80));
		vec2 Position = vec2((-100 + rand() % 200), -60);

		int randItem = rand() % 2 + 1;

		if (randItem == 1) {
			Sphere* newSphere = new Sphere(Position, Velocity, 0.5, 5, vec4(1, 0, 0, 1));
			addPhysicsObject(newSphere);
		}
		else if (randItem == 2) {
			Box* newBox = new Box(Position, Velocity, vec2(5, 5), 0, 1.0f, vec4(1, 0, 0, 1));
			addPhysicsObject(newBox);
		}
		else {
			std::cout << "wtf you broke it randItem == " << randItem << std::endl;
		}
	}
}

void PhysicsScene::HandleInputs() {
	aie::Input* input = aie::Input::getInstance();
	m_inputs = FrameInput();
	m_inputs.LeftMouseDown = input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT);

	if (m_inputs.LeftMouseDown)
		m_mouse->Cutting = true;
	else {
		m_mouse->Cutting = false;
		multiplier = 1;
	}
		
}

void PhysicsScene::Draw() {
	for (auto pObject : m_physicsObjects) {
		pObject->Draw();
	}
	m_mouse->Draw();
}
#pragma endregion
#pragma region CollisionHandling

std::vector<Rigidbody*> PhysicsScene::getSpheres()
{
	std::vector<Rigidbody*> retVal;

	for (auto pObj : m_physicsObjects) {
		Rigidbody* cast = dynamic_cast<Rigidbody*>(pObj);
		if (cast != nullptr && cast->getShapeID() == SPHERE)
			retVal.push_back(cast);
	}

	return retVal;
}

void PhysicsScene::setMousePos(glm::vec2 pos)
{
	if (m_mouse != nullptr) 
		m_mouse->setPos(pos); 
}

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
	if (m_mouse->Cutting) {
		for (int x = 0; x < objectcount; x++) {
			PhysicsObject* obj = m_physicsObjects[x];

			int id = obj->getShapeID();
			int funcIdx = (id * SHAPE_COUNT) + MOUSE;
			fn collisionFunc = collisionFunctions[funcIdx];
			if (collisionFunc != nullptr) {
				collisionFunc(obj, m_mouse);
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
		float dist = length(sphere1->getPosition() - sphere2->getPosition());
		
		float penetration = sphere1->getRadius() + sphere2->getRadius() - dist;

		if (penetration > 0) {
			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()), nullptr ,penetration);
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
	
		float penetration = sphere->getRadius() - length(circleToBox);

		if (penetration > 0) {
			vec2 direction = normalize(circleToBox);
			vec2 contact = closestPointWorld;

			box->resolveCollision(sphere, contact, &direction, penetration);
		}

	}

	return false;
}
bool PhysicsScene::box2Plane(PhysicsObject* boxObj, PhysicsObject* planeObj) {
	return plane2Box(planeObj, boxObj);
}

#pragma endregion
#pragma region MouseCollisions
bool PhysicsScene::mouse2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
	MouseObj* mouse = dynamic_cast<MouseObj*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);
		if (mouse != nullptr && sphere != nullptr) { // double check if both items worked
			if (sphere->isInteractable()) {}
			float dist = length(mouse->getPosition() - sphere->getPosition());

			float penetration = mouse->getRadius() + sphere->getRadius() - dist;

			if (penetration > 0) {
				mouse->resolveCollision(sphere, 0.5f * (mouse->getPosition() + sphere->getPosition()));
				return true;
			}
		}

	return false;
}
bool PhysicsScene::mouse2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
	MouseObj* mouse = dynamic_cast<MouseObj*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);
	if (box != nullptr && mouse != nullptr) {
		// circles position relative to box space
		vec2 circleWorldPos = mouse->getPosition() - box->getPosition();
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
		vec2 circleToBox = mouse->getPosition() - closestPointWorld;

		float penetration = mouse->getRadius() - length(circleToBox);

		if (penetration > 0) {
			vec2 direction = normalize(circleToBox);
			vec2 contact = closestPointWorld;

			mouse->resolveCollision(box, contact, &direction, penetration);
		}

	}

	return false;
}


#pragma endregion

void PhysicsScene::ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm:: vec2 norm, float pen)
{
	float body2Mass = body2 ? body2->getMass() : INT_MAX;
	float body1Factor = body2Mass / (body1->getMass() + body2Mass);

	body1->setPosition(body1->getPosition() - body1Factor * norm * pen);
	if (body2)
		body2->setPosition(body2->getPosition() + (1 - body1Factor) * norm * pen);
}

#pragma endregion
