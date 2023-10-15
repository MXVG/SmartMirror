#include "MirrorMod.h"

MirrorMod::MirrorMod(){

    debug.SetupDebugOutput();

}


MirrorMod::~MirrorMod(){

    //FT_Done_FreeType(this->ft);

}


void MirrorMod::loadAssets(){

    namespace fs = std::filesystem;

    for(const auto pair : renderObjects){

        std::string folderPath = std::string("/home/mxvg/Desktop/projects/SmartMirror/Shaders/src/Modules") + std::string("/") + name + std::string("/") + pair.first;

        std::string vertFilePath;
        std::string fragFilePath;
        std::string fontFilePath = "";
        std::string textureFilePath = "";


        for(const auto& entry : fs::directory_iterator(folderPath)){


            if(fs::is_regular_file(entry)){

                fs::path filePath = entry.path();

                const std::string& ext = filePath.extension().string();
                
                if(ext == ".vert")
                {

                    std::cout << "loaded vertex file: " << filePath << std::endl;
                    vertFilePath = filePath.string();

                }
                else if(ext == ".frag")
                {
                    std::cout << "loaded fragment file: " << filePath << std::endl;
                    fragFilePath = filePath.string();
                }
                else if(ext == ".ttf")
                {
                    std::cout << "loaded font file: " << filePath << std::endl;
                    fontFilePath = filePath.string();
                }
                else if(ext == ".jpg")
                {
                    std::cout << "loaded texture: " << filePath << std::endl;
                    

                }
            }
        }

        if(fontFilePath != ""){
            
            std::map<GLchar, Character*> c = loadFont(fontFilePath);
            Renderable* r =  new Renderable(pair.first, c);
            r->isText = true;

            renderObjects[pair.first] = r;
        }else{
            
            ShapeData t = ShapeGenerator::Triangle();
            Renderable* s = new Renderable(pair.first, t);
            s->isText = false;

            renderObjects[pair.first] = s;
        }   

        renderObjects[pair.first]->shader = new Shader(vertFilePath.c_str(), fragFilePath.c_str());

    }

}


//NOTE: DOES NOT BIND/UNBIND ANY VAOS
void MirrorMod::renderText(Renderable* charRenderable, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO, Shader &shader, 
std::string text, float x, float y, float scale, glm::vec3 color){

    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    
    //iterate through characters
    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++){

        Character* ch = charRenderable->charSet[*c];

        float xpos = x + ch->Bearing.x * scale;
        float ypos = y - (ch->Size.y - ch->Bearing.y) * scale;

        float w = ch->Size.x * scale;
        float h = ch->Size.y * scale;

        //update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f } 
        };

        //render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch->TextureID);
        //update content of VBO memory
        glBufferSubData(GL_ARRAY_BUFFER, GMoffsetVBO, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        //advance cursors for next glyph (advance = 1/64)
        x += (ch->Advance >> 6) * scale; // (2^6 = 64) 1/64 for amount of pixels
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

std::map<GLchar, Character*> MirrorMod::loadFont(std::string filePath){

    std::map<GLchar, Character*> characterMap;

    FT_Face font;
    if(FT_New_Face(ft, filePath.c_str(), 0, &font)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(font, 0, 48); //CHANGE THIS

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //load first 128 ASCII chars
    for(unsigned char c = 0; c < 128; c++){

        //load character glyph
        if(FT_Load_Char(font, c, FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
            continue;
        }

        //generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            font->glyph->bitmap.width,
            font->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            font->glyph->bitmap.buffer
        );
        //set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //store character for later use
        Character* character = new Character{
            texture,
            glm::ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows),
            glm::ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top),
            static_cast<unsigned int>(font->glyph->advance.x)
        };
        characterMap.insert(std::pair<char, Character*>(c, character));

    }

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(font);

    return characterMap;

}

void MirrorMod::renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO){

    printf("No render behavior programmed\n");
}


