#include "particle.h"
#include "vhelper.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QRectF>
#include <cmath>
#include <QtMath>

Particle::Particle()
    : m_pos(QVector2D(qrand() % 1920, qrand() % 1080))
    , m_prevpos(m_pos)
    , m_radius(1)
    , m_color(QColor(255,255,255,255))
    , m_pen(QPen(m_color, m_radius))
    , m_brush(QBrush(m_color, Qt::SolidPattern))
    , m_renderMode(line)
{
}

Particle::Particle(const Particle &other)
    :  QGraphicsItem()
    ,  m_pos(other.m_pos)
    , m_prevpos(other.m_pos)
    , m_radius(other.m_radius)
    , m_color(other.m_color)
    , m_pen(other.m_pen)
    , m_brush(other.m_brush)
    , m_renderMode(other.m_renderMode)
{
}

QRectF Particle::boundingRect() const
{
    return QRectF(0,0,m_radius,m_radius);
}

void Particle::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    if (m_renderMode == line)
        painter->drawLine(m_prevpos.toPointF(), m_pos.toPointF());
    else if (m_renderMode == square)
        painter->drawRect(QRectF(m_prevpos.toPointF(), m_pos.toPointF()));
    else if (m_renderMode == hexagon)
    {
        QVector<QPointF> points;
        float radius = (m_prevpos-m_pos).length();
        QVector2D midpoint = (m_prevpos+m_pos)/2.0;
        for (int i = 0; i < 6; i++)
        {
            float x = radius*cos(qDegreesToRadians(i*360.0/6.0));
            float y = radius*sin(qDegreesToRadians(i*360.0/6.0));
            QVector2D corner = midpoint + QVector2D(x,y);
            points.append(corner.toPointF());
        }
        QPolygonF polygon(points);
        painter->drawConvexPolygon(polygon);
    }
    painter->resetTransform();
}

Particle *Particle::physicsUpdate(const QImage &inputImage)
{
    m_prevpos = m_pos;
    m_pos += QVector2D((qrand() % 20) - 10, (qrand() % 20) - 10);
    m_pos = vhelper::limit(m_pos, 0, 1919, 0, 1079);
    m_color = inputImage.pixelColor(m_pos.toPoint());
    if (m_renderMode == line)
        m_color.setAlpha(int(255/4));
    else if (m_renderMode == square || m_renderMode == hexagon)
        m_color.setAlpha(int(255/10));
    m_pen = QPen(m_color, m_radius);
    m_brush = QBrush(m_color, Qt::SolidPattern);
    Particle *newParticle = new Particle(*this);
    return newParticle;
}

void Particle::setRenderMode(Particle::tp_render_mode mode)
{
    m_renderMode = mode;
}

Particle::tp_render_mode Particle::getRenderMode() const
{
    return m_renderMode;
}

