#include "scene.h"

Scene::Scene(QString filepath, QString pathType, QString texturePath) :
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_filepath(filepath),
    m_pathType(pathType),
    m_texturePath(texturePath),
    m_error(false)
{

}


void Scene::initialize()
{
    this->initializeOpenGLFunctions();

    createShaderProgram("/home/ccg/Documentos/Qt Projects/3DModelLoader/ads_fragment.vert", "/home/ccg/Documentos/Qt Projects/3DModelLoader/ads_fragment.frag");
    createBuffers();
    createAttributes();
    setupLightingAndMatrices();

    glEnable(GL_DEPTH_TEST);
    glClearColor(.5, .5, .5, 1.0);
}
void Scene::createShaderProgram(QString vShader, QString fShader)
{
    // Compile vertex shader
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vShader.toUtf8()))
        qCritical() << "Unable to compile vertex shader. Log:" << m_shaderProgram.log();

    // Compile fragment shader
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fShader.toUtf8()))
        qCritical() << "Unable to compile fragment shader. Log:" << m_shaderProgram.log();

    // Link the shaders together into a program
    if (!m_shaderProgram.link())
        qCritical() << "Unable to link shader program. Log:" << m_shaderProgram.log();
}

void Scene::createBuffers()
{
    ModelLoader model;

    if (!model.Load(m_filepath))
    {
        m_error = true;
        return;
    }

    QVector<float> *vertices;
    QVector<float> *normals;
    QVector<unsigned int> *indices;

    model.getBufferData(&vertices, &normals, &indices);
    // Create a vertex array object
    m_vao.create();
    m_vao.bind();

    // Create a buffer and copy the vertex data to it
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(&(*vertices)[0], vertices->size() * sizeof(float));

    // Create a buffer and copy the vertex data to it
    m_normalBuffer.create();
    m_normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normalBuffer.bind();
    m_normalBuffer.allocate(&(*normals)[0], normals->size() * sizeof(float));

    // Create a buffer and copy the index data to it
    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.bind();
    m_indexBuffer.allocate(&(*indices)[0], indices->size() * sizeof(unsigned int));

    m_rootNode = model.getNodeData();
}

void Scene::createAttributes()
{
    if (m_error)
        return;

    m_vao.bind();
    // Set up the vertex array state
    m_shaderProgram.bind();

    // Map vertex data to the vertex shader's layout location '0'
    m_vertexBuffer.bind();
    m_shaderProgram.enableAttributeArray(0);       // layout location
    m_shaderProgram.setAttributeBuffer(0,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       3);         // number of components (3 for x,y,z)

    // Map normal data to the vertex shader's layout location '1'
    m_normalBuffer.bind();
    m_shaderProgram.enableAttributeArray(1);       // layout location
    m_shaderProgram.setAttributeBuffer(1,          // layout location
                                       GL_FLOAT,   // data's type
                                       0,          // Offset to data in buffer
                                       3);         // number of components (3 for x,y,z)
}

void Scene::setupLightingAndMatrices()
{
    m_view.setToIdentity();
    m_view.lookAt(
                QVector3D(0.0f, 0.0f, 1.2f),    // Camera Position
                QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    float aspect = 4.0f/3.0f;
    m_projection.setToIdentity();
    m_projection.perspective(
                60.0f,          // field of vision
                aspect,         // aspect ratio
                0.3f,           // near clipping plane
                1000.0f);       // far clipping plane
    m_lightInfo.Position = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
    m_lightInfo.Intensity = QVector3D(1.0f, 1.0f, 1.0f);

    m_materialInfo.Ambient = QVector3D(0.1f, 0.05f, 0.0f);
    m_materialInfo.Diffuse = QVector3D(.9f, .6f, .2f);
    m_materialInfo.Specular = QVector3D(.2f, .2f, .2f);
    m_materialInfo.Shininess = 50.0f;
}

void Scene::resize(int w, int h)
{
    glViewport(0, 0, w, h);

    m_projection.setToIdentity();
    m_projection.perspective(60.0f, (float)w/h, .3f, 1000);
}

void Scene::update()
{
    if (m_error)
        return;

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind shader program
    m_shaderProgram.bind();

    // Set the model matrix
    // Translate and rotate it a bit to get a better view of the model
    m_model.setToIdentity();
    m_model.translate(0.0f, 0.0f, 0.0f);
    m_model.rotate(0.0f,0.0f,0.0f, 0.0f);

    // Set shader uniforms for light information
    m_shaderProgram.setUniformValue("lightPosition", m_lightInfo.Position);
    m_shaderProgram.setUniformValue("lightIntensity", m_lightInfo.Intensity);

    // Bind VAO and draw everything
    m_vao.bind();
    drawNode(m_rootNode.data(), QMatrix4x4());
    m_vao.release();

}

void Scene::drawNode(const NodeInfo *node, QMatrix4x4 objectMatrix)
{
    // Prepare matrices
    objectMatrix *= node->transformation;
    QMatrix4x4 modelMatrix = m_model * objectMatrix;
    QMatrix4x4 modelViewMatrix = m_view * modelMatrix;
    QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();
    QMatrix4x4 mvp = m_projection * modelViewMatrix;

    m_shaderProgram.setUniformValue("MV", modelViewMatrix);// Transforming to eye space
    m_shaderProgram.setUniformValue("N", normalMatrix);    // Transform normal to Eye space
    m_shaderProgram.setUniformValue("MVP", mvp);           // Matrix for transforming to Clip space

    // Draw each mesh in this node
    for (int imm = 0; imm < node->meshes.size(); ++imm)
    {
        if (node->meshes[imm]->material->Name == QString("DefaultMaterial"))
            setMaterialUniforms(m_materialInfo);
        else
            setMaterialUniforms(*node->meshes[imm]->material);

        glDrawElements(GL_TRIANGLES, node->meshes[imm]->indexCount, GL_UNSIGNED_INT, (const void*)(node->meshes[imm]->indexOffset * sizeof(unsigned int)));
    }

    // Recursively draw this nodes children nodes
    for (int inn = 0; inn < node->nodes.size(); ++inn)
        drawNode(&node->nodes[inn], objectMatrix);
}

void Scene::setMaterialUniforms(MaterialInfo &mater)
{
    m_shaderProgram.setUniformValue("Ka", mater.Ambient);
    m_shaderProgram.setUniformValue("Kd", mater.Diffuse);
    m_shaderProgram.setUniformValue("Ks", mater.Specular);
    m_shaderProgram.setUniformValue("shininess", mater.Shininess);
}

void Scene::cleanup(){

}
