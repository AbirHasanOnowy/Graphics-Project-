//
//  cube.h
//  test
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef octagon_h
#define octagon_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Octagon {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Octagon()
    {
        setUpOctagonVertexDataAndConfigureVertexAttribute();
    }

    Octagon(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpOctagonVertexDataAndConfigureVertexAttribute();
    }

    Octagon(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpOctagonVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Octagon()
    {
        glDeleteVertexArrays(1, &octagonVAO);
        glDeleteVertexArrays(1, &lightOctagonVAO);
        glDeleteVertexArrays(1, &lightTexOctagonVAO);
        glDeleteBuffers(1, &octagonVBO);
        glDeleteBuffers(1, &octagonEBO);
    }

    void drawOctagonWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexOctagonVAO);
        glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);
    }

    void drawOctagonWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();
        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightOctagonVAO);
        glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);
    }

    void drawOctagon(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient",glm::vec3(r, g, b));
        lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
        lightingShader.setVec3("material.specular", glm::vec3(r, g, b));
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setMat4("model", model);

        glBindVertexArray(octagonVAO);
        glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int octagonVAO;
    unsigned int lightOctagonVAO;
    unsigned int lightTexOctagonVAO;
    unsigned int octagonVBO;
    unsigned int octagonEBO;

    void setUpOctagonVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float octagon_vertices[] = {

            // positions                  // normals         // texture

            //top surface
            0.70711f,     1.0f,     0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmin, //AP 0
            1.70711f,     1.0f,     0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmin, //BP 1
            2.41422f,     1.0f, 0.70711f, 0.0f, 1.0f, 0.0f, TXmin, TYmax, //CP 2
            2.41422f,     1.0f, 1.70711f, 0.0f, 1.0f, 0.0f, TXmax, TYmax, //DP 3
            1.70711f,     1.0f, 2.41422f, 0.0f, 1.0f, 0.0f, TXmax, TYmax, //EP 4
            0.70711f,     1.0f, 2.41422f, 0.0f, 1.0f, 0.0f, TXmax, TYmax, //FP 5
                0.0f,     1.0f, 1.70711f, 0.0f, 1.0f, 0.0f, TXmax, TYmax, //GP 6
                0.0f,     1.0f, 0.70711f, 0.0f, 1.0f, 0.0f, TXmax, TYmax, //HP 7

            //bottom surface
            0.70711f,     0.0f,     0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin, //A 8
            1.70711f,     0.0f,     0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin, //B 9
            2.41422f,     0.0f, 0.70711f, 0.0f, -1.0f, 0.0f, TXmin, TYmax, //C 10
            2.41422f,     0.0f, 1.70711f, 0.0f, -1.0f, 0.0f, TXmax, TYmax, //D 11
            1.70711f,     0.0f, 2.41422f, 0.0f, -1.0f, 0.0f, TXmax, TYmax, //E 12
            0.70711f,     0.0f, 2.41422f, 0.0f, -1.0f, 0.0f, TXmax, TYmax, //F 13
                0.0f,     0.0f, 1.70711f, 0.0f, -1.0f, 0.0f, TXmax, TYmax, //G 14
                0.0f,     0.0f, 0.70711f, 0.0f, -1.0f, 0.0f, TXmax, TYmax, //H 15


            //BACK A B BP AP
            0.70711f,     0.0f,     0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin, //A 16
            0.70711f,     1.0f,     0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin, //AP 17
            1.70711f,     1.0f,     0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin, //BP 18
            1.70711f,     0.0f,     0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin, //B 19

            //RIGHT D C CP DP
            2.41422f,     0.0f, 0.70711f, 1.0f, 0.0f, 0.0f, TXmin, TYmax, //C 20
            2.41422f,     1.0f, 0.70711f, 1.0f, 0.0f, 0.0f, TXmin, TYmax, //CP 21
            2.41422f,     1.0f, 1.70711f, 1.0f, 0.0f, 0.0f, TXmax, TYmax, //DP 22
            2.41422f,     0.0f, 1.70711f, 1.0f, 0.0f, 0.0f, TXmax, TYmax, //D 23

            //FRONT E F FP EP
            0.70711f,     0.0f, 2.41422f, 0.0f, 0.0f, 1.0f, TXmax, TYmax, //F 24
            1.70711f,     0.0f, 2.41422f, 0.0f, 0.0f, 1.0f, TXmax, TYmax, //E 25
            1.70711f,     1.0f, 2.41422f, 0.0f, 0.0f, 1.0f, TXmax, TYmax, //EP 26
            0.70711f,     1.0f, 2.41422f, 0.0f, 0.0f, 1.0f, TXmax, TYmax, //FP 27

            //LEFT G H HP GP
                0.0f,     0.0f, 1.70711f, -1.0f, 0.0f, 0.0f, TXmax, TYmax, //G 28
                0.0f,     1.0f, 1.70711f, -1.0f, 0.0f, 0.0f, TXmax, TYmax, //GP 29
                0.0f,     1.0f, 0.70711f, -1.0f, 0.0f, 0.0f, TXmax, TYmax, //HP 30
                0.0f,     0.0f, 0.70711f, -1.0f, 0.0f, 0.0f, TXmax, TYmax, //H 31

            //BACK RIGHT B C CP BP
            1.70711f,     0.0f,     0.0f, 0.707107f, 0.0f, -0.707107f, TXmin, TYmin, //B 32
            1.70711f,     1.0f,     0.0f, 0.707107f, 0.0f, -0.707107f, TXmin, TYmin, //BP 33
            2.41422f,     1.0f, 0.70711f, 0.707107f, 0.0f, -0.707107f, TXmin, TYmax, //CP 34
            2.41422f,     0.0f, 0.70711f, 0.707107f, 0.0f, -0.707107f, TXmin, TYmax, //C 35

            //RIGHT FRONT D E EP DP
            2.41422f,     0.0f, 1.70711f, 0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //D 36
            2.41422f,     1.0f, 1.70711f, 0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //DP 37
            1.70711f,     1.0f, 2.41422f, 0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //EP 38
            1.70711f,     0.0f, 2.41422f, 0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //E 39

            //FRONT LEFT F G GP FP
            0.70711f,     0.0f, 2.41422f, -0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //F 40
            0.70711f,     1.0f, 2.41422f, -0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //FP 41
                0.0f,     1.0f, 1.70711f, -0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //GP 42
                0.0f,     0.0f, 1.70711f, -0.707107f, 0.0f, 0.707107f, TXmax, TYmax, //G 43

            //LEFT BACK H A AP HP
                0.0f,     0.0f, 0.70711f, -0.707107f, 0.0f, -0.707107f, TXmax, TYmax, //H 44
                0.0f,     1.0f, 0.70711f, -0.707107f, 0.0f, -0.707107f, TXmax, TYmax, //HP 45
            0.70711f,     1.0f,     0.0f, -0.707107f, 0.0f, -0.707107f, TXmax, TYmin, //AP 46
            0.70711f,     0.0f,     0.0f, -0.707107f, 0.0f, -0.707107f, TXmax, TYmin, //A 47

        };
        unsigned int octagon_indices[] = {
            
            //TOP OCTAGON
            0, 2, 1,
            0, 3, 2,
            0, 4, 3,
            0, 5, 4,
            0, 6, 5,
            0, 7, 6,

            //BOTTOM OCTAGON
            8, 9, 10,
            8, 10, 11,
            8, 11, 12,
            8, 12, 13,
            8, 13, 14,
            8, 14, 15,

            //BACK
            16, 17, 18, //A AP BP
            16, 18, 19, //A BP B

            //RIGHT
            20, 21, 22, //C CP DP
            20, 22, 23, //C DP D

            //FRONT
            24, 25, 26, //F E EP 
            24, 26, 27, //F EP FP

            //LEFT
            28, 29, 30, //G GP HP
            28, 30, 31, //G HP H

            //BACK RIGHT
            32, 33, 34, //B BP CP
            32, 34, 35, //B CP C

            //RIGHT FRONT
            36, 37, 38, //D DP EP
            36, 38, 39, //D EP E

            //FRONT LEFT
            40, 41, 42, //F FP GP
            40, 42, 43, //F GP G

            //LEFT BACK
            44, 45, 46, //H HP AP
            44, 46, 47 //H AP A
        };

        glGenVertexArrays(1, &octagonVAO);
        glGenVertexArrays(1, &lightOctagonVAO);
        glGenVertexArrays(1, &lightTexOctagonVAO);
        glGenBuffers(1, &octagonVBO);
        glGenBuffers(1, &octagonEBO);


        glBindVertexArray(lightTexOctagonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, octagonVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(octagon_vertices), octagon_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octagonEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(octagon_indices), octagon_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightOctagonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, octagonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octagonEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(octagonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, octagonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octagonEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


#endif /* cube_h */
