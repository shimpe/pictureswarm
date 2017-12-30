#ifndef PARTICLE_H
#define PARTICLE_H

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QVector2D>
#include <QSet>

class QImage;
class QGraphicsScene;

class Particle : public QGraphicsItem
{
public:
    enum tp_render_mode { line = 0, square = 1, hexagon = 2, open_square = 3, open_hexagon = 4, paint_stroke = 5};

    Particle();
    Particle(const Particle &other);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    Particle *physicsUpdate(const QImage &inputImage);

    void setRenderMode(tp_render_mode mode);
    tp_render_mode getRenderMode() const;
    float getPaintStrokeThickness() const;
    void setPaintStrokeThickness(float paintStrokeThickness);
    float getMovement() const;
    void setMovement(float movement);

    bool getStrokeDetailsLighter() const;
    void setStrokeDetailsLighter(bool strokeDetailsLighter);

    int getDetailColorChange() const;
    void setDetailColorChange(int detailColorChange);

    const QSet<int> &getDrawDetail() const;
    void setDrawDetail(const QSet<int> &drawDetail);

    bool getShowMainStroke() const;
    void setShowMainStroke(bool showMainStroke);

    int getMinAlpha() const;
    void setMinAlpha(int minAlpha);

private:
    QVector2D m_pos;
    QVector2D m_prevpos;
    float m_radius;
    QColor m_color;
    QPen m_pen;
    QBrush m_brush;
    tp_render_mode m_renderMode;

    int m_minAlphaForPaintStroke;
    float m_paintStrokeThickness;
    float m_movement;
    bool m_strokeDetailsLighter;
    int m_detailColorChange;
    QSet<int> m_drawDetail;
    bool m_showMainStroke;
};

#endif // PARTICLE_H
