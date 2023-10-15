#ifndef RENDERABLE_H
#define RENDERABLE_H



#include <string>
#include <map>
#include <filesystem>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader_s.h"
#include "ShapeLibrary/ShapeGen.h"
#include <ft2build.h>
#include FT_FREETYPE_H

// state info of loaded chars
struct Character {
    unsigned int TextureID; 
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

struct Renderable {

    std::string name;
    bool isText;

    unsigned int VAO;
    GLsizeiptr indexVBO;
    GLsizeiptr indexEBO;

    unsigned int textureID; //Only to be used if there's a texture needed for the shader
    
    Shader* shader;

    ShapeData shape;
    std::map<GLchar, Character*> charSet;

    Renderable(std::string name, ShapeData shape) : name(name), shape(shape){};
    Renderable(std::string name, std::map<GLchar, Character*> characters) : name(name), charSet(characters){};

    void render(){

    };

    ~Renderable();


};

#endif
