#include "Application.h"

int main() {
	Engine::Application* application = Engine::CreateApplication();

	if (application->Init()) {
		application->Run();
	}

	application->Shutdown();
	delete application;

	return 0;
}