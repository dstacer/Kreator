#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <iostream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
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
    window = glfwCreateWindow(640, 480, "Kreator", NULL, NULL);
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
    
    {
        float verts[] = {
            -0.6f, -0.25f, 0.0f, 0.0f,
             0.6f, -0.25f, 1.0f, 0.0f,
             0.6f,  0.25f, 1.0f, 1.0f,
            -0.6f,  0.25f, 0.0f, 1.0f
        };

        unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;
        VertexBuffer vbuf(verts, 4 * 4 * sizeof(float));
        
        VertexBufferLayout vblayout;
        vblayout.Push<float>(2);
        vblayout.Push<float>(2);
        va.AddBuffer(vbuf, vblayout);

        IndexBuffer ibuf(indices, 6);
        ibuf.Bind();

        glm::mat4 proj = glm::ortho(-1.6f, 1.6f, -1.2f, 1.2f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0);
        glm::vec3 translation(0.0, 0.0f, 0.0f);

        Shader shaderProgram("resources/shaders/VertexAndFrag.glsl");
        shaderProgram.Bind();
        shaderProgram.SetUniform4f("u_Color", 0.2f, 0.2f, 0.8f, 1.0f);
        shaderProgram.SetUniform1i("u_TexSlot", 0);
        
        Texture tex("resources/textures/Kreator.png");
        tex.Bind();
        
        va.Unbind();
        shaderProgram.Unbind();
        vbuf.Unbind();
        ibuf.Unbind(); 
        Renderer rend;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        float r = 0.0f;
        float increment = 0.05f;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            rend.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0), translation);
            glm::mat4 mvp = proj * view * model;
            shaderProgram.Bind();
            shaderProgram.SetUniform4f("u_Color", r, 0.2f, 0.8f, 1.0f);
            shaderProgram.SetUniformMat4f("u_MVP", mvp);


            va.Bind();
            ibuf.Bind();
            rend.Draw(va, ibuf, shaderProgram);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;
            // Our state
            bool show_demo_window = true;
            bool show_another_window = false;
            

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::SliderFloat3("float", &translation.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }


            ImGui::Render();
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}