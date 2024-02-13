#pragma once
#include "PhysicsObject.h"
class Rigidbody;
class PhysicsScene;

class MouseObj : public PhysicsObject
{
private:
	vec2 pos;
	const float radius = 3;
	
	PhysicsScene* parent;

public:
	bool Cutting = false;

	MouseObj(PhysicsScene*);

	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void resolveCollision(Rigidbody* other, glm::vec2 contact, glm::vec2* collisionNormal = nullptr, float pen = 0);


	//Setters
	void setPos(vec2 npos);
	void setPos(int, int);

	//Getters
	int getX() { return pos.x; }
	int getY() { return pos.y; }
	vec2 getPosition() { return pos; }
	float getRadius() { return radius; }
};

