#ifndef OpenGLWindow_H
#define OpenGLWindow_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWindow(QWindow* parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter* painter);
    virtual void render();
    virtual void initialize();

    void setAnimating(bool animating);

public slots :
    void renderLater();
    void renderNow();

protected :
    bool event(QEvent* event);

    void exposeEvent(QExposeEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext* m_context;
    QOpenGLPaintDevice* m_device;
};

#endif // MAINOpenGLWindow_H
