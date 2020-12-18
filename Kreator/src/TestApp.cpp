#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <map>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlApiCall(x) ClearGlErrors(); x; ASSERT(LogGlErrors(__FILE__, #x, __LINE__))

enum class ShaderType
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
    GEOMETRY = 2,
    NUM_TYPES
};

std::map<ShaderType, GLenum> shaderTypeMap = {
    {ShaderType::VERTEX, GL_VERTEX_SHADER},
    {ShaderType::FRAGMENT, GL_FRAGMENT_SHADER},
    {ShaderType::GEOMETRY, GL_GEOMETRY_SHADER} 
};

static void ClearGlErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool LogGlErrors(const char* file, const char* function, int line)
{
    while (GLenum err = glGetError())
    {
        std::cout << "OPENGL ERROR: (0x" << std::hex << err << ") in " 
                  << function << " " << file << ": " << std::dec << line << std::endl;
        return false;
    }
    return true;
}

// This function expects a file with exactly 2 shaders: Vertex and Fragment
static std::tuple<std::string, std::string> ParseShader(const std::string& srcPath)
{
    std::ifstream instream(srcPath);
    std::stringstream sources[(int)ShaderType::NUM_TYPES];
    ShaderType type(ShaderType::NONE);

    int numShaders = 0;

    std::string line;
    while (getline(instream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
          
        }
        else
        {
            sources[(int)type] << line << std::endl;
        }
    }

    return std::make_tuple(sources[0].str(),sources[1].str());
}

// This function handles an arbitray number of shaders in a single file
static std::map<ShaderType, std::string> ParseShaders(const std::string& srcPath)
{
    std::ifstream instream(srcPath);
    std::stringstream sources[(int)ShaderType::NUM_TYPES];
    std::map<ShaderType, std::string> retMap;
    ShaderType type(ShaderType::NONE);

    std::string line;
    while (getline(instream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::GEOMETRY;

        }
        else
        {
            sources[(int)type] << line << std::endl;
        }
    }

    for (int i = 0; i < (int)ShaderType::NUM_TYPES; i++)
        if (!sources[i].str().empty())
            retMap[(ShaderType)i] = sources[i].str();

    return retMap;
}

static GLuint CompileShader(GLenum shaderType, const std::string& src)
{
    GlApiCall(GLuint shaderId = glCreateShader(shaderType));
    const char* source = src.c_str();
    GlApiCall(glShaderSource(shaderId, 1, &source, nullptr));
    GlApiCall(glCompileShader(shaderId));

    GLint status;
    GlApiCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint msgLen;
        GlApiCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLen));
        char* msg = (char*)_malloca(msgLen * sizeof(char)); 
        GlApiCall(glGetShaderInfoLog(shaderId, msgLen, &msgLen, msg));
        std::string typeStr = (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
        std::cout << typeStr << " Shader compilation failed: " << msg << std::endl;
        GlApiCall(glDeleteShader(shaderId));

        return 0;
    }

    return shaderId;
}

static GLuint CreateShaderProgram(const std::string& vertSrc, const std::string& fragSrc)
{
    GlApiCall(GLuint program = glCreateProgram());
    GLuint vertId = CompileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragId = CompileShader(GL_FRAGMENT_SHADER, fragSrc);
    GlApiCall(glAttachShader(program, vertId));
    GlApiCall(glAttachShader(program, fragId));
    GlApiCall(glLinkProgram(program));
    GlApiCall(glValidateProgram(program));

    GlApiCall(glDeleteShader(vertId));
    GlApiCall(glDeleteShader(fragId));

    GlApiCall(glDetachShader(program, vertId));
    GlApiCall(glDetachShader(program, fragId));

    return program;
}

static GLuint CreateShaderProgram(const std::map<ShaderType, std::string> sourceMap)
{
    GlApiCall(GLuint program = glCreateProgram());
    std::vector<GLuint> shaderHandles;
    for (const auto& mapEntry : sourceMap)
    {
        shaderHandles.push_back( 
            CompileShader(shaderTypeMap[mapEntry.first], mapEntry.second) 
        );
        GlApiCall(glAttachShader(program, shaderHandles.back()));
    }

    GlApiCall(glLinkProgram(program));
    GlApiCall(glValidateProgram(program));

    for (GLuint handle : shaderHandles)
    {
        GlApiCall(glDeleteShader(handle));
        GlApiCall(glDetachShader(program, handle));
    }
       
    return program;
}

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

    unsigned int vertexArray;
    GlApiCall(glGenVertexArrays(1, &vertexArray));
    GlApiCall(glBindVertexArray(vertexArray));

    unsigned int vertexbuffer;
    GlApiCall(glGenBuffers(1, &vertexbuffer));
    GlApiCall(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer));
    GlApiCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    unsigned int indexbuffer;
    GlApiCall(glGenBuffers(1, &indexbuffer));
    GlApiCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer));
    GlApiCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    GlApiCall(glEnableVertexAttribArray(0));
    GlApiCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));

    //auto [vertSrc, fragSrc] = ParseShader("resources/shaders/VertexAndFrag.glsl");

    GLuint currProg = CreateShaderProgram( ParseShaders("resources/shaders/VertexAndFrag.glsl") );
    GlApiCall(glUseProgram(currProg));

    GLint loc = glGetUniformLocation(currProg, "u_color");
    GlApiCall(glUniform4f(loc, 0.2f, 0.2f, 0.8f, 1.0f));

    GlApiCall(glUseProgram(0));
    GlApiCall(glBindVertexArray(0));
    GlApiCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GlApiCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GlApiCall(glClear(GL_COLOR_BUFFER_BIT));

        GlApiCall(glUseProgram(currProg));
        GlApiCall(glUniform4f(loc, r, 0.2f, 0.8f, 1.0f));

        GlApiCall(glBindVertexArray(vertexArray));
        GlApiCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer));
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

    GlApiCall(glDeleteProgram(currProg));

    glfwTerminate();
    return 0;
}