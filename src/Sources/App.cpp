#include "../Headers/App.hpp"

App::App(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
{
	renderer::Camera* camera = new renderer::Camera(glm::vec3(0.0f, -.10f, .40f));
	resource::Scene* scene = new resource::Scene(title, resizable);

	physics::Physics* physics = new physics::Physics( scene);
	m_renderer = new renderer::Renderer(camera, scene, physics);
	m_controller = new Controller(camera, scene);
}

App::~App()
{
	delete m_renderer;
	delete m_controller;
}

void App::render()
{
	m_renderer->run();
}

void App::update()
{
	m_controller->update();
	m_renderer->update();

}

void App::run()
{
	while (!gui::getWindowShouldClose())
	{
		update();
		render();
	}
}
