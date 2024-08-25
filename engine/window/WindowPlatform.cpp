#include"WindowPlatform.h"
#include<cassert>

namespace Engine {
	NativeWindow* WindowPlatform::Create(EWindowPlatformSpec spec) {
		NativeWindow* window = nullptr;

		switch (spec)
		{
			case EWindowPlatformSpec::GLFW: window = new GLFWPlatformWindow();
			case EWindowPlatformSpec::SDL: assert("SDL Window not supported");
			case EWindowPlatformSpec::None: assert("Unknown Window detected");
			default: assert("Unknown Window detected");
		}

		return dynamic_cast<NativeWindow*>(window);
	}
}