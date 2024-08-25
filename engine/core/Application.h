#pragma once

#include<memory>
#include"window/WindowPlatform.h"

namespace Engine {
	struct ApplicationConfiguration {
		int Width, Height;
		const char* Title;
		EWindowPlatformSpec WindowSpec;
	};

	class Application {
	public:
		virtual ~Application() = default;
		bool Init();
		virtual void OnInitClient() = 0;
		void Run();
		virtual void OnShutdownClient() = 0;
		void Shutdown();
	protected:
		Application() = default;
		Application(const ApplicationConfiguration&);
	private:
		ApplicationConfiguration mConfig;
		std::unique_ptr<NativeWindow> mNativeWindow;
	};

	extern Application* CreateApplication();
}