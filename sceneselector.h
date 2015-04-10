#ifndef SCENESELECTOR_H
#define SCENESELECTOR_H

#include <scenebase.h>
#include <QPair>

class SceneSelector
{
public:
    virtual SceneBase* getScene(QPair<int,int> glVersion) = 0;
};

#endif // SCENESELECTOR_H
