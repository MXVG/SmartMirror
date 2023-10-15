#include "TimeModule.h"

TimeModule::TimeModule(FT_Library& ft, const int& width, const int& height){

    this->name = "Time";
    this->assetFileName = "TimeDisplay";

    this->screenWidth = width;
    this->screenHeight = height;

    this->ft = ft;


    if(this->ft == nullptr){
        std::cout << "ERROR::FREETYPE: No instance of FreeType found" << std::endl;
    }else{

        renderObjects.emplace("TimeDisplay", nullptr);

        loadAssets();
    }

}

TimeModule::~TimeModule(){

}



void TimeModule::getCurrentTime(){

    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    localTime = *std::localtime(&currentTime_t);

    std::string mins = (localTime.tm_min < 10) ? "0" + std::to_string(localTime.tm_min) : std::to_string(localTime.tm_min);

    if(localTime.tm_hour > 12){

        stringTime = std::to_string(localTime.tm_hour - 12) + ":" + mins + " PM";
    }else{
        stringTime = std::to_string(localTime.tm_hour) + ":" + std::to_string(localTime.tm_min) + " AM";
    }
}

void TimeModule::renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO){
    
    getCurrentTime();

    //set OpenGL state for text rendering
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderable* display = renderObjects.at("TimeDisplay");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
    display->shader->use();
    glUniformMatrix4fv(glGetUniformLocation(display->shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    glBindVertexArray(display->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(display->indexVBO));
    glEnableVertexAttribArray(0);

    renderText(display, display->indexVBO, display->indexEBO, *display->shader, stringTime, 25.0f, 25.0f, 2.0f, glm::vec3(1.0f, 1.0f, 1.0f));


    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OPENGL ERROR::Time module " << error << std::endl;
    } 

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);

    // Unbind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   

}

