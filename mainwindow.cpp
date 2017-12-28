#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include "vehicle.h"
#include <QPixmap>
#include "chelper.h"
#include "mhelper.h"
#include <QVector2D>
#include <QString>
#include "perlinnoise.h"
#include <cmath>
#include <QVector3D>
#include <QMatrix4x4>
#include "particle.h"

QString GLOBALpath = "/home/shimpe/development/pswarm/pictureswarm/";

void MainWindow::loadImageVehicles(const QString &filename, tp_load_mode loadMode, int threshold)
{
    m_scene->clear();
    m_vehicles.clear();
    m_inputImage.load(filename);
    QSize size = m_inputImage.size();
    for (int row=0; row < size.height(); row+=1)
    {
        for (int col=0; col < size.width(); col+=1)
        {
            QColor AvgColor = m_inputImage.pixelColor(col,row);
            float brightness = AvgColor.value();
            if (brightness > threshold)
            {
                Vehicle *item = nullptr;
                switch(loadMode)
                {
                case from_center:
                    item = new Vehicle(1920/2, 1080/2,
                                       col, row,
                                       1,
                                       AvgColor);
                    break;
                case real_position:
                    item = new Vehicle(col, row,
                                       col, row,
                                       1,
                                       AvgColor);
                    break;
                case random:
                    item = new Vehicle(qrand() % 1920, qrand() % 1080,
                                       col, row,
                                       1,
                                       AvgColor);
                    break;
                case left_side:
                    item = new Vehicle(0, row,
                                       col, row,
                                       1,
                                       AvgColor);
                    break;

                }
                if (item)
                {
                    m_scene->addItem(item);
                    m_vehicles.append(item);
                }
            }
        }
    }
}

