//  Chao 19/101/27
// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION

#include <Application/Application.h>


#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <stdlib.h>
// getcwd() both include in <direct.h> and <unistd.h>
#elif WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int Close();


Application* app;

char const * FileSystem::app_root;

int windowWidth;
int windowHeight;

int main()
{
    #ifdef WIN32
        char buffer[512];
        FileSystem::app_root = getcwd(buffer, sizeof(buffer));
    
    #elif __APPLE__
    // macos
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0)
        {
            string directory;
            directory = realpath("chaoGL",path); // symboollink in case
            
            const size_t last_slash_idx = directory.rfind('/');
            if (std::string::npos != last_slash_idx)
            {
                directory = directory.substr(0, last_slash_idx);
            }
            FileSystem::app_root = directory.c_str();
            printf("[MacOS] current path is: %s\n", FileSystem::app_root);
            //FileSystem::app_root = FileSystem::GetDirectory(path).c_str();
        }

    
        else
            printf("buffer too small; need size %u\n", size);

    #endif
    

	// App & Sections
	app = new Application();


	
	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(Application::GetWindow()))
	{
		// Input
		// Check if hit esc key

		// Render
		Application::OnFrameBegin();

		Application::Update();

		// on ftame end
		Application::OnFrameEnd();
	}

	return Close();

}


int Close()
{
	//Release resource while terminate
	glfwTerminate();
	delete app;

	return 0;
}


