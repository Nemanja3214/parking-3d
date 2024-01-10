#include "Room.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


Room::Room(Shader& shader, glm::mat4 view, glm::mat4 projection)
    :ceiling(true), Renderable(shader)
{
    this->model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 2.5f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

    glm::vec4 cameraPos1 = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec4 cameraPos2 = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec4 cameraPos3 = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
    glm::vec4 cameraPos4 = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
    std::vector<glm::vec4> camerasPositions = { cameraPos1, cameraPos2, cameraPos3, cameraPos4 };
    for (glm::vec4 position : camerasPositions) {
        position = model * position;
    }

    glm::vec4 cameraLook1 = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
    glm::vec4 cameraLook2 = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
    glm::vec4 cameraLook3 = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
    glm::vec4 cameraLook4 = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
    std::vector<glm::vec4> camerasLooks= { cameraLook1, cameraLook2, cameraLook3, cameraLook4 };
    for (glm::vec4 look : camerasLooks) {
        look = model * look;
    }

    glm::vec4 cameraUp1 = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
    glm::vec4 cameraUp2 = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
    glm::vec4 cameraUp3 = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 cameraUp4 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    std::vector<glm::vec4> camerasUps= { cameraUp1, cameraUp2, cameraUp3, cameraUp4 };
    for (glm::vec4 up : camerasUps) {
        up = model * up;
    }
    for (int i = 0; i < 4; ++i) {
        this->cornerCameras.push_back({ camerasPositions[i], camerasLooks[i], camerasUps[i] });
    }

   birdCamera = { glm::vec3(model * glm::vec4(0.0f, 1.5f, 0.0f, 1.0f)), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
}

std::vector<Camera> Room::getCornerCameras()
{

    return this->cornerCameras;
}


bool Room::isCeiling()
{
    return ceiling;
}


void Room::setCeiling(bool ceiling)
{
    this->ceiling = ceiling;
}

Camera Room::getBirdCamera()
{
    return birdCamera;
}


