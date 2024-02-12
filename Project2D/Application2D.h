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

	virtual vec2 screen2World(vec2);
protected:
	PhysicsScene* m_physicsScene;
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
private:
	const float extents = 100;
	const float aspectRatio = 16.0f / 9.0f;
};