#ifndef TIMEMODULE_H
#define TIMEMODULE_H

#include <iostream>
#include <chrono>
#include "../MirrorMod.h"




class TimeModule : public MirrorMod {

    public:

        TimeModule(FT_Library& ft, const int& width, const int& height);
        ~TimeModule();


        void renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO) override;

    private:

        std::tm localTime;
        std::string stringTime;

        void getCurrentTime();


};

#endif