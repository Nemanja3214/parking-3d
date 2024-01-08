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
#include "House.h"


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
    Shader roomShader("basic.vert", "basic.frag");
    roomShader.Unbind();
    Shader houseShader("basic.vert", "basic.frag");

    {
        Renderer renderer;

        float cubeVertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

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


        // ROOM
        roomShader.Bind();
        VertexArray roomVa;
        VertexBuffer roomVb(cubeVertices, 36 * stride);

        unsigned int indices[] = {
         6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
         IndexBuffer ib(indices, 30);

        VertexBufferLayout roomLayout;
        // location 0
        roomLayout.AddFloat(3);
        //location 1
        roomLayout.AddFloat(3);

        roomVa.AddBuffer(roomVb, roomLayout);

        Room room(roomShader, wWidth, wHeight);
        roomShader.Unbind();


        // HOUSE



        float cubeWithWindowVertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

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
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, -1.0f,


        };

        stride = (3 + 3) * sizeof(float);

        houseShader.Bind();
        VertexArray houseVa;
        VertexBuffer houseVb(cubeWithWindowVertices, 42 * stride);
         //IndexBuffer ib(indices, 6);

        unsigned int withoutWindowIndices[] = {
            0, 1, 2, 3, 4, 5,
            6,7,8, 9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18,19,20,21,22,23,
            24,25,26,27,28,29,
            30,31,32,33,34,35
        };
        IndexBuffer withoutWindowIndicesIb(withoutWindowIndices, 36);

        unsigned int windowIndices[] = {
            36, 37, 38, 39, 40,41
        };
        IndexBuffer windowIndicesIb(windowIndices, 6);

        VertexBufferLayout houseLayout;
        // location 0
        houseLayout.AddFloat(3);
        //location 1
        houseLayout.AddFloat(3);

        houseVa.AddBuffer(houseVb, houseLayout);


        House house(houseShader, wWidth, wHeight);
        houseShader.Unbind();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        GLCall(glEnable(GL_DEPTH_TEST));
        //glDepthFunc(GL_LESS);
        //GLCall(glEnable(GL_CULL_FACE));
       // glCullFace(GL_BACK);
        GLCall(glClearColor(255.0, 255.0, 255.0, 1.0));
        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                room.setModel(glm::rotate(room.getModel(), glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
                house.setModel(glm::rotate(house.getModel(), glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                room.setModel(glm::rotate(room.getModel(), glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
                house.setModel(glm::rotate(house.getModel(), glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
            }
            roomShader.SetUniform4f("u_Color", 105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f);
            renderer.Draw(roomVa, ib, roomShader);

            house.SetMatrices();
            houseShader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
            renderer.Draw(houseVa, withoutWindowIndicesIb, houseShader);

            house.SetMatrices();
            houseShader.SetUniform4f("u_Color", 167.0f/255, 199.0f/255, 203.0f/255, 0.5f);
            renderer.Draw(houseVa, windowIndicesIb, houseShader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
}
