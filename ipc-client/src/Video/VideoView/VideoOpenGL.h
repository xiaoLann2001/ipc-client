#ifndef VIDEOOPENGL_H
#define VIDEOOPENGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class VideoOpenGL : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    enum ImageFormat
    {
        IMAGE_FORMAT_Unknown,
        IMAGE_FORMAT_NV12,       // y... uvuv...
        IMAGE_FORMAT_NV21,       // y... vuvu...
        IMAGE_FORMAT_I420,       // y... u... v...
        IMAGE_FORMAT_RGB32,      // b g r a
        IMAGE_FORMAT_ARGB32,     // b g r a
        IMAGE_FORMAT_RGB888,     // r g b
        IMAGE_FORMAT_Grayscale8, // grayscale8
        IMAGE_FORMAT_BGR888,     // b g r(OpenCV Mat Format)
    };

    VideoOpenGL(QWidget *parent = nullptr);
    ~VideoOpenGL();

    void setId(int id) { m_id = id; }
    int getId() const { return m_id; }

    void setImageSize(const int &width, const int &height, const ImageFormat &farmat);
    void setImage(char *data_ptr);
    void setImage(const QImage &image);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    // void paintEvent(QPaintEvent *e) override;

private:
    int m_id = -1;

    struct impl;
    struct impl *m;
};

#endif // VIDEOOPENGL_H