#include <QApplication>
#include "scene.h"
#include "sceneselector.h"

#include <openglwindow.h>
#include <window.h>
//QString OpenGL_Model("/home/isaac/Documents/QT Projects/3DModelLoader/KinectFusionReconstructio3.ply");
//QString pathType("/home/isaac/Documents/QT Projects/3DModelLoader/KinectFusionReconstructio3.ply");

// OpenGL ES 2.0, a single vertex buffer can have at max std::numeric_limits<unsigned short>::max() vertices,
// so use a smaller model if rendering with OpenGL ES
//QString OpenGLES_Model("demon_head/demon_head.3ds");

QString filePath("/home/ccg/Documentos/Qt Projects/3DModelLoader/chair.ply");
QString texturePath("");
class SceneSelect : public SceneSelector {

    bool isOpenGLES;

    SceneBase* getScene(QPair<int, int> glVersion) {
        // use Scene class when GL version is 3.3

        isOpenGLES=false;
        if (glVersion == qMakePair(3,3) && !isOpenGLES) {
            return new Scene(filePath, filePath, texturePath);
        }
        // just use GL ES scene for any other version
        //else if (isOpenGLES) {
        //    return new Scene(OpenGLES_Model, ModelLoader::RelativePath);
        //}
        else{
            return new Scene(filePath, filePath, texturePath);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SceneSelect sceneSelect;
    //Window w2;
    OpenGLWindow w1(&sceneSelect,40,3,3);
    w1.show();
    //w2.show();
    return a.exec();
}

