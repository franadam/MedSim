#include "../Headers/App.hpp"

App::App(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
{
	Camera* camera = new Camera(glm::vec3(0.0f, -.10f, .40f));
	Scene* scene = new Scene(title, resizable);

	m_renderer = new Renderer(camera, scene);
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
