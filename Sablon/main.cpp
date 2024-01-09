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
#include "Ramp.h"


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
    Shader rampShader("basic.vert", "basic.frag");

    {
        Renderer renderer;

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)wWidth / (float)wHeight, 0.1f, 10.0f);
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
        0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
         IndexBuffer ib(indices, 30);

        VertexBufferLayout roomLayout;
        // location 0
        roomLayout.AddFloat(3);
        //location 1
        roomLayout.AddFloat(3);

        roomVa.AddBuffer(roomVb, roomLayout);

        Room room(roomShader, wWidth, wHeight, view, projection);
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


        House house(houseShader, wWidth, wHeight, view, projection);
        houseShader.Unbind();

        // Ramp
        float rampVertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ
        -0.30f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.05f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.05f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.05f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.30f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.30f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.30f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.05f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.05f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.05f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.30f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.30f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.30f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.30f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.30f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.30f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.30f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.30f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.05f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.05f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.05f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.05f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.05f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.05f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.30f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.05f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.05f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.05f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.30f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.30f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.30f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.05f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.05f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.05f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.30f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.30f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

        //moveable 
        -0.45f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
         0.85f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
         0.85f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
         0.85f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
        -0.45f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,
        -0.45f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,

        -0.45f, -0.25f,  0.25f,  0.0f,  0.0f, 1.0f,
         0.85f, -0.25f,  0.25f,  0.0f,  0.0f, 1.0f,
         0.85f,  0.25f,  0.25f,  0.0f,  0.0f, 1.0f,
         0.85f,  0.25f,  0.25f,  0.0f,  0.0f, 1.0f,
        -0.45f,  0.25f,  0.25f,  0.0f,  0.0f, 1.0f,
        -0.45f, -0.25f,  0.25f,  0.0f,  0.0f, 1.0f,

        -0.45f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f,
        -0.45f,  0.25f, -0.25f, -1.0f,  0.0f,  0.0f,
        -0.45f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f,
        -0.45f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f,
        -0.45f, -0.25f,  0.25f, -1.0f,  0.0f,  0.0f,
        -0.45f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f,

         0.85f,  0.25f,  0.25f,  1.0f,  0.0f,  0.0f,
         0.85f,  0.25f, -0.25f,  1.0f,  0.0f,  0.0f,
         0.85f, -0.25f, -0.25f,  1.0f,  0.0f,  0.0f,
         0.85f, -0.25f, -0.25f,  1.0f,  0.0f,  0.0f,
         0.85f, -0.25f,  0.25f,  1.0f,  0.0f,  0.0f,
         0.85f,  0.25f,  0.25f,  1.0f,  0.0f,  0.0f,

        -0.45f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
         0.85f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,
         0.85f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,
         0.85f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,
        -0.45f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,
        -0.45f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,

        -0.45f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,
         0.85f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,
         0.85f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,
         0.85f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,
        -0.45f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,
        -0.45f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,
        };

        rampShader.Bind();
        VertexArray rampVa;
        VertexBuffer rampVb(rampVertices, 72 * stride);

        unsigned int unmoveableRampIndices[36];
        for (unsigned int i = 0; i < 36; ++i) {
            unmoveableRampIndices[i] = i;
        }
        IndexBuffer unmoveableRampIndicesIb(unmoveableRampIndices, 36);

        unsigned int moveableRampIndices[36];
        for (unsigned int i = 0; i < 36; ++i) {
            moveableRampIndices[i] = i + 36;
        }
        IndexBuffer moveableRampIndicesIb(moveableRampIndices, 36);

        VertexBufferLayout rampLayout;
        // location 0
        rampLayout.AddFloat(3);
        //location 1
        rampLayout.AddFloat(3);

        rampVa.AddBuffer(rampVb, rampLayout);

        Ramp ramp(rampShader, wWidth, wHeight, view, projection);
        ramp.rampAngle = 0.0f;
        float increment = 0.1f;
        bool rampEnabled = false;
        rampShader.Unbind();

        Renderable scene[] = { room, house, ramp };
        

        //ALPHA
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        // DEPTH TEST
        GLCall(glEnable(GL_DEPTH_TEST));
        glDepthFunc(GL_LESS);
        // CULLING
        //GLCall(glEnable(GL_CULL_FACE));
       // glCullFace(GL_BACK);
        // WIREFRAME MODE
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // THICKER LINE
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(5.0);
        GLCall(glClearColor(255.0, 255.0, 255.0, 1.0));

        const double targetFPS = 120.0;
        double lastTime = 0.0;
        while (!glfwWindowShouldClose(window))
        {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;

            double targetFrameTime = 1.0 / targetFPS;

            if (deltaTime < targetFrameTime) {
                double sleepTime = targetFrameTime - deltaTime;
                std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            }

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            //view = glm::rotate(view, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
           
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                view = glm::rotate(view, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                view = glm::rotate(view, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
            {
                // position inside house, look down, up is in direction of window
                Camera camera = house.getHouseCamera();
                view = glm::lookAt(camera.position, camera.look, camera.up);

            }

            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            {
                // position inside house, look down, up is in direction of window
                Camera camera = room.getCornerCameras()[0];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            {
                // position inside house, look down, up is in direction of window
                Camera camera = room.getCornerCameras()[1];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            {
                // position inside house, look down, up is in direction of window
                Camera camera = room.getCornerCameras()[2];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
            {
                // position inside house, look down, up is in direction of window
                Camera camera = room.getCornerCameras()[3];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }


            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            {
                rampEnabled = true;
            }
            for (int i = 0; i < 3; ++i) {
                scene[i].setView(view);
            }

            
          
            roomShader.SetUniform4f("u_Color", 105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f);
            renderer.Draw(roomVa, ib, roomShader);

            houseShader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
            renderer.Draw(houseVa, withoutWindowIndicesIb, houseShader);

            houseShader.SetUniform4f("u_Color", 167.0f/255, 199.0f/255, 203.0f/255, 0.5f);
            renderer.Draw(houseVa, windowIndicesIb, houseShader);

        
            rampShader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
            glm::mat4 unmovebleModel = ramp.getModel();
            renderer.Draw(rampVa, unmoveableRampIndicesIb, rampShader);

          
            if (rampEnabled) {
                if (ramp.rampAngle >= 80.0f)
                    increment = -(80.0f / targetFPS);
                else if (ramp.rampAngle <= 0.0f) {
                    increment = 80.0f / targetFPS;
                    rampEnabled = false;
                }
                    
                ramp.rampAngle += increment;
                rampShader.SetUniformMat4f("uM", ramp.rotate());
            }
           
          
           
            rampShader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f);
            renderer.Draw(rampVa, moveableRampIndicesIb, rampShader);
            ramp.setModel(unmovebleModel);

            lastTime = glfwGetTime();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
}
