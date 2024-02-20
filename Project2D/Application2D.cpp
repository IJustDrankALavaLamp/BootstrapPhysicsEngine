#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include "PhysicsScene.h"
#include "string.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0,-9.81));
	m_physicsScene->setTimeStep(0.01f);
	// initialize objects
	Sphere* ball1 = new Sphere(glm::vec2(-40, 10), glm::vec2(0, 10), 1.0f, 5, glm::vec4(1, 0, 0, 1));

	Box* box = new Box(glm::vec2(-25, 5), glm::vec2(0, 0), glm::vec2(5, 5), 0, 1.0f, glm::vec4(1, 1, 0, 1));

	Plane* planeRight = new Plane(glm::vec2(-1, 0), -100);
	Plane* planeLeft = new Plane(glm::vec2(1, 0), -100);
	Plane* planeTop = new Plane(vec2(0, -1), -55);

	// add objects to scene
	m_physicsScene->addPhysicsObject(planeRight);
	m_physicsScene->addPhysicsObject(planeLeft);
	m_physicsScene->addPhysicsObject(planeTop);



	return true;
}

void Application2D::shutdown() {
	delete m_physicsScene;
	delete m_font;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {
	// input example
	aie::Input* input = aie::Input::getInstance();
	
	int mouseX, mouseY;
	input->getMouseXY(&mouseX, &mouseY);
	m_physicsScene->setMousePos(screen2World(vec2(mouseX, mouseY)));

	aie::Gizmos::clear();
	m_physicsScene->Update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	//draw physics objects
	m_physicsScene->Draw();
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	m_2dRenderer->setRenderColour(1, 1, 1, 1);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 70);
	std::string str = "Score: " + std::to_string(m_physicsScene->getScore());
	char const* charr = str.c_str();
	m_2dRenderer->drawText(m_font, charr, 0, 720 - 40);
	// done drawing sprites
	m_2dRenderer->end();	

	
	
}

vec2 Application2D::screen2World(vec2 screenPos) {
	vec2 worldPos = screenPos;

	// move to centre of screen
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	//scale to extents
	worldPos.x *= 2.0f * extents / getWindowWidth();
	worldPos.y *= 2.0f * extents / (aspectRatio * getWindowHeight());

	return worldPos;
}

