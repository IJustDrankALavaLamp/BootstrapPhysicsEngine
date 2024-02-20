#include "Application2D.h"

int main() {
	// this is a test line
	// allocation
	auto app = new Application2D();
	srand(196);
	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}