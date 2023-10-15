#include "HandTrackingM.h"

HandTrackingModule::HandTrackingModule() : MirrorMod(){

    this->name = MODULE_NAME;
    this->assetFileName = "LM";

    renderObjects.emplace("LM", nullptr);

    loadAssets();

    startServerThread();

}

HandTrackingModule::~HandTrackingModule(){

    shouldExit = 1;
    joinServerThread();

    delete[] parsedLM;
    delete[] currLandmarkData;

}


glm::vec3* HandTrackingModule::getLandmarkData(){

    return parsedLM;
}


void HandTrackingModule::serverThreadFunction(){

    int sockfd;
    struct sockaddr_in serverAddr;
    //char buffer[1024];
    //const char* message = "Connected to UDP server...";

    //create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    //bind the socket to the server address
    if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        perror("Binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", port);

    while(!shouldExit){
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);

        char buf[sizeof(float) * NUM_FLOATS];

        //recieve data from client
        ssize_t numBytesReceived = recvfrom(sockfd, buf, sizeof(buf), 0,
        (struct sockaddr *)&clientAddr, &addrLen);


        if(numBytesReceived < 0){
            perror("Receive failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        memcpy(currLandmarkData, buf, sizeof(buf));
        parseLandmarks(currLandmarkData, NUM_FLOATS);


    }

    close(sockfd);

}

void HandTrackingModule::parseLandmarks(const float* currLandmarkData, size_t numFloats){
    

    if(numFloats % 3 != 0){
        throw std::runtime_error("Invalid number of floats for vec3");
    }

    size_t numVec3 = numFloats / 3;

    for(size_t i = 0, j = 0; i < numFloats; i+=3, ++j){
        parsedLM[j] = glm::vec3(currLandmarkData[i], currLandmarkData[i + 1], currLandmarkData[i + 2]);
    }

    //printData(parsedLM, NUM_LANDMARKS);
}

void HandTrackingModule::printData(const glm::vec3* vec3Data, size_t numVec3){
    
    for(size_t i = 0; i < numVec3; ++i){
        const glm::vec3 &vec = vec3Data[i];
        std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;

    }
}


void HandTrackingModule::renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO) {

    Renderable* LM = renderObjects.at("LM");

    glBindVertexArray(LM->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(LM->indexVBO));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float) + LM->indexVBO));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    LM->shader->use();

    glm::mat4 transform = glm::mat4(1.0f);

    size_t parsedLMSize = sizeof(parsedLM) / sizeof(parsedLM[0]);

    for(size_t i = 0; i < NUM_LANDMARKS; ++i){
        transform = glm::rotate(transform, (float)(3/2 * M_PI), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(-1 * parsedLM[i].x + X_OFFSET, parsedLM[i].y - Y_OFFSET, 0.0f));
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, LM->shape.numIndices, GL_UNSIGNED_SHORT, (void*)0);
        transform = glm::mat4(1.0f);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glUseProgram(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OPENGL ERROR::Test module " << error << std::endl;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

}