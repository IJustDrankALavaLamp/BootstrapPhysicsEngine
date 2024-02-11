#include "MouseObj.h"
MouseObj::MouseObj() : PhysicsObject(ShapeType::MOUSE) {
	pos = vec2(0, 0);
}


void MouseObj::Draw()
{
	aie::Gizmos::add2DCircle(pos, 3, 100, vec4(1, 0, 0, 0));
}

void MouseObj::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void MouseObj::setPos(vec2 npos)
{
	npos.x -= 640;
	npos.y -= 360;
	npos.x = npos.x / 6.25;
	npos.y = npos.y / 6.25;

	pos = npos;
}

void MouseObj::setPos(int mouseX, int mouseY)
{
	mouseX -= 640;
	mouseY -= 360;
	mouseX = mouseX / 6.25;
	mouseY = mouseY / 6.25;

	pos = vec2(mouseX, mouseY);
}