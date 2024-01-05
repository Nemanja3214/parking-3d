#define _CRT_SECURE_NO_WARNINGS

#define CRES 30 // Circle Resolution = Rezolucija kruga

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>


#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <thread>
#include <chrono>

#include "stb_image.h"
#include <string>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


int main(void)
{


    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1200;
    unsigned int wHeight = 800;
    const char wTitle[] = "Parking";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }
    //shaders and finding uniforms
    Shader spotShader("basic.vert", "basic.frag");
    spotShader.Bind();
    spotShader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.8f, 1.0f);
    float red = 1.0f;
    

    //glUseProgram(spotShader);
    //int uSpotMVP = glGetUniformLocation(spotShader, "mvp");

    float vertices[] =
    {
         0.2f,  0.2f,
         0.5f,  0.2f,
         0.35f, 0.6f,
          0.6f, 0.6f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 0, 4
    };

    {
        VertexArray va;
        VertexBuffer vb(vertices, 4 * 2 * sizeof(float));
        IndexBuffer ib(indices, 6);

        VertexBufferLayout layout;
        layout.AddFloat(2);

        va.AddBuffer(vb, layout);
        va.Bind();
        ib.Bind();
        va.Unbind();

        unsigned int stride = (2) * sizeof(float);

        glClearColor(255.0, 255.0, 255.0, 1.0);
        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            GLCall(glClear(GL_COLOR_BUFFER_BIT));
          
            spotShader.Bind();
            spotShader.SetUniform4f("u_Color", red, 0.0f, 0.0f, 1.0f);
            va.Bind();
           
            GLCall(glDrawElements(GL_TRIANGLE_STRIP, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
            spotShader.Unbind();

            if (red < 0.3)
                red += 0.1;
            else
                red -= 0.1;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
}
