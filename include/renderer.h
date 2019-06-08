#ifndef __RENDERER__
#define __RENDERER__

#include "window.h"

class Renderer {
public:
    void SetWidth(int);
    void SetHeight(int);
    void Init();
    void Update();
private:
    int width;
    int height;
};

#endif /* End of __RENDERER__ */