void MainWindow::loadImageParticles(const QString &filename, int noOfParticles, Particle::tp_render_mode mode)
{
    m_scene->clear();
    m_particles.clear();
    m_inputImage.load(filename);
    for (int i = 0; i < noOfParticles; ++i)
    {
        Particle *item = new Particle();
        item->setRenderMode(mode);
        m_scene->addItem(item);
        m_particles.append(item);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_frame(0),
    m_saveIdx(0),
    m_movie(dandelion)
{
    ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0,0,1920,1080);
    ui->canvas->setScene(m_scene);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(myUpdate()));
    //float fps = 25.0;
    //m_timer.start(1000/fps); // set framerate
    m_elapsedtime.start();
    m_timer.start(0); // as fast as possible
    m_overalltimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::renderDandelion(const PerlinNoise &p, int startFrame)
{
    int frame = (m_frame - startFrame);
    if (frame < 0)
        return;

    for (auto &v : m_vehicles)
    {
        v->behaviors();
        v->physicsUpdate();
    }

    if (frame == 0)
    {
        loadImageVehicles(GLOBALpath + QString("inputs/dandelion.jpg"), from_center, 20);
    }

    if (frame >= 200 && frame < 280)
    {
        for (auto &v : m_vehicles)
        {
            v->setJitter(mhelper::map(frame, 200, 280, 0, 10));
        }
    }
    else if (frame >= 280 && frame < 420)
    {
        for (auto &v : m_vehicles)
        {
            v->setJitter(mhelper::map(frame, 280, 420, 10, 0));
        }
    }
    else if (frame >= 420 && frame < 620)
    {
        PerlinNoise p;
        for (auto &v : m_vehicles)
        {
            if ((frame-420) % 12)
            {
                QVector2D t = v->getTarget();
                QVector2D n(p.noise(t.x(), t.y(), frame/100.0),
                            p.noise(t.x(), t.y(), 1000-frame/100.0));
                v->setTargetModifier(n*mhelper::map(frame, 420, 619, 10, 0));
            }
        }
    }
    else if (frame >= 620 && frame < 700)
    {
        PerlinNoise p;
        for (auto &v : m_vehicles)
        {
            v->setJitter(mhelper::map(frame, 620, 700, 10, 0));
            QColor currentColor = v->getColor();
            QColor newColor;
            QVector2D target = v->getTarget();
            int noiseVal = (int)(p.noise(target.x()/1000.0, target.y()/1000.0, frame/10.0)*255);
            newColor.setHsv(int(255*(cos(noiseVal*0.02)+1)/2.0), int(255*(sin(noiseVal*0.017)+1)/2.0), currentColor.value(), currentColor.alpha());
            v->setColor(newColor);
            v->setColor(newColor);
        }
    }
    else if (frame >= 700 && frame < 840)
    {
        for (auto &v : m_vehicles)
        {
            v->setJitter(0);
            v->setTargetModifier(QVector2D(0,0));
            v->setTarget(QVector2D(1920/2, 1080/2));
            QColor currentColor = v->getColor();
            QColor newColor;
            QVector2D target = v->getTarget();
            int noiseVal = (int)(p.noise(target.x()/1000.0, target.y()/1000.0, frame/10.0)*255);
            newColor.setHsv(int(255*(cos(noiseVal*0.02)+1)/2.0), int(255*(sin(noiseVal*0.017)+1)/2.0), currentColor.value(), currentColor.alpha());
        }
    }

    saveCurrentFrame();
}

void MainWindow::renderPlanets(const PerlinNoise &/*p*/, int startFrame)
{
    int frame = (m_frame - startFrame);
    if (frame < 0)
        return;

    for (auto &v : m_vehicles)
    {
        v->behaviors();
        v->physicsUpdate();
    }

    if (frame == 0)
    {
        loadImageVehicles(GLOBALpath + QString("inputs/planets.jpg"), left_side, 20);
    }

    if (frame >= 100 && frame < 400)
    {
        if (frame <= 250)
        {
            for (auto &v : m_vehicles)
            {
                QVector2D t = v->getTarget();
                QVector3D v3d = t.toVector3D();
                QVector3D middle(1920/2,1080/2,0);
                QMatrix4x4 rot;
                rot.rotate(360.0/(250-100+1), QVector3D(0,0,1));
                QVector3D rotatedT = rot*(v3d-middle) + middle;
                QVector2D result(rotatedT.x(), rotatedT.y());
                v->setTarget(result);
            }
        }
    }
    else if (frame >= 400 && frame < 500)
    {
        if (frame == 400)
        {
            for (auto &v : m_vehicles)
            {
                v->setTarget(QVector2D(1920/2,1080));
            }
        }
    }

    saveCurrentFrame();
}

void MainWindow::renderParticles(const PerlinNoise &/*noise*/, int startFrame)
{
    int frame = (m_frame - startFrame);
    if (frame < 0)
        return;

    if (frame == 0)
    {
        loadImageParticles(GLOBALpath + QString("inputs/musician.jpg"), 3000, Particle::hexagon);
    }

    if (frame < 500)
    {
        QList<Particle*> newParticles;
        for (auto &p : m_particles)
        {
            Particle *newParticle = static_cast<Particle*>(p)->physicsUpdate(m_inputImage);
            newParticles.append(newParticle);
        }

        m_particles.clear();
        for (auto & np : newParticles)
        {
            m_particles.append(np);
            m_scene->addItem(np);
        }
    }

    saveCurrentFrame();
}

void MainWindow::renderJokerMovie(const PerlinNoise &/*noise*/, int startFrame)
{
    int frame = (m_frame - startFrame);
    if (frame < 0)
        return;

    int maxIterations = 500;

    if (frame % maxIterations == 0)
    {
        int imageindex = int(frame / maxIterations);
        loadImageParticles(GLOBALpath + QString("inputs/jokermovie/joker_%1.jpg").arg(imageindex+1, 5, 10, QChar('0')), 3000, Particle::hexagon);
    }

    if (frame % maxIterations <= (maxIterations - 1))
    {
        QList<Particle*> newParticles;
        for (auto &p : m_particles)
        {
            Particle *newParticle = static_cast<Particle*>(p)->physicsUpdate(m_inputImage);
            newParticles.append(newParticle);
        }

        m_particles.clear();
        for (auto & np : newParticles)
        {
            m_particles.append(np);
            m_scene->addItem(np);
        }
        if (frame < maxIterations)
            saveCurrentFrame(); // first 500 frames: build up first movie frame; next saved frames are only the completely finished movie-frames
    }
    if (frame % maxIterations == (maxIterations-1))
    {
        saveCurrentFrame();
    }
}

void MainWindow::renderAfricanMovie(const PerlinNoise &/*p*/, int startFrame)
{
    int frame = (m_frame - startFrame);
    if (frame < 0)
        return;

    int maxIterations = 300;

    if (frame % maxIterations == 0)
    {
        int imageindex = int(frame / maxIterations);
        loadImageParticles(GLOBALpath + QString("inputs/africanmovie/african_%1.jpg").arg(imageindex+1, 5, 10, QChar('0')), 3000, Particle::open_hexagon);
    }

    if (frame % maxIterations <= (maxIterations - 1))
    {
        QList<Particle*> newParticles;
        for (auto &p : m_particles)
        {
            Particle *newParticle = static_cast<Particle*>(p)->physicsUpdate(m_inputImage);
            newParticles.append(newParticle);
        }

        m_particles.clear();
        for (auto & np : newParticles)
        {
            m_particles.append(np);
            m_scene->addItem(np);
        }
    }
    if (frame % maxIterations == (maxIterations-1))
    {
        saveCurrentFrame();
    }
}

float MainWindow::saveCurrentFrame()
{
    QPixmap pixmap(1920,1080);
    pixmap.fill(Qt::black);
    QPainter painter(&pixmap);
    m_scene->render(&painter, pixmap.rect(), pixmap.rect());
    pixmap.save(GLOBALpath + QString("render/test_%1.png").arg(m_saveIdx, 6, 10, QChar('0')));
    m_saveIdx++;
    float elapsed = m_elapsedtime.elapsed();
    if (!elapsed)
        elapsed = 0.00001;

    qDebug(QString("Saved frame %1 (extension: _%4.jpg) [took %2 ms, framerate: %3 fps]").arg(m_frame).arg(elapsed).arg(1000.0/elapsed).arg(m_saveIdx, 6, 10, QChar('0')).toStdString().c_str());

    return elapsed;
}

void MainWindow::myUpdate()
{
    PerlinNoise p;    
    m_elapsedtime.start();

    //renderDandelion(p, 0); // 840 frames
    //renderPlanets(p, 0);
    //renderParticles(p, 0);
    //renderJokerMovie(p, 0);
    renderAfricanMovie(p, 0);

    if (m_frame >= 500*315)
    {
        m_timer.stop();
        qDebug(QString("Rendered %1 frames in %2 minutes.").arg(m_frame).arg(m_overalltimer.elapsed()/1000/60).toStdString().c_str());
        QApplication::quit();
    }

    if (m_frame % 100 == 0)
    {
        float elapsed = m_elapsedtime.elapsed();
        qDebug(QString("Finished processing frame %1 [took %2 ms, framerate: %3 fps]").arg(m_frame).arg(elapsed).arg(1000.0/elapsed).toStdString().c_str());
    }
    m_frame++;
}
