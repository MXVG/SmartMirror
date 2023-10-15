#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <functional>
#include "ShapeLibrary/ShapeGen.h"
#include "Modules/MirrorMod.h"
#include <ft2build.h>

#include FT_FREETYPE_H

/**
 * GRAPHICS MANAGER
 * 
 * Description
 * -----------------
 * Responsible for managing and rendering all objects belonging to currently
 * active modules of type MirrorModule.
 * 
 * Initialization:
 * -----------------
 *      Input: int screen width, int screen height
 *      Error: negative width, negative height
 *      Dependencies: Glad, glfw, ft2build.h, shader_s.h
 * 
 * Functionality:
 * -----------------
 * (1) Instantiate in main()
 * (2) Create application window
 * (3) Load modules to be rendered
 * (4) Start render loop
 * 
 * Class Members:
 * -----------------
 * * [int] width - screen width
 * * [int] height - screen height
 * * [FT_Library] ft - text rendering library instance
 * * [uint] vertexBufferID
 * * [uint] indexBufferID
 * * [vector<MirrorMod*>] activeModules - modules to render
 * * [GLFWwindow*] window - application window ptr 
 * 
 * 
*/
class GraphicsManager{

    public:


        GraphicsManager();
        GraphicsManager(const int& width, const int& height);
        ~GraphicsManager();

        //window dimensions (Testing 640x480)
        int width;
        int height;

        FT_Library ft;

        /** start()
         * -----------------
         * Input: N/A
         * Returns: [int] errorcode 
         * Error:
         *      glfw not initialized
         *      glad not initialized
         *      glad/glfw headerfiles included before glm
         * Description: Initializes glfw and glad. Creates application window
        */
        int start();


        /** loop()
         * -----------------
         * Input: N/A
         * Returns: void
         * Error: N/A
         * Description: Render loop for the application. Calls each active modules'
         * renderModule() to render each modules objects every frame.
        */
        void loop();


        /** addModule()
         * -----------------
         * Input: [MirrorMod*] module
         * Returns: void
         * Error:
         *      Module's renderable objects do not fit inside vertex/index/text buffers
         * Description: Adds a new module to be rendered in the render loop
        */
        void addModule(MirrorMod* module);

    private:

        //vertex and index buffer IDs
        unsigned int vertexBuffer;
        unsigned int indexBuffer;

        //text buffer IDs
        unsigned int textBuffer;

        //offsets for vertex and index buffers. Increases/decreases when new module is added/deleted
        GLintptr vertexBufferOffset = 0;
        GLintptr indexBufferOffset = 0;


        //Active Modules
        std::vector<MirrorMod*> activeModules;

        GLFWwindow* window;

        //Callback function to get input
        void processInput(GLFWwindow *window);

        //Callback function to adjust window size
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

};