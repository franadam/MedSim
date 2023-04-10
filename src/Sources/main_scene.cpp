// Local Headers
#include "../Headers/App.hpp"

int main()
{
	App simulation("Medical TEST", gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT, gui::GL_VERSION_MAJOR, gui::GL_VERSION_MINOR, false);

	simulation.run();
	return 0;
}

