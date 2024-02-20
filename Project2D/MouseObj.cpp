#include "MouseObj.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Box.h"

MouseObj::MouseObj(PhysicsScene* paront) : PhysicsObject(ShapeType::MOUSE) {
	pos = vec2(0, 0);
	parent = paront;
}


void MouseObj::Draw()
{
	aie::Gizmos::add2DCircle(pos, radius, 100, vec4(.5, .5, .5, 0));
}

void MouseObj::Update(float deltaTime)
{
}

void MouseObj::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void MouseObj::resolveCollision(Rigidbody* other, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
	int type = other->getShapeID();
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : other->getPosition() - pos); // the normal of the plane the collision occurs on
	glm::vec2 perp(normal.y, -normal.x); //the vector perpendicular to the collision normal

	if (other->isInteractable()) {
		switch (type) {
		case SPHERE:
		{
			Sphere* sphere = dynamic_cast<Sphere*>(other);
			Sphere* newSphere = new Sphere(vec2(sphere->getPosition().x + (radius * .75f), sphere->getPosition().y+(radius * .75f)), sphere->getVelocity(),
				sphere->getMass() / 2, sphere->getRadius() * .75f, sphere->getColour());
			newSphere->setInteractable(false);

			Sphere* newSphere2 = new Sphere(vec2(sphere->getPosition().x - (radius * .75f), sphere->getPosition().y - (radius * .75f)), sphere->getVelocity(),
				sphere->getMass() * 2.0f, sphere->getRadius() * 0.75f, sphere->getColour());
			newSphere2->setInteractable(false);

			parent->addPhysicsObject(newSphere);
			parent->addPhysicsObject(newSphere2);

			parent->removePhysicsObject(sphere);
			delete(sphere); 
		}
		break;
		case BOX:
		{
			Box* box = dynamic_cast<Box*>(other);
			Box* newBox = new Box(vec2(box->getPosition().x+2, box->getPosition().y+2), box->getVelocity(), 
				box->getExtents() * 0.5f, box->getOrientation(), box->getMass(), box->getColour(), box->getAngularVel());
			Box* newBox2 = new Box(vec2(box->getPosition().x-2, box->getPosition().y-2), box->getVelocity(),
				box->getExtents() * 0.5f, box->getOrientation(), box->getMass(), box->getColour(), box->getAngularVel());
			Box* newBox3 = new Box(vec2(box->getPosition().x-2, box->getPosition().y+2), box->getVelocity(),
				box->getExtents() * 0.5f, box->getOrientation(), box->getMass(), box->getColour(), box->getAngularVel());
			Box* newBox4 = new Box(vec2(box->getPosition().x+2, box->getPosition().y-2), box->getVelocity(),
				box->getExtents() * 0.5f, box->getOrientation(), box->getMass(), box->getColour(), box->getAngularVel());

			newBox->setInteractable(false);
			newBox2->setInteractable(false);
			newBox3->setInteractable(false);
			newBox4->setInteractable(false);
			
			parent->addPhysicsObject(newBox);
			parent->addPhysicsObject(newBox2);
			parent->addPhysicsObject(newBox3);
			parent->addPhysicsObject(newBox4);

			parent->removePhysicsObject(box);
			delete box;

			std::cout << "BoxCollisionSuccessful" << std::endl;
		}
		break;

		default: // if other doesnt have a collidable part return
			return;
			break;
		}
		parent->addScore(1 * parent->getMulti());
		parent->addMulti(1);
	}
}

void MouseObj::setPos(vec2 npos)
{
	pos = npos;
}

void MouseObj::setPos(int mouseX, int mouseY)
{
	pos = vec2(mouseX, mouseY);
}