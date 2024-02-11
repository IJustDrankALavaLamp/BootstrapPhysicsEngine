#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include "PhysicsScene.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	mouse = new MouseObj();

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0,0));
	m_physicsScene->setTimeStep(0.01f);
	// initialize objects
	Sphere* ball1 = new Sphere(glm::vec2(-40, 10), glm::vec2(0, 0), 3.0f, 5, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(40, 10), glm::vec2(-50, 0), 3.0f, 5, glm::vec4(0, 0, 1, 1));
	Sphere* ball3 = new Sphere(glm::vec2(0, 10), glm::vec2(0, 0), 3.0f, 5, glm::vec4(0, 1, 1, 1));

	Box* box = new Box(glm::vec2(-25, 5), glm::vec2(10, 0), glm::vec2(5, 5), 0, 3.0f, glm::vec4(1, 1, 0, 1));
	Box* box2 = new Box(glm::vec2(0, 0), glm::vec2(10, 0), glm::vec2(5, 5), 0, 3.0f, glm::vec4(1, 0.5, 0, 1));
	Box* box3 = new Box(glm::vec2(25, 5), glm::vec2(10, 0), glm::vec2(5, 5), 0, 3.0f, glm::vec4(1, 0, 1, 1));

	Plane* plane = new Plane(glm::vec2(0,1), -58);
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -100);
	Plane* plane3 = new Plane(glm::vec2(1, 0), -100);
	Plane* plane4 = new Plane(vec2(0, -1), -55);

	// add objects to scene
	m_physicsScene->addPhysicsObject(ball1);
	//m_physicsScene->addPhysicsObject(ball2);
	//m_physicsScene->addPhysicsObject(ball3);

	//m_physicsScene->addPhysicsObject(box);
	//m_physicsScene->addPhysicsObject(box2);
	//m_physicsScene->addPhysicsObject(box3);

	m_physicsScene->addPhysicsObject(plane);
	m_physicsScene->addPhysicsObject(plane2);
	m_physicsScene->addPhysicsObject(plane3);
	m_physicsScene->addPhysicsObject(plane4);



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

	// set mouse location
	int mouseX = 0;
	int mouseY = 0;
	input->getMouseXY(&mouseX, &mouseY);
	mouse->setPos(mouseX, mouseY);

	aie::Gizmos::clear();
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		std::cout << "mouse X: " << mouseX << " mouseY: " << mouseY << std::endl;

		int i = 0;
		std::vector<Rigidbody*> spheres = m_physicsScene->getSpheres();
		for (auto pObj : spheres) {
			std::cout << "Sphere" << i << ": X: " << pObj->getPosition().x << " Y: " << pObj->getPosition().y << std::endl;
			i++;
		}
	}
	if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT)) {
		std::cout << "mouse X: " << mouseX << " mouseY: " << mouseY << std::endl;
	}
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	static float aspectRatio = 16 / 9.f;

	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100/aspectRatio, 100/aspectRatio, -1.0f, 1.0f));

	m_2dRenderer->drawText(m_font, "Press ESC to quit", 2, 15);

	// done drawing sprites
	m_2dRenderer->end();
}