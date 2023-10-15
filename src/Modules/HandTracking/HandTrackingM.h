#ifndef HANDTRACKINGM_H
#define HANDTRACKINGM_H

#include <iostream>

#include "../MirrorMod.h"

#include "../../ShapeLibrary/ShapeGen.h"

#include <array>
#include <ostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

#define MODULE_NAME "HandTracking"
#define NUM_VAO 1

#define NUM_LANDMARKS 20

#define NUM_FLOATS 60
#define PORT 8080

#define X_OFFSET 0.4f
#define Y_OFFSET 0.2f

class HandTrackingModule : public MirrorMod {

    public:
        HandTrackingModule();
        ~HandTrackingModule();

        //DEBUG
        void printData(const glm::vec3* vec3Data, size_t numVec3);

        glm::vec3* getLandmarkData();

        void renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO) override;


    private:

        float* currLandmarkData = new float[NUM_FLOATS]; //60 total floats for 20 total LMs
        glm::vec3* parsedLM = new glm::vec3[NUM_LANDMARKS];

        pthread_t serverThread;
        const int port = PORT;
        int shouldExit = 0;
        pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;

        void startServerThread(){
            if(pthread_create(&serverThread, NULL, serverThreadEntry, this) != 0){
                perror("Failed to create server thread");
            }
        }

        void joinServerThread(){
            pthread_join(serverThread, NULL);
        }

        static void* serverThreadEntry(void* arg){

            HandTrackingModule* module = static_cast<HandTrackingModule*>(arg);

            module->serverThreadFunction();

            return NULL;
        }

        void serverThreadFunction();

        void parseLandmarks(const float* currLandmarkData, size_t numFloats);



};

#endif