#include "Plane.h"
#include "Rigidbody.h"

Plane::Plane(glm::vec2 normal, float dTT) : PhysicsObject(ShapeType::PLANE){
	m_distanceToOrigin = dTT;
	m_normal = glm::normalize(normal);
	m_colour = vec4(1, 1, 1, 1);
	m_elasticity = .9f;
}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = colour;
	m_elasticity = .9f;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw() {
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = {.5,.5,.5,1};

	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);

	//aie::Gizmos::add2DLine(start, end, m_colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 5.0f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 5.0f, start - m_normal * 5.0f, m_colour, colourFade, colourFade);

}

void Plane::resetPosition()
{
}

void Plane::resolveCollision(Rigidbody* other, vec2 contact)
{
	// point where force is applied from
	vec2 localContact = contact - other->getPosition();

	// relitive velocity is just the other objects velocity at the contact point
	// the plane has no velocity
	vec2 relVel = other->getVelocity() + other->getAngularVel() * vec2(-localContact.y, localContact.x); // the difference between velocity
	float velIntoPlane = dot(relVel, m_normal);
	float e = (getElasticity() + other->getElasticity()) / 2.0f;
	if (dot(m_normal, relVel) >= 0)
		return;


	float r = dot(localContact, vec2(m_normal.y, -m_normal.x));
	float mass0 = 1.0f / (1.0f / other->getMass() + (r * r) / other->getMoment());
	float j = -(1+e) * velIntoPlane * mass0;

	vec2 force = m_normal * j;
	float preKE = other->getKineticEnergy();
	other->applyForce(force, localContact);
	float postKE = other->getKineticEnergy();

	float finalKE = postKE - preKE;
	if (finalKE > postKE * 0.01f)
		std::cout << "Kinetic Energy change from " << preKE * 0.01f << " to " << postKE * 0.01f << std::endl;

	float pen = dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(other, nullptr, m_normal, pen);
}
