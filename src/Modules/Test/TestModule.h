#ifndef TEST_H
#define TEST_H


#include "../MirrorMod.h"


class TestModule : public MirrorMod {

    public:

        TestModule();
        ~TestModule();

    void renderModule(unsigned int vertexBufferID, unsigned int indexBufferID, unsigned int textBufferID, GLintptr GMoffsetVBO, GLintptr GMoffsetEBO) override;

    private:

        void printIndexBufferContents(unsigned int indexBufferID);

};

#endif