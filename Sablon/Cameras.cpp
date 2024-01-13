#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Cameras.h"


Cameras::Cameras(Shader& shader, glm::mat4 view, glm::mat4 projection, std::vector<Camera> cameras)
    : Renderable(shader)
{

    //this->model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    shader.Bind();

    // spotlight 1
    shader.SetUniform1f("uSpotlight1.constant", 2.1f);
    shader.SetUniform1f("uSpotlight1.linear", 0.0f);
    shader.SetUniform1f("uSpotlight1.quadratic", 0.0f);

    shader.SetUniform3f("uSpotlight1.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uSpotlight1.kD", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform3f("uSpotlight1.kS", glm::vec3(0.1f, 0.1f, 0.1f));

    // spotlight 2
    shader.SetUniform1f("uSpotlight2.constant", 2.1f);
    shader.SetUniform1f("uSpotlight2.linear", 0.0f);
    shader.SetUniform1f("uSpotlight2.quadratic", 0.0f);

    shader.SetUniform3f("uSpotlight2.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uSpotlight2.kD", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetUniform3f("uSpotlight2.kS", glm::vec3(0.1f, 0.1f, 0.1f));

    shader.SetUniform1f("uMaterial.shine", 52);
    shader.SetUniform3f("uMaterial.kA", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uMaterial.kD", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniform3f("uMaterial.kS", glm::vec3(0.8f, 0.8f, 0.8f));


    for (int i = 0; i < 4; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        if(i < 2)
            model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        else
            model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
     
        //float xOffset = (i % 3) * 1.35f - 1.25f;
       // float yOffset = i > 2 ? 0.25f : 1.5f;
        glm::vec3 biggerTranslation = glm::vec3(cameras[i].position[0], cameras[i].position[1], cameras[i].position[2]);
        model = glm::translate(model, biggerTranslation);
        model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
        model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
    
        //model = glm::translate(model, biggerTranslation);
     
        //glm::vec3(-1.0f, 1.25f, -1.45f)

        this->models.push_back(model);
    }
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.45f));

    //shader.SetUniformMat4f("uM", model);

    this->view = view;
    shader.SetUniformMat4f("uV", view);

    this->projection = projection;
    shader.SetUniformMat4f("uP", projection);


}
void Cameras::setCameras(float direction) {
    for (int i = 0; i < this->models.size(); ++i) {
        //glm::mat4 cameraModel = glm::translate(glm::mat4(1.0f), -position);
        //cameraModel = glm::translate(cameraModel, position);
        if(i < 2)
            this->models[i] = glm::rotate(this->models[i], glm::radians(-direction), glm::vec3(0.0f, 0.0f, 1.0f));
        else
            this->models[i] = glm::rotate(this->models[i], glm::radians(direction), glm::vec3(0.0f, 0.0f, 1.0f));
        //position = cameraModel * glm::vec4(position, 1.0);
        //look = glm::vec3(model * glm::vec4(look, 1.0));
       // up = glm::vec3(model * glm::vec4(up, 1.0));
    }

}
std::vector<glm::mat4> Cameras::getModels()
{
    return models;
}
