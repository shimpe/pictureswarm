#ifndef VEHICLE_H
#define VEHICLE_H

#include <memory>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>


class QVector2D;
class QColor;

class Vehicle : public QGraphicsItem
{
public:
    Vehicle(float sx, float sy, float tx, float ty, float radius, const QColor &color);

    // setup
    void setPosition(const QVector2D &pos);
    void setTarget(const QVector2D &target);
    void setVelocity(const QVector2D &velocity);
    void setAcceleration(const QVector2D &acceleration);
    void setColor(const QColor &color);
    void setRadius(float radius);
    void setAngle(float angle);
    void setMaxSpeed(float maxspeed);
    void setJitter(float jitter);
    void setTargetModifier(const QVector2D &modifier);
    const QVector2D &getPosition() const;
    const QVector2D &getTarget() const;
    const QVector2D &getVelocity() const;
    const QVector2D &getAcceleration() const;
    const QColor &getColor() const;
    float getRadius() const;
    float getMaxSpeed() const;
    float getAngle() const;
    float getJitter() const;
    const QVector2D &getTargetModifier() const;

    void setSeekTarget(const QVector2D &seekTarget);
    void setSeekRadius(float radius);
    void toggleSeek(bool onOff);

    void setFleeTarget(const QVector2D &fleeTarget);
    void setFleeRadius(float Radius);
    void toggleFlee(bool onOff);

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // physics
    void behaviors();
    void applyForce(const QVector2D &Force);
    void physicsUpdate();
    void updateGfx();

    // behaviors
    QVector2D seek(const QVector2D &target, float max_distance);
    QVector2D flee(const QVector2D &target, float max_distance);
    QVector2D arrive(const QVector2D &target, float max_distance);

private:
    std::unique_ptr<QVector2D> m_position;
    std::unique_ptr<QVector2D> m_target;
    std::unique_ptr<QVector2D> m_velocity;
    std::unique_ptr<QVector2D> m_acceleration;
    std::unique_ptr<QColor> m_color;
    std::unique_ptr<QVector2D> m_targetModifier;
    float m_radius;
    float m_angle;
    float m_maxspeed;
    float m_maxforce;

    QPen m_pen;
    QBrush m_brush;

    bool m_seekActive;
    float m_seekRadius;
    std::unique_ptr<QVector2D> m_seekTarget;

    bool m_fleeActive;
    float m_fleeRadius;
    std::unique_ptr<QVector2D> m_fleeTarget;

    float m_jitter;
};

#endif // VEHICLE_H
