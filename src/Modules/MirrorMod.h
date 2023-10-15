#ifndef MIRRORMOD_H
#define MIRRORMOD_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include "../Third_Party/stb_image.h"
#include "../shader_s.h"
#include "../ShapeLibrary/ShapeGen.h"

#include "../Renderable.h"

#include <ft2build.h>
#include FT_FREETYPE_H


class OpenGLDebug {

    public:
        static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

            std::cerr << "OpenGL Debug Message (" << id << "): " << message << std::endl;
        }

        void SetupDebugOutput(){
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(DebugCallback, nullptr);
        }


};

/** MIRROR MODULE
 * 
 * Description
 * -----------------
 * Base class for modules. Each module has it's own function
 * e.g. getting the weather, handtracking, etc. Each module
 * function should contain the logic to implement it's functionality,
 * and a renderModule() function for the GraphicsManager to call to render
 * the module's objects. This base class just has a function for loading 
 * module assets.
 * 
 * Initialization
 * -----------------
 *      input: N/A
 *      output: N/A
 *      Error:
 *          Not intended to be directly called as an object
 *          Module name needs to be defined in child classes
 * 
 * Funcionality
 * -----------------
 * Intended to serve as a base class for modules
 * Loads module assets
*/


class MirrorMod{


    public:
        MirrorMod();
        ~MirrorMod();

        //Module name
        std::string name;

        /** renderModule()
         * -----------------
         * Input: 
         *      * [unsigned int] vertexBufferID
         *      * [unsigned int] indexBufferID
         *      * [unsigned int] textBufferID
         *      * [GLintptr] GMoffsetVBO
         *      * [GLintptr] GMoffsetEBO
         * Returns: void
         * Error:
         *      Not overridding this function in child classes
         * Description:
         *      Intended to be overidden by classes that inherit from this one.
         *      Called inside GraphicsManager's render loop.
         * 
        */
        virtual void renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO);


        /** renderText()
         * -----------------
         * Input:
         *      * [Renderable*] charRenderData
         *      * [GLintptr] GMoffsetVBO
         *      * [GLintptr] GMoffsetEBO
         *      * [Shader] shaderToUse
         *      * [string] text
         *      * [float] x position
         *      * [float] y position
         *      * [float] z position
         *      * [float] text scale
         *      * [glm::vec3] text color
         * Returns:
         *      void
         * Error:
         *      Correct buffer offsets need to be supplied
         * Description:
         *      To be called in a modules renderModule() function.
         *      Renders text with specified parameters. 
         * 
        */
        void renderText(Renderable* charRenderable, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO, Shader &shader,
        std::string text, float x, float y, float scale, glm::vec3 color);

        std::map<std::string, Renderable*> renderObjects;

    protected:

        //FreeType Library instance for rendering fonts
        FT_Library ft;

        //Window width and height
        int screenWidth;
        int screenHeight;

        //Vector of strings to hold module's asset names
        std::vector<std::string> assetNames;

        std::string assetFileName;

        /** loadAssets()
         * -----------------
         * Input: N/A
         * Returns: void
         * Error:
         *      File where assets are located does not have same name as module
         * Description:
         * Looks for and loads files of type .vert, .frag, .ttf in
         * the file with the module's corresponding name.
         * Usually called in child classes' constructor functions.
         * 
        */
        void loadAssets();

        /** loadFont()
         * -----------------
         * Input: [string] filePath
         * Returns: [map<GLchar, Character>] character atlas
         * Error:
         *      Invalid file path
         *      Invalid ttf file
         * Description:
         *      Loads font of .ttf file.
         *      Can only handle 128 ASCII characters.
         * 
        */
        std::map<GLchar, Character*> loadFont(std::string filePath);

        // OpenGLDebud instance
        OpenGLDebug debug;

};

#endif