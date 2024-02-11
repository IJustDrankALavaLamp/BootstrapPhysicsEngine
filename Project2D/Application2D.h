#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "MouseObj.h"
#include "glm/glm.hpp"
#include <glm\ext.hpp>
class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	PhysicsScene* m_physicsScene;
	MouseObj* mouse;
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};