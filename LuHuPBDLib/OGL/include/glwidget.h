#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <QObject>
#include <QGLWidget>
#include <QTimer>
#include <glm/glm.hpp>

#include <PBDLib>

enum paintType{
    LINES,
    TRIANGLES
};

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent=nullptr);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void BendingConTest();
    void addPlain();
    void addCone();
    void addSmolPlain();
    void addcube();
    bool drawPBDObjects(paintType _type);
    void drawGrid(uint size);

    int time;
    std::vector<double> CamPos;
    std::vector<double> LookAt;
    double anglex=90;
    double angley=0;
    double topangle=100;
    bool simulate=false;

    paintType drawMode=LINES;
private:
    QTimer timer;

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    LuHu::solver * m_solver;
};

#endif // GLWIDGET_H
