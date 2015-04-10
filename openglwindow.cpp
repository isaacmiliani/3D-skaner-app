#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(SceneSelector *sceneSelector, int refreshRate, int major, int minor, QScreen* screen)
    : QWindow(screen)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    QSurfaceFormat requestedFormat;
    requestedFormat.setDepthBufferSize(24);
    requestedFormat.setMajorVersion(major);
    requestedFormat.setMinorVersion(minor);

    requestedFormat.setSamples(4);
    requestedFormat.setProfile(QSurfaceFormat::CoreProfile);

    m_context = new QOpenGLContext;
    m_context->setFormat(requestedFormat);
    m_context->create();

    if (m_context->format().version() == qMakePair(3,3)) {
        qDebug() << "Using OpenGL 3.3 context";
        m_scene = sceneSelector->getScene(qMakePair(3,3));
    } else {
        m_context->deleteLater();
        requestedFormat.setMajorVersion(2);
        requestedFormat.setMinorVersion(1);
        m_context = new QOpenGLContext;
        m_context->setFormat(requestedFormat);
        m_context->create();

        if (m_context->format().version().first < 2) {
            qDebug() << "Unable to get a valid version of OpenGL, aborting";
            exit(1);
        }

        qDebug() << "Unable to get an OpenGL 3.3 context. Version is " << m_context->format().version() << ", Using OpenGL 2.1/OpenGL ES context";
        m_scene = sceneSelector->getScene(qMakePair(2,1));
    }
    setSurfaceType(OpenGLSurface);
    setFormat(m_context->format());
    resize(800, 600);
    create();

    connect(this, SIGNAL(widthChanged(int)), this, SLOT(resizeGL()));
    connect(this, SIGNAL(heightChanged(int)), this, SLOT(resizeGL()));
    connect(m_context, SIGNAL(aboutToBeDestroyed()), this, SLOT(cleanup()), Qt::DirectConnection);

    initializeGL();
    resizeGL();

    m_timer = new QTimer;
    m_timer->setInterval(refreshRate);
    connect(m_timer, &QTimer::timeout, this, &OpenGLWindow::updateGL);
    m_timer->start();

}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::initializeGL()
{
    m_context->makeCurrent(this);

    m_scene->initialize();
}

void OpenGLWindow::updateGL()
{
    if (!isExposed())
        return;

    m_context->makeCurrent(this);

    m_scene->update();

    m_context->swapBuffers(this);
}

void OpenGLWindow::resizeGL()
{
    m_context->makeCurrent(this);

    m_scene->resize(width(), height());
    updateGL();
}

void OpenGLWindow::cleanup()
{
    m_context->makeCurrent(this);

    m_scene->cleanup();
}

void OpenGLWindow::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void OpenGLWindow::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void OpenGLWindow::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void OpenGLWindow::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


