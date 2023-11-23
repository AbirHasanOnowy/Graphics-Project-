//
//  sphere.h
//  test
//
//  Created by Nazirul Hasan on 26/9/23.
//

#ifndef sphere_h
#define sphere_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

class Sphere
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess;

    // ctor/dtor
    Sphere(unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax, float radius = 0.5f, int sectorCount = 48, int stackCount = 18, glm::vec3 amb = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f), float shiny = 32.0f) : verticesStride(24)
    {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny, dMap, sMap, textureXmin, textureYmin, textureXmax, textureYmax);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &sphereTexVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);

        glBindVertexArray(sphereTexVAO);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        int stride = this->getVerticesStride();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // unbind VAO and VBOs
        /*glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
    }
    ~Sphere() {}

    // getters/setters

    void set(float radius, int sectorCount, int stackCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        if (radius > 0)
            this->radius = radius;
        this->sectorCount = sectorCount;
        if (sectorCount < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stackCount;
        if (stackCount < MIN_STACK_COUNT)
            this->stackCount = MIN_STACK_COUNT;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
        //this->emissive = emis;
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->TXmin = textureXmin;
        this->TXmax = textureXmax;
        this->TYmin = textureYmin;
        this->TYmax = textureYmax;
    }

    void setRadius(float radius)
    {
        if (radius != this->radius)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    }

    void setSectorCount(int sectors)
    {
        if (sectors != this->sectorCount)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    }

    void setStackCount(int stacks)
    {
        if (stacks != this->stackCount)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    }

    // for interleaved vertices
    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;     // # of vertices
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);  // # of bytes
    }

    int getVerticesStride() const
    {
        return verticesStride;   // should be 24 bytes
    }
    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

    // draw in VertexArray mode
    void drawSphere(Shader& lightingShader, glm::mat4 model) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);


        lightingShader.setMat4("model", model);

        // draw a sphere with VAO
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }
    void drawSphere2(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f, float alpha = 0.5f) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
        lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
        lightingShader.setVec3("material.specular", glm::vec3(r, g, b));
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setVec4("color", glm::vec4(r, g, b, alpha));
        lightingShader.setMat4("model", model);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // draw a sphere with VAO
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices
        //
        // unbind VAO
        glBindVertexArray(0);
        glDisable(GL_BLEND);
    }

    void drawSphereWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f)) {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setVec3("material.ambient", this->ambient);
        lightingShaderWithTexture.setVec3("material.diffuse", this->diffuse);
        lightingShaderWithTexture.setVec3("material.specular", this->specular);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(sphereTexVAO);
        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0);
    }

private:
    // member functions
    void buildCoordinatesAndIndices()
    {
        float x, y, z, xz;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle = -sectorStep;
        float stackAngle = PI / 2 + stackStep;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle -= stackStep;        // starting from pi/2 to -pi/2
            xz = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);
            // add (sectorCount+1) vertices per stack
            // first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle += sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                z = xz * cosf(sectorAngle);
                x = xz * sinf(sectorAngle);
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                // vertex tex coord (s, t) range between [0, 1]
                /*s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back(s);
                texCoords.push_back(t);*/
                s = j * 0.05;//(1/20)
                //t = t;

                texCoords.push_back(s);
                texCoords.push_back(t);
            }
        }

        // generate index list of sphere triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                if (i != 0 && i != (stackCount - 1))
                {
                    // k1 => k2 => k1+1
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);

                    // k1+1 => k2 => k2+1
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
                // 2 triangles per sector excluding first and last stacks
                else if (i == 0)
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);

                }

                else if (i == (stackCount - 1))
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
            }
        }
    }

    void buildTextureCoordinates()
    {
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;

        for (int i = 0; i <= stackCount; ++i)
        {
            float stackAngle = PI / 2 - i * stackStep;

            for (int j = 0; j <= sectorCount; ++j)
            {
                float sectorAngle = j * sectorStep;

                float x = cosf(sectorAngle) * sinf(stackAngle);
                float y = cosf(stackAngle);
                float z = sinf(sectorAngle) * sinf(stackAngle);

                textureCoordinates.push_back(x);
                textureCoordinates.push_back(y);
            }
        }
    }

    void buildVertices()
    {
        size_t i, j;
        size_t count = coordinates.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(-1 * normals[i]);
            vertices.push_back(-1 * normals[i + 1]);
            vertices.push_back(-1 * normals[i + 2]);

            /*if (i < normals.size())
                vertices.push_back(normals[i]);
            if (i + 1 < normals.size())
                vertices.push_back(normals[i + 1]);
            if (i + 2 < normals.size())
                vertices.push_back(normals[i + 2]);*/


                // Add texture coordinates
            if (j < textureCoordinates.size())
                vertices.push_back(textureCoordinates[j]);
            if (j + 1 < textureCoordinates.size())
                vertices.push_back(textureCoordinates[j + 1]);
        }
    }

    vector<float> computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        const float EPSILON = 0.000001f;

        vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }

    // memeber vars
    unsigned int sphereTexVAO;
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    vector<float> textureCoordinates, texCoords;
    int verticesStride;                 // # of bytes to hop to the next vertex (should be 24 bytes)

};

#endif /* sphere.h */