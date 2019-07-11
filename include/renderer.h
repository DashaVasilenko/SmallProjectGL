#ifndef __RENDERER__
#define __RENDERER__

#include "window.h"
#include "camera.h"
#include "mesh.h"

class Renderer {
public:
    void SetWidth(int);
    void SetHeight(int);

    void SetActiveCamera(const Camera* camera);
    void AddMesh(const Mesh*);
    void Init();
    void Update();
private:
    glm::mat4 projection;
    std::vector<const Mesh*> meshes;
    const Camera* camera;
    int width;
    int height;
};

#endif /* End of __RENDERER__ */