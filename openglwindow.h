#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <sceneselector.h>
#include <QTimer>
#include <scenebase.h>
#include <QOpenGLContext>
#include <modelloader.h>
#include <QMouseEvent>

class OpenGLWindow : public QWindow
{
    Q_OBJECT

public:
    OpenGLWindow(SceneSelector *scene, int refreshRate, int major=3, int minor=3, QScreen* screen = 0);
    ~OpenGLWindow();

    ModelLoader modelLoader;

protected:
    void initializeGL();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QTimer *m_timer;
    SceneBase *m_scene;
    QOpenGLContext* m_context;
    float xRot;
    float yRot;
    float zRot;
    bool m_error;
    QPoint lastPos;

protected slots:
    void updateGL();
    void resizeGL();
    void cleanup();
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
};
#endif
