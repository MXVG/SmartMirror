
#include "TestModule.h"

TestModule::TestModule(){

    this->name = "Test";
    this->assetFileName = "Triangle";

    renderObjects.emplace("Triangle", nullptr);

    loadAssets();
}


TestModule::~TestModule(){

}



void TestModule::renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO){


    Renderable* triangleRenderable = renderObjects.at("Triangle");



    glBindVertexArray(triangleRenderable->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(triangleRenderable->indexVBO));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float) + triangleRenderable->indexEBO));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    triangleRenderable->shader->use();

    glDrawElements(GL_TRIANGLES, triangleRenderable->shape.numIndices, GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glUseProgram(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OPENGL ERROR::Test module " << error << std::endl;
    } 
    
    // Unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);    

}


void TestModule::printIndexBufferContents(unsigned int indexBufferID){
    // Bind the index buffer you want to inspect
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

    // Get the buffer size
    GLint bufferSize = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

    // Allocate memory to store buffer data
    std::vector<GLushort> bufferData(bufferSize / sizeof(GLushort));

    // Retrieve the buffer data
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferSize, bufferData.data());

    // Print the buffer data to the console
    for (size_t i = 0; i < bufferData.size(); i++) {
        std::cout << "Index[" << i << "] = " << bufferData[i] << std::endl;
    }
    
    // Unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
