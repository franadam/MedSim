#include "../Headers/App.hpp"

App::App(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
{

	initOpenGL(title, resizable);

	m_camera = new renderer::Camera(glm::vec3(0.10f, 0.10f, 0.20f));
	m_scene = new resource::Scene();
	physics::Physics* physics = new physics::Physics();
	m_renderer = new renderer::Renderer(m_camera, m_scene, physics);
	m_controller = new Controller(m_camera, m_scene);
}

App::~App()
{
	delete m_scene;
	delete m_camera;
	delete m_renderer;
	delete m_controller;
}

void App::initOpenGL(const char* title, bool resizable) {
	gui::initGLFW();
	gui::initWindow(title, resizable);
	gui::initGlad();
	gui::initFramebuffer();
	gui::setCallbacks();
	gui::initOpenGLOptions();
}

void App::render()
{
	m_renderer->run();
}

void App::update()
{
	m_controller->update();
	//m_renderer->update();

}

void App::run()
{
	while (!gui::getWindowShouldClose())
	{
		update();
		render();
	}
}
