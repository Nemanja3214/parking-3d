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

#include <string>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Room.h"
#include "Renderer.h"
#include "House.h"
#include "Ramp.h"
#include "model.hpp"
#include "Man.h"
#include "Spots.h"
#include "Car.h"
#include "Index.h"
#include "Monitor.h"
#include "Cameras.h"

struct ParkingSpot {
    double startTime;
    double endTime;
    glm::vec4 color;
    bool taken;
    bool numberVisible;
};

void takePlace(ParkingSpot spots[], int i, double now);
void freePlace(ParkingSpot spots[], int i, double now);
void resetPlace(ParkingSpot spots[], int i, double now);
glm::mat4 transform(float xOffset, float yOffset, float scale);
ParkingSpot parkingSpots[6];


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
    Shader roomShader("texture.vert", "texture.frag");
    Shader camerasShader("texture.vert", "phongSpotlight.frag");
    Shader indexShader("texture.vert", "texture.frag");
    Shader houseShader("texture.vert", "phong.frag");
    Shader monitorShader("texture.vert", "phong.frag");
    Shader rampShader("texture.vert", "phongSpotlight.frag");
    Shader manShader("model.vert", "phong.frag");
    Shader carShader("model.vert", "phongSpotlight.frag");
    Shader spotShader("texture.vert", "phongSpotlight.frag");

    Shader monitorSpotShader("basic.vert", "basic.frag");
    Shader monitorCarShader("basic.vert", "basic.frag");
    Shader monitorProgressOutlineShader("basic.vert", "basic.frag");
    Shader monitorNumberShader("texture.vert", "texture.frag");
    Shader monitorSemaphoreShader("basic.vert", "basic.frag");
    Shader monitorProgressShader("progress.vert", "basic.frag");

    {
        Renderer renderer;

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 perspective = glm::perspective(glm::radians(70.0f), (float)wWidth / (float)wHeight, 0.1f, 10.0f);
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)wWidth / (float)wHeight, 0.1f, 10.0f);
        glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);

        GLCall(glEnable(GL_TEXTURE_2D));
        GLCall(glEnable(GL_TEXTURE_CUBE_MAP_EXT));
        unsigned int texture[6];
        GLCall(glGenTextures(3, texture));

        glActiveTexture(GL_TEXTURE0);
        GLCall(glBindTexture(GL_TEXTURE_2D, texture[0]));

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load("Textures/concrete.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            GLint InternalFormat = -1;
            switch (nrChannels) {
            case 1: InternalFormat = GL_RED; break;
            case 3: InternalFormat = GL_RGB; break;
            case 4: InternalFormat = GL_RGBA; break;
            default: InternalFormat = GL_RGB; break;
            }

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, InternalFormat, GL_UNSIGNED_BYTE, data));
            // set the texture wrapping/filtering options (on the currently bound texture object)
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
            //clamp to edge maybe
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            // load and generate the texture
            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

      

        // SECOND TEXTURE----------------------
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture[1]);

        for (unsigned int i = 0; i < 6; i++)
        {
            unsigned char* data = stbi_load("Textures/cpy.png", &width, &height, &nrChannels, 0);
            if (data)
            {
                GLint InternalFormat = -1;
                switch (nrChannels) {
                case 1: InternalFormat = GL_RED; break;
                case 3: InternalFormat = GL_RGB; break;
                case 4: InternalFormat = GL_RGBA; break;
                default: InternalFormat = GL_RGB; break;
                }
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, InternalFormat, width, height, 0, InternalFormat, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << "Textures/cpy.png" << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                //GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        // THIRD TEXTURE----------------------
        glActiveTexture(GL_TEXTURE2);
        GLCall(glBindTexture(GL_TEXTURE_2D, texture[2]));

        data = stbi_load("Textures/index.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            GLint InternalFormat = -1;
            switch (nrChannels) {
            case 1: InternalFormat = GL_RED; break;
            case 3: InternalFormat = GL_RGB; break;
            case 4: InternalFormat = GL_RGBA; break;
            default: InternalFormat = GL_RGB; break;
            }

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, InternalFormat, GL_UNSIGNED_BYTE, data));
            // set the texture wrapping/filtering options (on the currently bound texture object)
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
            //clamp to edge maybe
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            // load and generate the texture
            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        unsigned int numberTexture[6];
        glGenTextures(6, numberTexture);
        for (int i = 0; i < 6; ++i) {

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, numberTexture[i]);
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //clamp to edge maybe
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load and generate the texture
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            std::string si = "Textures/" + std::to_string(i + 1) + ".png";
            unsigned char* data = stbi_load(si.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        float rectangleVertices[] = {
            -1.0f, -1.0f, 0.0f,   0.0f,  0.0f, 1.0f,    0.0f,   0.0f,   0.0f, 0.0f,
            1.0f, -1.0f,0.0f,     0.0f,  0.0f, 1.0f,    1.0f,   0.0f,   1.0f, 0.0f,
            -1.0f, 1.0f,0.0f,     0.0f,  0.0f, 1.0f,    0.0f,   1.0f,   0.0f, 1.0f,
            1.0f, 1.0f,0.0f,      0.0f,  0.0f, 1.0f,    1.0f,   1.0f,   1.0f, 1.0f,
        };
        unsigned int stride = (3 + 3 + 2 + 2) * sizeof(float);
        indexShader.Bind();
        VertexArray indexVa;
        VertexBuffer indexVb(rectangleVertices, 4 * stride);

        unsigned int indexIndices[] = {
        0, 1, 2, 1, 3, 2};
        IndexBuffer indexIndicesIb(indexIndices, 6);


        VertexBufferLayout indexLayout;
        // location 0
        indexLayout.AddFloat(3);
        indexLayout.AddFloat(3);
        indexLayout.AddFloat(2);
        indexLayout.AddFloat(2);

        indexVa.AddBuffer(indexVb, indexLayout);

        Index index(indexShader, view, projection);

        stride = (3 + 3 + 2 + 2) * sizeof(float);
        monitorCarShader.Bind();
        VertexArray monitorCarVa;
        VertexBuffer monitorCarVb(rectangleVertices, 4 * stride);

        unsigned int monitorCarIndices[] = {
        0, 1, 2, 1, 3, 2 };
        IndexBuffer monitorCarIndicesIb(monitorCarIndices, 6);


        VertexBufferLayout monitorCarLayout;
        // location 0
        monitorCarLayout.AddFloat(3);
        monitorCarLayout.AddFloat(3);
        monitorCarLayout.AddFloat(2);
        monitorCarLayout.AddFloat(2);

        monitorCarVa.AddBuffer(monitorCarVb, monitorCarLayout);
        monitorCarShader.Unbind();

        stride = (3 + 3 + 2 + 2) * sizeof(float);
        monitorSpotShader.Bind();
        VertexArray monitorSpotVa;
        VertexBuffer monitorSpotVb(rectangleVertices, 4 * stride);

        unsigned int monitorSpotIndices[] = {
        0,1, 3,2, 0};
        IndexBuffer monitorSpotIndicesIb(monitorSpotIndices, 5);


        VertexBufferLayout monitorSpotLayout;
        // location 0
        monitorSpotLayout.AddFloat(3);
        monitorSpotLayout.AddFloat(3);
        monitorSpotLayout.AddFloat(2);
        monitorSpotLayout.AddFloat(2);

        monitorSpotVa.AddBuffer(monitorSpotVb, monitorSpotLayout);
        monitorSpotShader.Unbind();

        float circle[CRES * 3 + 6]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
        float r = 0.2;

        circle[0] = 0; //center x
        circle[1] = 0; //center y
        circle[2] = 0; //center z
        int i;
        for (i = 0; i <= CRES; i++)
        {

            circle[3 + 3 * i] = circle[0] + r * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
            circle[3 + 3 * i + 1] = circle[1] + r * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
            circle[3 + 3 * i + 2] = 0.0f; //Zi
        }
        stride = (3) * sizeof(float);
        monitorSemaphoreShader.Bind();
        VertexArray monitorSemaphoreVa;
        VertexBuffer monitorSemaphoreVb(circle, ((CRES * 3 + 6) / 3) * stride);

        VertexBufferLayout monitorSemaphoreLayout;
        // location 0
        monitorSemaphoreLayout.AddFloat(3);

        monitorSemaphoreVa.AddBuffer(monitorSemaphoreVb, monitorSemaphoreLayout);
        monitorSemaphoreShader.Unbind();


        float cubeVertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ        TEX1          TEX2
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,   0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 100.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 100.0f, 100.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 100.0f, 100.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,   100.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,   0.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 100.0f, 0.0f,   10.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 100.0f, 100.0f, 10.0f, 10.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 100.0f, 100.0f, 10.0f, 10.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 100.0f,   0.0f, 10.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 100.0f,    0.0f, 10.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 100.0f,    0.0f, 10.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 100.0f,    0.0f, 10.0f,

         0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 100.0f, 100.0f, 10.0f, 10.0f,
         0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 100.0f, 100.0f, 10.0f, 10.0f,
         0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 100.0f, 0.0f,   10.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 100.0f, 0.0f,   10.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 100.0f, 0.0f,   10.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 100.0f, 100.0f, 10.0f, 10.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,       0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 100.0f, 0.0f,     10.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 100.0f, 0.0f,     10.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 100.0f, 0.0f,     10.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,       0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,       0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 100.0f,   0.0f, 10.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 100.0f, 100.0f, 10.0f, 10.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 100.0f, 100.0f, 10.0f, 10.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 100.0f, 100.0f, 10.0f, 10.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 100.0f,   0.0f, 10.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 100.0f,   0.0f, 10.0f,
        };
        stride = (3 + 3 + 2 + 2) * sizeof(float);


        // ROOM
        roomShader.Bind();
        VertexArray roomVa;
        VertexBuffer roomVb(cubeVertices, 36 * stride);

        unsigned int indices[] = {
        0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
         IndexBuffer ib(indices, 30);

         unsigned int noCeilingIndices[] = {
       0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };
         IndexBuffer noCeilingIb(noCeilingIndices, 30);

        VertexBufferLayout roomLayout;
        // location 0
        roomLayout.AddFloat(3);
        //location 1
        roomLayout.AddFloat(3);

        roomLayout.AddFloat(2);

        roomLayout.AddFloat(2);

        roomVa.AddBuffer(roomVb, roomLayout);

        Room room(roomShader, view, projection);

        float cameraIncrement = 0.3f;
        roomShader.Unbind();


        // HOUSE
        float cubeWithWindowVertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.0f, -0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, // left of window
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // right of window
         0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, // bottom
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, // top
         0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,

        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, 0.0f, -0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, 0.0f, -0.5f,  0.0f,  0.0f, 1.0f,

        // monitor

        -0.45f,  0.45f,  0.45f, -1.0f,  0.0f,  0.0f,
        -0.45f,  0.45f, -0.45f, -1.0f,  0.0f,  0.0f,
        -0.45f, -0.45f, -0.45f, -1.0f,  0.0f,  0.0f,
        -0.45f, -0.45f, -0.45f, -1.0f,  0.0f,  0.0f,
        -0.45f, -0.45f,  0.45f, -1.0f,  0.0f,  0.0f,
        -0.45f,  0.45f,  0.45f, -1.0f,  0.0f,  0.0f,
        };

        stride = (3 + 3) * sizeof(float);

        houseShader.Bind();
        VertexArray houseVa;
        VertexBuffer houseVb(cubeWithWindowVertices, 48 * stride);
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

        unsigned int monitorIndices[] = {
           42, 43, 44, 45, 46, 47
        };
        IndexBuffer monitorIndicesIb(monitorIndices, 6);

        VertexBufferLayout houseLayout;
        // location 0
        houseLayout.AddFloat(3);
        //location 1
        houseLayout.AddFloat(3);

        houseVa.AddBuffer(houseVb, houseLayout);


        House house(houseShader, view, projection);
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

        /////////////////
        camerasShader.Bind();
        VertexArray camerasVa;
        VertexBuffer camerasVb(rampVertices, 36 * stride);

        unsigned int camerasIndices[36];
        for (unsigned int i = 0; i < 36; ++i) {
            camerasIndices[i] = i;
        }
        IndexBuffer camerasIndicesIb(camerasIndices, 36);

        VertexBufferLayout camerasLayout;
        // location 0
        camerasLayout.AddFloat(3);
        //location 1
        camerasLayout.AddFloat(3);

        camerasVa.AddBuffer(camerasVb, camerasLayout);

        ///////////////

        Ramp ramp(rampShader, view, projection);
        ramp.rampAngle = 0.0f;
        float increment = 0.1f;
        bool rampEnabled = false;
        rampShader.Unbind();

        Model manModel("Models/man/FabConvert.com_uploads_files_1939375_casual_male.obj");
        Man man(manShader, view, projection);

        Model carModel("Models/carNew/LowPolyCars.obj");
        Car car(carShader, view, projection);

        Monitor monitor(monitorShader, view, projection);

        spotShader.Bind();
        float spotVertices[] =
        {   //Kocka
            //Normale su potrebne za racun osvjetljenja.
        //X     Y      Z       NX    NY     NZ
        -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,

        //inner part
         -0.25f, -0.25f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.25f, -0.25f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.25f,  0.25f,0.0f, 0.0f,  0.0f,  1.0f,
        -0.25f,  0.25f, 0.0f,  0.0f,  0.0f,  1.0f,

        };
        stride = (3 + 3) * sizeof(float);
        VertexArray spotVa;
        VertexBuffer spotVb(spotVertices, 8 * stride);

        unsigned int spotIndices[] = // indexes
        {
           0,4,1,
           4,5, 1,

           1,5,2,
           5,6,2,

           2, 6, 3,
           6, 7, 3,

           3, 7, 4,
           3, 4, 0
        };
        IndexBuffer spotIndicesIb(spotIndices, 24);

        VertexBufferLayout spotLayout;
        // location 0
        spotLayout.AddFloat(3);

        // location 1
        spotLayout.AddFloat(3);

        spotVa.AddBuffer(spotVb, spotLayout);
    

        Spots spots(spotShader, view, projection);
        spotShader.Unbind();

        Cameras cameras(camerasShader, view, projection, room.getCornerCameras());

        Renderable scene[] = { room, house, ramp, man, spots, car, monitor, cameras};
        

        //ALPHA
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        // DEPTH TEST
        GLCall(glEnable(GL_DEPTH_TEST));
        glDepthFunc(GL_LESS);
        // CULLING
        GLCall(glEnable(GL_CULL_FACE));
        glCullFace(GL_BACK);

        bool isPolygon = false;

        // THICKER LINE
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(5.0);
        GLCall(glClearColor(255.0, 255.0, 255.0, 1.0));

        const double targetFPS = 120.0;
        double lastTime = 0.0;

        srand(time(0));

        bool isLighting = false;
        bool isLightbulbOn = true;

        glm::mat4 transformM;
        glm::mat4 spotResults[6], semaphoreResults[6],
            progressResults[6], numberResults[6],
            carResults[6],
            backgroundResult, indexResult;
        float const scale = 0.1f;
        for (int i = 0; i < 6; ++i) {
            float xOffset = (i % 3) * 2.0f + 0.7f;
            float yOffset = i > 2 ? 4.7f : 0.5f;
            xOffset = xOffset / 30.0f;
            yOffset = yOffset / 30.0f;

            transformM = glm::mat4(1.0f);
            transformM = glm::rotate(transformM, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            transformM = glm::translate(transformM, glm::vec3(yOffset, xOffset, 0.0f));
            transformM = glm::translate(transformM, glm::vec3(-0.1f, -0.7f, 0.345f));
            transformM = glm::scale(transformM, glm::vec3(0.03f, 0.03f, 0.03f));
            transformM = glm::scale(transformM, glm::vec3(1.0f, 0.5f, 1.0f));
            spotResults[i] = transformM;

            carResults[i] = spotResults[i];

            transformM = spotResults[i];
            transformM = glm::scale(transformM, glm::vec3(1.0f, 2.0f, 1.0f));
            transformM = glm::translate(transformM, glm::vec3(0.0f, 1.0f, 0.0f));
            semaphoreResults[i] =  transformM;

            //transformM = transform(xOffset + 200, yOffset, 100.0f);
            transformM = spotResults[i];
            transformM = glm::rotate(transformM, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            transformM = glm::scale(transformM, glm::vec3(0.06f, 0.06f, 0.06f));
            transformM = glm::scale(transformM, glm::vec3(10.0f, 1.0f, 1.0f));
        
            transformM = glm::translate(transformM, glm::vec3(3.5f, -10.0f, 0.0f));
            progressResults[i] = transformM;

            transformM = spotResults[i];
            transformM = glm::rotate(transformM, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            transformM = glm::translate(transformM, glm::vec3(0.0f, 0.0f, -0.001f));
            //transformM = glm::scale(transformM, glm::vec3(0.6f, 0.6f, 0.6f));
            numberResults[i] =  transformM;

        }
        float progressWidth = 1.0, progressHeight = 0.1;
        float min_x = -progressWidth / 2.0, max_x = progressWidth / 2.0;
        float progress = 0;
        for (int i = 0; i < 6; ++i) {
            parkingSpots[i] = {
                0,
                0,
                glm::vec4(),
                false
            };
        }
        int numberKeys[6] = { GLFW_KEY_1 , GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6 };
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA16F, wWidth, wHeight);
        while (!glfwWindowShouldClose(window))
        {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;

            double targetFrameTime = 1.0 / targetFPS;

            if (deltaTime < targetFrameTime) {
                double sleepTime = targetFrameTime - deltaTime;
                std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            }

            for (int i = 0; i < 6; ++i) {

                parkingSpots[i].taken = parkingSpots[i].endTime > currentTime;
                int taken = parkingSpots[i].taken;
                //progress = taken ? (now - spots[i].startTime) / (spots[i].endTime - spots[i].startTime) : 0.0f;
            }

            for (int j = 0; j < 6; ++j) {
                if (glfwGetKey(window, numberKeys[j]) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                    freePlace(parkingSpots, j, currentTime);
                else if (glfwGetKey(window, numberKeys[j]) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                    resetPlace(parkingSpots, j, currentTime);
                else if (glfwGetKey(window, numberKeys[j]) == GLFW_PRESS)
                    takePlace(parkingSpots, j, currentTime);
                else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
                    parkingSpots[j].numberVisible = true;
                else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
                    parkingSpots[j].numberVisible = false;
            }

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);



            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

           
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                view = glm::rotate(view, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            {
                isPolygon = !isPolygon;
                if(isPolygon)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                view = glm::rotate(view, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            {
                isLighting = !isLighting;
                if (isLighting) {
                    houseShader.SetUniform1i("uIsLight", 1);
                    rampShader.SetUniform1i("uIsLight", 1);
                    roomShader.SetUniform1i("uIsLight", 1);
                    spotShader.SetUniform1i("uIsLight", 1);
                    carShader.SetUniform1i("uIsLight", 1);
                    manShader.SetUniform1i("uIsLight", 1);
                    camerasShader.SetUniform1i("uIsLight", 1);
                }
                else {
                    houseShader.SetUniform1i("uIsLight", 0);
                    rampShader.SetUniform1i("uIsLight", 0);
                    roomShader.SetUniform1i("uIsLight", 0);
                    spotShader.SetUniform1i("uIsLight", 0);
                    carShader.SetUniform1i("uIsLight", 0);
                    manShader.SetUniform1i("uIsLight", 0);
                    camerasShader.SetUniform1i("uIsLight", 0);
                }
            }

            if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
            {
                isLightbulbOn = !isLightbulbOn;
                if (isLightbulbOn) {
                    houseShader.SetUniform1i("uIsLightbulbOn", 1);
                    manShader.SetUniform1i("uIsLightbulbOn", 1);
                   // rampShader.SetUniform1i("uIsLightbulbOn", 1);
                   // spotShader.SetUniform1i("uIsLightbulbOn", 1);
                }
                else {
                    houseShader.SetUniform1i("uIsLightbulbOn", 0);
                    manShader.SetUniform1i("uIsLightbulbOn", 0);
                   // rampShader.SetUniform1i("uIsLightbulbOn", 0);
                    //spotShader.SetUniform1i("uIsLightbulbOn", 0);
                }
            }
            

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                view = glm::rotate(view, glm::radians(-0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                view = glm::rotate(view, glm::radians(0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
            }

            if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
            {
                room.currentCornerCameraIndex = -1;
                projection = perspective;
                room.setCeiling(true);
                // position inside house, look down, up is in direction of window
                Camera camera = house.getHouseCamera();
                view = glm::lookAt(camera.position, camera.look, camera.up);

            }

            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            {
                room.currentCornerCameraIndex = -1;
                projection = ortho;
                // position inside house, look down, up is in direction of window
                Camera camera = room.getBirdCamera();
                room.setCeiling(false);
                view = glm::lookAt(camera.position, camera.look, camera.up);

            }

            if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
            {
                room.currentCornerCameraIndex = 0;
                projection = perspective;
                room.setCeiling(true);
                Camera camera = room.getCornerCameras()[0];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
            {
                room.currentCornerCameraIndex = 1;
                projection = perspective;
                room.setCeiling(true);
                Camera camera = room.getCornerCameras()[1];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
            {
                room.currentCornerCameraIndex = 2;
                projection = perspective;
                room.setCeiling(true);
                Camera camera = room.getCornerCameras()[2];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
            {
                room.currentCornerCameraIndex = 3;
                projection = perspective;
                room.setCeiling(true);
                Camera camera = room.getCornerCameras()[3];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }


            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            {
                rampEnabled = true;
            }

            if (room.currentAngle >= 35.0f)
                cameraIncrement = -0.3f;
            else if (room.currentAngle <= -35.0f) {
                cameraIncrement = 0.3f;
            }
            room.currentAngle += cameraIncrement;
            room.setCameras(cameraIncrement);
            cameras.setCameras(cameraIncrement);

           
        

            carShader.Bind();
            carShader.SetUniform3f("uSpotlight1.pos", room.getCornerCameras()[3].position);
            carShader.SetUniform3f("uSpotlight1.lightDir", room.getCornerCameras()[3].look);

            carShader.SetUniform3f("uSpotlight2.pos", room.getCornerCameras()[1].position);
            carShader.SetUniform3f("uSpotlight2.lightDir", room.getCornerCameras()[1].look);


            rampShader.Bind();
            rampShader.SetUniform3f("uSpotlight1.pos", room.getCornerCameras()[3].position);
            rampShader.SetUniform3f("uSpotlight1.lightDir", room.getCornerCameras()[3].look);

            rampShader.SetUniform3f("uSpotlight2.pos", room.getCornerCameras()[1].position);
            rampShader.SetUniform3f("uSpotlight2.lightDir", room.getCornerCameras()[1].look);

            roomShader.Bind();
            roomShader.SetUniform3f("uSpotlight1.pos", room.getCornerCameras()[3].position);
            roomShader.SetUniform3f("uSpotlight1.lightDir", room.getCornerCameras()[3].look);

            roomShader.SetUniform3f("uSpotlight2.pos", room.getCornerCameras()[1].position);
            roomShader.SetUniform3f("uSpotlight2.lightDir", room.getCornerCameras()[1].look);

            spotShader.Bind();
            spotShader.SetUniform3f("uSpotlight1.pos", room.getCornerCameras()[3].position);
            spotShader.SetUniform3f("uSpotlight1.lightDir", room.getCornerCameras()[3].look);

            spotShader.SetUniform3f("uSpotlight2.pos", room.getCornerCameras()[1].position);
            spotShader.SetUniform3f("uSpotlight2.lightDir", room.getCornerCameras()[1].look);

            if (room.currentCornerCameraIndex != -1) {
                Camera camera = room.getCornerCameras()[room.currentCornerCameraIndex];
                view = glm::lookAt(camera.position, camera.look, camera.up);
            }

            for (int i = 0; i < 8; ++i) {
                scene[i].setView(view);
                scene[i].setProjection(projection);
            }
            //glDepthMask(GL_FALSE);
            roomShader.Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            roomShader.SetUniform1i("uDiffMap1", 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture[1]);
            roomShader.SetUniform1i("cubemap", 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            roomShader.SetUniform1i("emission", 2);

            roomShader.SetUniform4f("u_Color", 105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f);
            if (room.isCeiling())
                renderer.Draw(roomVa, ib, roomShader);
            else
                renderer.Draw(roomVa, noCeilingIb, roomShader);
            //glDepthMask(GL_TRUE);


        
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

            manShader.Bind();
            manShader.SetUniform4f("u_Color", 1.0f , 127.0f / 255, 80.0f / 255, 1.0f);
            manModel.Draw(manShader);

            spotShader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
            for (glm::mat4 model : spots.getModels()) {
                spotShader.SetUniformMat4f("uM", model);
                renderer.Draw(spotVa, spotIndicesIb, spotShader);
            }

            camerasShader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
            for (glm::mat4 model : cameras.getModels()) {
                camerasShader.SetUniformMat4f("uM", model);

                renderer.Draw(camerasVa, camerasIndicesIb, camerasShader);
            }

            for (int i = 0; i < 6; ++i) {

                parkingSpots[i].taken = parkingSpots[i].endTime > currentTime;
                int taken = parkingSpots[i].taken;
                if (taken) {
                    carShader.SetUniformMat4f("uM", car.getModels()[i]);
                    parkingSpots[i].color[3] = parkingSpots[i].numberVisible ? 0.5f : 1.0f;
                    carShader.SetUniform4fv("u_Color", parkingSpots[i].color);
                    carModel.Draw(carShader);
                }
            }



            houseShader.SetUniform4f("u_Color", 0.1f, 0.2f, 0.8f, 1.0f);
            renderer.Draw(houseVa, withoutWindowIndicesIb, houseShader);

            renderer.Draw(houseVa, monitorIndicesIb, monitorShader);

            monitorShader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
            for (int i = 0; i < 6; ++i) {

                parkingSpots[i].taken = parkingSpots[i].endTime > currentTime;
                int taken = parkingSpots[i].taken;
                progress = taken ? (currentTime - parkingSpots[i].startTime) / (parkingSpots[i].endTime - parkingSpots[i].startTime) : 0.0f;
                // setting uniforms
                //SPOT
                monitorSpotShader.SetUniform4fv("u_Color", glm::vec4(-1.0));
                monitorSpotShader.SetUniformMat4f("uM", spotResults[i]);
                monitorSpotShader.SetUniformMat4f("uV", view);
                monitorSpotShader.SetUniformMat4f("uP", projection);

                //CAR
                parkingSpots[i].color[3] = parkingSpots[i].numberVisible ? 0.5f : 1.0f;
                monitorCarShader.SetUniform4fv("u_Color", parkingSpots[i].color);
                monitorCarShader.SetUniformMat4f("uM", carResults[i]);
                monitorCarShader.SetUniformMat4f("uV", view);
                monitorCarShader.SetUniformMat4f("uP", projection);


                //NUMBER
                monitorNumberShader.SetUniformMat4f("uM", numberResults[i]);
                monitorNumberShader.SetUniformMat4f("uV", view);
                monitorNumberShader.SetUniformMat4f("uP", projection);

                //SEMAPHORE
                monitorSemaphoreShader.SetUniform4fv("u_Color", taken ? glm::vec4(1.0, 0.0, 0.0, 1.0) : glm::vec4(0.0, 1.0, 0.0, 1.0));
                monitorSemaphoreShader.SetUniformMat4f("uM", semaphoreResults[i]);
                monitorSemaphoreShader.SetUniformMat4f("uV", view);
                monitorSemaphoreShader.SetUniformMat4f("uP", projection);

                //PROGRESS
                monitorProgressShader.SetUniform1f("progress", progress);
                monitorProgressShader.SetUniform1f("minX", min_x);
                monitorProgressShader.SetUniform1f("width", progressWidth);

                monitorProgressShader.SetUniform4fv("u_Color", glm::vec4(0.0, 1.0, 0.0, 1.0));
                monitorProgressShader.SetUniformMat4f("uM", progressResults[i]);
                monitorProgressShader.SetUniformMat4f("uV", view);
                monitorProgressShader.SetUniformMat4f("uP", projection);

                //PROGRESS OUTLINE
                monitorProgressOutlineShader.SetUniform4fv("u_Color", glm::vec4(0.0, 0.0, 0.0, 1.0));
                monitorProgressOutlineShader.SetUniformMat4f("uM", progressResults[i]);
                monitorProgressOutlineShader.SetUniformMat4f("uV", view);
                monitorProgressOutlineShader.SetUniformMat4f("uP", projection);


                monitorNumberShader.Bind();
                monitorNumberShader.SetUniform1i("uIsLight", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, numberTexture[i]);
                monitorNumberShader.SetUniform1i("uDiffMap1", 0);
                monitorNumberShader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);
                renderer.Draw(monitorCarVa, monitorCarIndicesIb, monitorNumberShader);
                //glBindVertexArray(VAO[3]);
                //glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(number) / (2 * sizeof(float)));


                glEnable(GL_LINE_SMOOTH);
                glLineWidth(5.0);
                renderer.DrawLineStrip(monitorSpotVa, monitorSpotIndicesIb, monitorSpotShader);
                glLineWidth(1.0);

                if (taken)renderer.Draw(monitorCarVa, monitorCarIndicesIb, monitorCarShader);

                renderer.Draw(monitorCarVa, monitorCarIndicesIb, monitorProgressShader);

                renderer.DrawFan(monitorSemaphoreVa,
                    0, sizeof(circle) / (2 * sizeof(float)), monitorSemaphoreShader);

                //progress bar outline
                renderer.DrawLineStrip(monitorSpotVa, monitorSpotIndicesIb, monitorProgressOutlineShader);

                progress += 0.00001;

            }
        
            
            houseShader.SetUniform4f("u_Color", 167.0f / 255, 199.0f / 255, 203.0f / 255, 0.5f);
            renderer.Draw(houseVa, windowIndicesIb, houseShader);

            roomShader.Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            roomShader.SetUniform1i("uDiffMap1", 0);
            indexShader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);
            glDisable(GL_DEPTH_TEST);
            renderer.Draw(indexVa, indexIndicesIb, indexShader);
            glEnable(GL_DEPTH_TEST);


            lastTime = glfwGetTime();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glDeleteTextures(6, texture);
        glfwTerminate();
        return 0;
    }
}
glm::mat4 transform(float xOffset, float yOffset, float scale) {
    glm::mat4 translateM = glm::translate(glm::mat4(1.0f), glm::vec3(xOffset, yOffset, 0.0f));
    glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 0.0f));
    glm::mat4 rotateM = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 modelM = translateM * rotateM * scaleM;
    return modelM;
}
void takePlace(ParkingSpot spots[], int i, double now) {
    if (!spots[i].taken)
        spots[i] = {
            now,
            now + 20,
            glm::vec4((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, spots[i].numberVisible ? 0.5f : 1.0f),
            true,
            spots[i].numberVisible
    };
}

void freePlace(ParkingSpot spots[], int i, double now) {
    if (spots[i].taken)
        spots[i] = {
            0,
            0,
            glm::vec4(),
            false,
            spots[i].numberVisible
    };
}

void resetPlace(ParkingSpot spots[], int i, double now) {
    if (spots[i].taken) {
        spots[i].startTime = now;
        spots[i].endTime = now + 20;
    }
}

