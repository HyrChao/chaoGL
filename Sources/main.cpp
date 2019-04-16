//  Chao 19/101/27
// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/

// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#include <Application/Application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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


//GLFWwindow* window;
GLFWwindow* window;
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
    
    // init GLFW
	glfwInit();

	//Window
	windowWidth = 800;
	windowHeight = 800;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        // Mac 10.9 later using OpenGL4.1
        // need campative if using version 3.3
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   //This is for mac OS

	// Create a new window object
	window = glfwCreateWindow(windowWidth, windowHeight, "chaoGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Rigist window adjust call back function 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// App & Sections
	app = new Application(window,windowWidth,windowHeight);


	
	//-------------------------------------------------------------
	// Loop
	while (!glfwWindowShouldClose(Application::GetWindow()))
	{
		// Input
		// Check if hit esc key

		// Render
		Application::OnFrameBegin();

		Application::Update();
		// Event
		glfwPollEvents();

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

// Function called while change window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Define viewport
	glViewport(0, 0, width, height);
}
