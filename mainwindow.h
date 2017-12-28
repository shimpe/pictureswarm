#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QList>
#include <QElapsedTimer>
#include <QImage>
#include <QGraphicsItem>
#include "particle.h"

class QGraphicsScene;
class Vehicle;
class QString;


namespace Ui {
class MainWindow;
}
class PerlinNoise;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum tp_step { dandelion = 0, face = 1 };
    enum tp_load_mode { from_center = 0, real_position = 1, random = 2, left_side = 3 };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadImageVehicles(const QString &filename, tp_load_mode loadMode, int threshold);
    void loadImageParticles(const QString &filename, int noOfParticles, Particle::tp_render_mode mode);
    float saveCurrentFrame();

    void renderDandelion(const PerlinNoise &p, int startFrame);
    void renderPlanets(const PerlinNoise &p, int startFrame);
    void renderParticles(const PerlinNoise &p, int startFrame);
    void renderJokerMovie(const PerlinNoise &p, int startFrame);
    void renderAfricanMovie(const PerlinNoise &p, int startFrame);

private slots:
    void myUpdate();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    QTimer m_timer;
    QImage m_inputImage;
    QList<Vehicle*> m_vehicles;
    QList<QGraphicsItem*> m_particles;
    QElapsedTimer m_elapsedtime;
    QElapsedTimer m_overalltimer;

    int m_frame;
    int m_saveIdx;

    tp_step m_movie;
};

#endif // MAINWINDOW_H
