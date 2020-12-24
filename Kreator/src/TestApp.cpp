#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <iostream>

#include "Renderer.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestTransform.h"
#include "tests/TestFlatColor.h"
#include "tests/TestBatch.h"
#include "tests/TestStaticBatch.h"
#include "TestApp.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 675, "Kreator", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    /* Init glew */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    std::cout << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << " "
        << glGetString(GL_VERSION) << " " << glGetString(GL_SHADING_LANGUAGE_VERSION)
        << std::endl;
    
    
        test::Test* currentTest(nullptr);
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;
        testMenu->AddTest<test::TestClearColor>("Test Clear Color");
        testMenu->AddTest<test::TestTexture>("Test Texture");
        testMenu->AddTest<test::TestTransform>("Test Transform");
        testMenu->AddTest<test::TestFlatColor>("Test Flat Color");
        testMenu->AddTest<test::TestBatch>("Test Batch");
        testMenu->AddTest<test::TestStaticBatch>("Test Static Batch");

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            Renderer rend;
            rend.SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
            rend.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("TestMenu");
                if (currentTest != testMenu && ImGui::Button(" <- "))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImguiRender();
                ImGui::End();
            }

            ImGui::Render();
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}