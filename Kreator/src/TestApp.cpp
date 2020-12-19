#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
        glm::mat4 model = glm::scale(glm::mat4(1.0), { 2.0, 2.0f, 2.0f });
        glm::mat4 mvp = proj * view * model;

        Shader shaderProgram("resources/shaders/VertexAndFrag.glsl");
        shaderProgram.Bind();
        shaderProgram.SetUniform4f("u_Color", 0.2f, 0.2f, 0.8f, 1.0f);
        shaderProgram.SetUniform1i("u_TexSlot", 0);
        shaderProgram.SetUniformMat4f("u_MVP", mvp);
        
        Texture tex("resources/textures/Kreator.png");
        tex.Bind();
        
        va.Unbind();
        shaderProgram.Unbind();
        vbuf.Unbind();
        ibuf.Unbind(); 
        Renderer rend;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            rend.Clear();

            shaderProgram.Bind();
            shaderProgram.SetUniform4f("u_Color", r, 0.2f, 0.8f, 1.0f);

            va.Bind();
            ibuf.Bind();
            rend.Draw(va, ibuf, shaderProgram);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}