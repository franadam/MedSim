#pragma once
#ifndef _APP_
#define _APP_

// System Headers
#include "glitter.hpp"

// Local Headers
#include "Gui.hpp"
#include "Controller.hpp"
#include "Renderer.hpp"

class App
{
public:
	//Constructors / Destructors
	App(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable = false
	);
	virtual ~App();


	//Functions
	void run();

private:
	///Variables
	Renderer* m_renderer;
	Controller* m_controller;

	void update();
	void render();

};


#endif