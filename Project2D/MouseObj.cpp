#include "MouseObj.h"
MouseObj::MouseObj() : PhysicsObject(ShapeType::MOUSE) {
	pos = vec2(0, 0);
}


void MouseObj::Draw()
{
	aie::Gizmos::add2DCircle(pos, radius, 100, vec4(.5, .5,.5, 0));
}

void MouseObj::Update(float deltaTime)
{
}

void MouseObj::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void MouseObj::setPos(vec2 npos)
{
	pos = npos;
}

void MouseObj::setPos(int mouseX, int mouseY)
{
	pos = vec2(mouseX, mouseY);
}