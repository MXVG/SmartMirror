#ifndef WEATHERMODULE_H
#define WEATHERMODULE_H

#include <iostream>
#include "../../HttpClient.h"
#include "../MirrorMod.h"




class WeatherModule : public MirrorMod {

    public:

        WeatherModule(FT_Library& ft, const int& width, const int& height);
        ~WeatherModule();


        void renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO) override;

    private:


        double elevation;
        double hourlyRain[24];
        double hourlyTemp[24];

        double avgRain;
        double avgTemp;

        double currTemp;
        
        double dailyHighTemp;
        double dailyLowTemp;

        void accessAPI();

        void computeAvgData();


};

#endif