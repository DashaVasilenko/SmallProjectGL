#ifndef __ENGINE__
#define __ENGINE__

#include "resourceManager.h"
#include "shaderProgram.h"
#include "geometry.h"
#include "texture.h"

class Engine {
public:
    static ResourceManager<ShaderProgram> programManager;
    static ResourceManager<Texture2D> textureManager;
    static ResourceManager<Geometry> geometryManager;
    
private:
}; 

#endif /* End Of Engine */