#include "GraphDrawer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "Window.h"
#include "Renderer.h"
#include "FontRenderer.h"

void error_callback(int error, const char* msg) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}
GraphDrawer::GraphDrawer()
{
    loop();
}

void GraphDrawer::loop()
{
    glfwSetErrorCallback(error_callback);
    // init glfw
    if (glfwInit() != GLFW_TRUE)
    {
        std::cout << "Could not init GLFW\n";
        throw std::invalid_argument("Could not init GLFW\n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    

    // create window
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
    Window window(800, 800, "Graph Drawer", WINDOW_WINDOWED);
    window.MakeCurrent();
    window.SetVSync(0);
    Renderer renderer(&window);


    // init glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Could not init GLEW\n";
        throw std::invalid_argument("Could not init GLEW\n");
    }
    renderer.InitShaders();


    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    FontRenderer::Setup(renderer.GetShader());

    // loop

    while (!glfwWindowShouldClose(window.GetHandle()))
    {
        renderer.Draw();
        window.Update();
    }

    // exit program
    glfwSetErrorCallback(nullptr);
    FontRenderer::Exit();
    glfwTerminate();

}
