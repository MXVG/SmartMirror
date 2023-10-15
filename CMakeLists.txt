cmake_minimum_required(VERSION 3.0.0)
project(SmartMirror VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)


add_executable(SmartMirror src/main.cpp /usr/include/glad/glad.h src/glad.c 
src/shader_s.h src/shader_s.cpp 
/usr/include/glm/glm.hpp 
/usr/include/glm/gtc/matrix_transform.hpp 
/usr/include/glm/gtc/type_ptr.hpp 
src/ShapeLibrary/Vertex.h 
src/ShapeLibrary/ShapeData.h 
src/ShapeLibrary/ShapeGen.h src/ShapeLibrary/ShapeGen.cpp 
src/GraphicsManager.h src/GraphicsManager.cpp 
src/Renderable.h 
src/HttpClient.h src/HttpClient.cpp   
src/Modules/MirrorMod.h src/Modules/MirrorMod.cpp 
src/Modules/HandTracking/HandTrackingM.h src/Modules/HandTracking/HandTrackingM.cpp 
src/Modules/Test/TestModule.h src/Modules/Test/TestModule.cpp 
src/Modules/Time/TimeModule.h src/Modules/Time/TimeModule.cpp 
src/Modules/Weather/WeatherModule.h src/Modules/Weather/WeatherModule.cpp)

target_link_libraries(SmartMirror glfw OpenGL::GL freetype curl)