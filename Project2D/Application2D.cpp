#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);

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
	
	aie::Gizmos::clear();
	
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

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