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
#include "Room.h"
#include "Renderer.h"


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
    unsigned int wHeight = 1200;
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
    


    {
        Renderer renderer;

        float vertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ
        //-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        // 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        // 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       //  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        //-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       // -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        unsigned int stride = (3 + 3) * sizeof(float);

        VertexBuffer vb(vertices, 30 * stride);
        // IndexBuffer ib(indices, 6);

        VertexBufferLayout layout;
        // location 0
        layout.AddFloat(3);
        //location 1
        layout.AddFloat(3);
        VertexArray va;
        va.AddBuffer(vb, layout);

        //unsigned int indices[] = {
        //    0, 1, 2,
         //   2, 0, 4
        //};
     
        Room room(spotShader, wWidth, wHeight);

        GLCall(glEnable(GL_DEPTH_TEST));
        //glDepthFunc(GL_LESS);
        GLCall(glEnable(GL_CULL_FACE));
        glCullFace(GL_BACK);
        GLCall(glClearColor(255.0, 255.0, 255.0, 1.0));
        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                room.setModel(glm::rotate(room.getModel(), glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
            }

            spotShader.SetUniform4f("u_Color", red, 0.0f, 0.0f, 1.0f);
            renderer.Draw(va, 0, 30, spotShader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
}
