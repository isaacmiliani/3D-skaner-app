#ifndef MODELLOADER_H
#define MODELLOADER_H


#include <QSharedPointer>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>


#pragma once
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

struct MaterialInfo
{
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};
struct MeshInfo
{
    QString name;
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<MaterialInfo> material;
};

struct NodeInfo
{
    QString name;
    QMatrix4x4 transformation;
    QVector<QSharedPointer<MeshInfo> > meshes;
    QVector<NodeInfo> nodes;
};
class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();
    bool Load(QString pathType);
    void getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices);


    QSharedPointer<NodeInfo> getNodeData() { return m_rootNode; }
    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *mater);
    QSharedPointer<MeshInfo> processMesh(aiMesh *mesh);
    void processNode(const aiScene *scene, aiNode *node, NodeInfo *parentNode, NodeInfo &newNode);

    void transformToUnitCoordinates();
    void findObjectDimensions(NodeInfo *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension);

    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    QVector<QSharedPointer<MaterialInfo> > m_materials;
    QVector<QSharedPointer<MeshInfo> > m_meshes;
    QSharedPointer<NodeInfo> m_rootNode;
};

#endif // MODELLOADER_H
