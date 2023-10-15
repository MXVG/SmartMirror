#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(){
    

    //Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Get monitor info
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);


    this->width = mode->width;
    this->height = mode->height;

    if(FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not initialize Freetype Library" << std::endl;
    }

}

GraphicsManager::GraphicsManager(const int& width, const int& height){

    //Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->width = width;
    this->height = height;

}

GraphicsManager::~GraphicsManager(){

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);

    glBindVertexArray(0);

    glfwTerminate();

}

void GraphicsManager::framebuffer_size_callback(GLFWwindow* window, int width, int height){

    glViewport(0, 0, width, height);

}

void GraphicsManager::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int GraphicsManager::start(){


    //Create window
    window = glfwCreateWindow(this->width, this->height, "Smart Mirror", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    //Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    //set viewport
    glViewport(0, 0, width, height);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //create text buffer
    glGenBuffers(1, &textBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //create vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    //create index buffer
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    // 4MB of space for vertices
    glBufferData(GL_ARRAY_BUFFER, 4 * 1204 * 1204, nullptr, GL_DYNAMIC_DRAW);

    //1MB of space for indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * 1204 * 1204, nullptr, GL_DYNAMIC_DRAW);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error at start: " << error << std::endl;
    }

    //Unbind both buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return EXIT_SUCCESS;
}

void GraphicsManager::loop(){

    while(!glfwWindowShouldClose(this->window)){
        
        processInput(this->window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < activeModules.size(); i++){

            activeModules.at(i)->renderModule(vertexBuffer, indexBuffer, textBuffer, vertexBufferOffset, indexBufferOffset);

        }

        //Swap buffers
        glfwSwapBuffers(this->window);

        //Poll for and process events
        glfwPollEvents();

    }

}


void GraphicsManager::addModule(MirrorMod* module){

    for(const auto& pair : module->renderObjects){

        glGenVertexArrays(1, &module->renderObjects[pair.first]->VAO);

        if(module->renderObjects[pair.first]->isText != true){

            //Load module geometry into Vertex and Index Buffers
            ShapeData renderData = module->renderObjects[pair.first]->shape;

            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);


            //Pass index in memory for rendering
            module->renderObjects[pair.first]->indexVBO = vertexBufferOffset;
            module->renderObjects[pair.first]->indexEBO = indexBufferOffset;


            //Push vertex data into vertex buffer
            glBufferSubData(GL_ARRAY_BUFFER, vertexBufferOffset, 
            renderData.vertexBufferSize(), renderData.vertices);

            vertexBufferOffset += renderData.vertexBufferSize();

            //Push indice data into indice buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexBufferOffset, 
            renderData.indexBufferSize(), renderData.indices);

            indexBufferOffset += renderData.indexBufferSize();
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error at addModule: " << error << std::endl;
        }
    }

    activeModules.push_back(module);

}

