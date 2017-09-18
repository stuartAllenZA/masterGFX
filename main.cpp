#include <GraphicsEngine.hpp>


int main() {
	GLFWwindow	*window;
	GraphicsEngine	gfx(window);
	gfx.initSystems();
	gfx.init();
	bool exitStatus = false;
	while (!exitStatus) {
		exitStatus = gfx.processInput();
		gfx.render();
	}
}
