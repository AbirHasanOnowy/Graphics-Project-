#pragma once
//
//  directionLight.h
//

#ifndef directionLight_h
#define directionLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class DirectionLight {
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    int lightNumber;
    //bool directionLightOn;

    DirectionLight(float dirX, float dirY, float dirZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, int num) {

        direction = glm::vec3(dirX, dirY, dirZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        lightNumber = num;
    }
    void setUpDirectionalLight(Shader& lightingShader)
    {
        lightingShader.use();

        if (lightNumber == 6) {
            lightingShader.setVec3("directionLight[0].direction", direction);
            lightingShader.setVec3("directionLight[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("directionLight[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("directionLight[0].specular", specularOn * specular);
            lightingShader.setBool("dayLightOn", true);
        }

        if (lightNumber == 7) {
            lightingShader.setVec3("directionLight[1].direction", direction);
            lightingShader.setVec3("directionLight[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("directionLight[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("directionLight[1].specular", specularOn * specular);
            lightingShader.setBool("moonLightOn", true);
        }
    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* directionLight_h */