#ifndef PARTICLE_H
#define PARTICLE_H

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QVector2D>

class QImage;
class QGraphicsScene;

class Particle : public QGraphicsItem
{
public:
    enum tp_render_mode { line = 0, square = 1, hexagon = 2};

    Particle();
    Particle(const Particle &other);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    Particle *physicsUpdate(const QImage &inputImage);
    void setRenderMode(tp_render_mode mode);
    tp_render_mode getRenderMode() const;

private:
    QVector2D m_pos;
    QVector2D m_prevpos;
    float m_radius;
    QColor m_color;
    QPen m_pen;
    QBrush m_brush;
    tp_render_mode m_renderMode;
};

#endif // PARTICLE_H
