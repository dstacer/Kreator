#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        float positions[8] = {
            -0.5, -0.5,
             0.5, -0.5,
             0.5,  0.5,
            -0.5,  0.5,
        };

        unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;
        VertexBuffer vbuf(positions, 4 * 2 * sizeof(float));
        
        VertexBufferLayout vblayout;
        vblayout.Push<float>(2);
        
        va.AddBuffer(vbuf, vblayout);

        IndexBuffer ibuf(indices, 6);
        ibuf.Bind();

        Shader shaderProgram("resources/shaders/VertexAndFrag.glsl");
        shaderProgram.Bind();
        shaderProgram.SetUniform4f("u_color", 0.2f, 0.2f, 0.8f, 1.0f);
        
        va.Unbind();
        shaderProgram.Unbind();
        vbuf.Unbind();
        ibuf.Unbind(); 

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GlApiCall(glClear(GL_COLOR_BUFFER_BIT));

            shaderProgram.Bind();
            shaderProgram.SetUniform4f("u_color", r, 0.2f, 0.8f, 1.0f);

            va.Bind();
            ibuf.Bind();
            GlApiCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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