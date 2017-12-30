#include "particle.h"
#include "vhelper.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QRectF>
#include <cmath>
#include <QtMath>
#include "mhelper.h"

Particle::Particle()
    : m_pos(QVector2D(qrand() % 1920, qrand() % 1080))
    , m_prevpos(m_pos)
    , m_radius(1)
    , m_color(QColor(255,255,255,255))
    , m_pen(QPen(m_color, m_radius))
    , m_brush(QBrush(m_color, Qt::SolidPattern))
    , m_renderMode(line)
    , m_minAlphaForPaintStroke(0)
    , m_paintStrokeThickness(40)
    , m_movement(10)
    , m_strokeDetailsLighter(false)
    , m_detailColorChange(20)
    , m_showMainStroke(true)
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
    , m_minAlphaForPaintStroke(other.m_minAlphaForPaintStroke)
    , m_paintStrokeThickness(other.m_paintStrokeThickness)
    , m_movement(10)
    , m_strokeDetailsLighter(other.m_strokeDetailsLighter)
    , m_detailColorChange(other.m_detailColorChange)
    , m_showMainStroke(other.m_showMainStroke)

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
    else if (m_renderMode == open_square)
    {
        painter->setPen(m_pen);
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->drawRect(QRectF(m_prevpos.toPointF(), m_pos.toPointF()));
    }
    else if (m_renderMode == open_hexagon)
    {
        painter->setBrush(QBrush(Qt::NoBrush));
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
    else if (m_renderMode == paint_stroke)
    {
        int thickness = m_paintStrokeThickness;
        if (thickness < 1)
            thickness = 1;
        m_pen.setWidth(thickness);
        painter->setPen(m_pen);
        if (m_showMainStroke)
            painter->drawLine(m_prevpos.toPointF(), m_pos.toPointF());
        QColor c = m_pen.color();
        if (m_strokeDetailsLighter && m_showMainStroke)
            c = c.lighter(100 + m_detailColorChange);
        else if (m_showMainStroke)
            c = c.darker(100 + m_detailColorChange);
        for (int i = 0; i < thickness; ++i)
        {
            float xoffs = mhelper::map(i, 0, thickness-1, -thickness/2.0f, thickness/2.0f);
            float yoffs = mhelper::map(i, 0, thickness-1, -thickness/2.0f, thickness/2.0f);
            if (m_drawDetail.find(i) != m_drawDetail.end())
            {
                m_pen.setWidth(thickness/12 >= 1 ? thickness/12 : 1);
                painter->setPen(m_pen);
                painter->drawLine((m_prevpos + QVector2D(xoffs,yoffs)).toPointF(),
                                  (m_pos + QVector2D(xoffs, yoffs)).toPointF());
            }
        }
    }
    painter->resetTransform();
}

Particle *Particle::physicsUpdate(const QImage &inputImage)
{
    m_prevpos = m_pos;
    int sign1 = qrand() % 2 < 1 ? -1 : 1;
    int sign2 = qrand() % 2 < 1 ? -1 : 1;
    QVector2D displacement = QVector2D((qrand() % int(m_movement))*sign1, (qrand() % int(m_movement))*sign2);
    displacement = vhelper::limitmin(displacement, m_movement/3.0);
    m_pos += displacement;
    m_pos = vhelper::limit(m_pos, 0, 1919, 0, 1079);
    m_color = inputImage.pixelColor(m_pos.toPoint());
    if (m_renderMode == line)
        m_color.setAlpha(int(255/4));
    else if (m_renderMode == square || m_renderMode == hexagon || m_renderMode == open_square || m_renderMode == open_hexagon)
        m_color.setAlpha(int(255/10));
    else if (m_renderMode == paint_stroke)
    {
        if (!m_minAlphaForPaintStroke)
            m_color.setAlpha(100 + (qrand() % 100));
        else
            m_color.setAlpha(m_minAlphaForPaintStroke + (qrand() % (255-m_minAlphaForPaintStroke)));
    }
    if (m_renderMode != paint_stroke)
        m_pen = QPen(m_color, m_radius);
    else
        m_pen = QPen(m_color, m_radius, Qt::SolidLine,Qt::RoundCap);
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

float Particle::getPaintStrokeThickness() const
{
    return m_paintStrokeThickness;
}

void Particle::setPaintStrokeThickness(float paintStrokeThickness)
{
    m_paintStrokeThickness = paintStrokeThickness;
}

float Particle::getMovement() const
{
    return m_movement;
}

void Particle::setMovement(float movement)
{
    m_movement = movement;
}

bool Particle::getStrokeDetailsLighter() const
{
    return m_strokeDetailsLighter;
}

void Particle::setStrokeDetailsLighter(bool strokeDetailsLighter)
{
    m_strokeDetailsLighter = strokeDetailsLighter;
}

int Particle::getDetailColorChange() const
{
    return m_detailColorChange;
}

void Particle::setDetailColorChange(int detailColorChange)
{
    m_detailColorChange = detailColorChange;
}

const QSet<int> &Particle::getDrawDetail() const
{
    return m_drawDetail;
}

void Particle::setDrawDetail(const QSet<int> &drawDetail)
{
    m_drawDetail = drawDetail;
}

bool Particle::getShowMainStroke() const
{
    return m_showMainStroke;
}

void Particle::setShowMainStroke(bool showMainStroke)
{
    m_showMainStroke = showMainStroke;
}

int Particle::getMinAlpha() const
{
    return m_minAlphaForPaintStroke;
}

void Particle::setMinAlpha(int minAlpha)
{
    m_minAlphaForPaintStroke = minAlpha;
}

