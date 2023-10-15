#include "ShapeGen.h"
#include "Vertex.h"
#include <glm/glm.hpp>

#include <cstring>

ShapeData ShapeGenerator::Triangle(){

    ShapeData toReturn;

    Vertex tri[] =
    {
        glm::vec3(0.02f, -0.02f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-0.02f, -0.02f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),   
        glm::vec3(0.0f,  0.02f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f)
    };

    toReturn.numVertices = sizeof(tri) / sizeof(*tri);
    toReturn.vertices = new Vertex[toReturn.numVertices];
    memcpy(toReturn.vertices, tri, sizeof(tri));


    GLushort indices[] {0, 1, 2};
    toReturn.numIndices = sizeof(indices) / sizeof(*indices);
    toReturn.indices = new GLushort[toReturn.numIndices];
    memcpy(toReturn.indices, indices, sizeof(indices));


    return toReturn;
}

ShapeData ShapeGenerator::Rectangle(){

    ShapeData toReturn;

    float rect[] = {
        // positions          // colors           // texture coords
         0.02f,  0.02f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.02f, -0.02f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.02f, -0.02f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.02f,  0.02f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    toReturn.numVertices = sizeof(rect) / sizeof(*rect);
    toReturn.vertices = new Vertex[toReturn.numVertices];
    memcpy(toReturn.vertices, rect, sizeof(rect));

    GLushort indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    toReturn.numIndices = sizeof(indices) / sizeof(*indices);
    toReturn.indices = new GLushort[toReturn.numIndices];
    memcpy(toReturn.indices, indices, sizeof(indices));

    return toReturn;


}
