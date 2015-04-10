#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLFunctions_3_3_Core>
#include "scenebase.h"
#include "modelloader.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QSharedPointer>


struct LightInfo
{
    QVector4D Position;
    QVector3D Intensity;
};

struct Mesh
{
    QString name;
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<MaterialInfo> material;
};

struct Node
{
    QString name;
    QMatrix4x4 transformation;
    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};

class Scene : public QOpenGLFunctions_3_3_Core, public SceneBase
{
public:
    Scene(QString filepath, QString pathType, QString texturePath="");
    void initialize();
    void resize(int w, int h);
    void update();
    void cleanup();


private:
    void createShaderProgram(QString vShader, QString fShader);
    void createBuffers();
    void createAttributes();
    void setupLightingAndMatrices();

    void draw();
    void drawNode(const NodeInfo *node, QMatrix4x4 objectMatrix);
    void setMaterialUniforms(MaterialInfo &mater);

    QOpenGLShaderProgram m_shaderProgram;

    QOpenGLVertexArrayObject m_vao;

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_textureUVBuffer;
    QOpenGLBuffer m_indexBuffer;

    QSharedPointer<NodeInfo> m_rootNode;

    QMatrix4x4 m_projection, m_view, m_model;

    QString m_filepath;
    QString m_pathType;
    QString m_texturePath;

    LightInfo m_lightInfo;
    MaterialInfo m_materialInfo;

    bool m_error;
};

#endif // SCENE_H
