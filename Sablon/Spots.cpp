#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Spots.h"


Spots::Spots(Shader& shader, glm::mat4 view, glm::mat4 projection)
    : Renderable(shader)
{

    this->model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f));

    for (int i = 0; i < 6; ++i) {
        float xOffset = (i % 3) * 1.35f - 1.25f;
        float yOffset = i > 2 ? 0.25f : 1.5f;

      
            //glm::vec3(-1.0f, 1.25f, -1.45f)
        
        this->models.push_back(glm::translate(model, glm::vec3(xOffset, yOffset+0.25, -1.45f)));
    }
    //model = glm::translate(model, glm::vec3(-1.0f, 1.25f, -1.45f));

    shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);

   
}

std::vector<glm::mat4> Spots::getModels()
{
    return models;
}
