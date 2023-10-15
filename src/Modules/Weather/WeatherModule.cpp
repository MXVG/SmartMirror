#include "WeatherModule.h"

WeatherModule::WeatherModule(FT_Library& ft, const int& width, const int& height){

    this->name = "Weather";
    this->assetFileName = "WeatherAssets";

    this->screenWidth = width;
    this->screenHeight = height;

    this->ft = ft;


    if(this->ft == nullptr){
        std::cout << "ERROR::FREETYPE: No instance of FreeType found" << std::endl;
    }else{

        renderObjects.emplace("WeatherAssets", nullptr);

        loadAssets();

        accessAPI();
    }



}

WeatherModule::~WeatherModule(){
    std::cout << "deleted";
}


void WeatherModule::accessAPI(){

    HttpClient client;

    std::string url = "https://api.open-meteo.com/v1/forecast?latitude=30.628&longitude=-96.3344&hourly=temperature_2m,rain,showers&daily=temperature_2m_max,temperature_2m_min&temperature_unit=fahrenheit&timezone=America%2FChicago&forecast_days=1";

    nlohmann::json response = client.Get(url);

    //Check if response is empy or error
    if(response.empty()){
        std::cerr << "Failed to retrieve JSON data." << std::endl;
    }

    std::cout << "Received JSON data... " << std::endl;
    //std::cout << response.dump(4) << std::endl;


    try{
        this->elevation = response["elevation"];
        
        nlohmann::json hourlyData = response["hourly"];
        nlohmann::json dailyData = response["daily"];

        this->dailyLowTemp = dailyData["temperature_2m_min"][0];
        this->dailyHighTemp = dailyData["temperature_2m_max"][0];


        for(int i = 0; i < 144; i++){
            this->hourlyRain[i] = hourlyData["rain"][i];
            this->hourlyTemp[i] = hourlyData["temperature_2m"][i];
        }



    }catch(const nlohmann::json::exception& e){
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    computeAvgData();

}


void WeatherModule::renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO){

    //set OpenGL state for text rendering
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderable* display = renderObjects.at("WeatherAssets");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
    display->shader->use();
    glUniformMatrix4fv(glGetUniformLocation(display->shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    glBindVertexArray(display->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(display->indexVBO));
    glEnableVertexAttribArray(0);

    renderText(display, display->indexVBO, display->indexEBO, *display->shader, "Daily High TEMP: " + std::to_string((int) this->dailyHighTemp) + "F", 25.0f, 1000.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));


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

void WeatherModule::computeAvgData(){

    //all hourly data has same array size
    int arrSize = sizeof(this->avgRain) / sizeof(double);

    for(int i = 0; i < arrSize; i++){

        this->avgRain = 0;
        this->avgTemp = 0;

        this->avgRain += hourlyRain[i];
        this->avgTemp += hourlyTemp[i];
    }

    this->avgRain /= arrSize;
    this->avgTemp /= arrSize;

}