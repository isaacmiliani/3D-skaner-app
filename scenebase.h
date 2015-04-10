#ifndef SCENEBASE_H
#define SCENEBASE_H


class SceneBase
{
public:
    virtual void initialize() = 0;
    virtual void resize(int w, int h) = 0;
    virtual void update() = 0;
    virtual void cleanup() = 0;
};

#endif // SCENEBASE_H
