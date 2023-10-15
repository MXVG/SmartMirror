#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <vector>
//#include "shader_s.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "GraphicsManager.h"
#include "Modules/MirrorMod.h"
#include "Modules/HandTracking/HandTrackingM.h"
#include "Modules/Test/TestModule.h"
#include "Modules/Time/TimeModule.h"
#include "Modules/Weather/WeatherModule.h"


int main(){


  GraphicsManager gfxManager = GraphicsManager();

  gfxManager.start();


  //TestModule moduleTest;
  //gfxManager.addModule(&moduleTest);


  TimeModule clock = TimeModule(gfxManager.ft, gfxManager.width, gfxManager.height);
  WeatherModule weather = WeatherModule(gfxManager.ft, gfxManager.width, gfxManager.height);
  //HandTrackingModule handTracking = HandTrackingModule();


  gfxManager.addModule(&clock);
  gfxManager.addModule(&weather);
  //gfxManager.addModule(&handTracking);

  gfxManager.loop();


  return 0;
}