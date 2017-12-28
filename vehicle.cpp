#include "vehicle.h"
#include "vhelper.h"
#include "mhelper.h"
#include <QVector2D>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QColor>
#include <QPainter>

Vehicle::Vehicle(float startx, float starty, float targetx, float targety, float radius, const QColor &color)
    : m_position(std::make_unique<QVector2D>(startx,starty))
    , m_target(std::make_unique<QVector2D>(targetx,targety))
    , m_velocity(std::make_unique<QVector2D>(0,0))
    , m_acceleration(std::make_unique<QVector2D>(0,0))
    , m_color(std::make_unique<QColor>(color))
    , m_targetModifier(std::make_unique<QVector2D>(0,0))
    , m_radius(radius)
    , m_angle(0)
    , m_maxspeed(20)
    , m_maxforce(2)
    , m_pen(QPen(*m_color))
    , m_brush(QBrush(*m_color, Qt::SolidPattern))
    , m_seekActive(false)
    , m_seekRadius(400)
    , m_seekTarget(std::make_unique<QVector2D>(0,0))
    , m_fleeActive(false)
    , m_fleeRadius(400)
    , m_fleeTarget(std::make_unique<QVector2D>(0,0))
    , m_jitter(0)
{
}

void Vehicle::setPosition(const QVector2D &pos)
{
    *m_position = pos;
    setPos(m_position->x(), m_position->y());
    //update();
}

void Vehicle::setTarget(const QVector2D &target)
{
    *m_target = target;
}

void Vehicle::setVelocity(const QVector2D &velocity)
{
    *m_velocity = velocity;
}

void Vehicle::setAcceleration(const QVector2D &acceleration)
{
    *m_acceleration = acceleration;
}

void Vehicle::setColor(const QColor &color)
{
    *m_color = color;
    m_pen = QPen(*m_color);
    m_brush = QBrush(*m_color, Qt::SolidPattern);
    //update();
}

void Vehicle::setRadius(float radius)
{
    m_radius = radius;
    //update();
}

void Vehicle::setAngle(float angle)
{
    m_angle = angle;
    //update();
}

void Vehicle::setMaxSpeed(float maxspeed)
{
    m_maxspeed = maxspeed;
}

void Vehicle::setJitter(float jitter)
{
    m_jitter = jitter;
}

void Vehicle::setTargetModifier(const QVector2D &modifier)
{
    *m_targetModifier = modifier;
}

const QVector2D &Vehicle::getPosition() const
{
    return *m_position;
}

const QVector2D &Vehicle::getTarget() const
{
    return *m_target;
}

const QVector2D &Vehicle::getVelocity() const
{
    return *m_velocity;
}

const QVector2D &Vehicle::getAcceleration() const
{
    return *m_acceleration;
}

const QColor &Vehicle::getColor() const
{
    return *m_color;
}

float Vehicle::getRadius() const
{
    return m_radius;
}

float Vehicle::getMaxSpeed() const
{
    return m_maxspeed;
}

float Vehicle::getAngle() const
{
    return m_angle;
}

float Vehicle::getJitter() const
{
    return m_jitter;
}

const QVector2D &Vehicle::getTargetModifier() const
{
    return *m_targetModifier;
}

void Vehicle::setSeekTarget(const QVector2D &seekTarget)
{
    *m_seekTarget = seekTarget;
}

void Vehicle::setSeekRadius(float radius)
{
    m_seekRadius = radius;
}

void Vehicle::toggleSeek(bool onOff)
{
    m_seekActive = onOff;
}

void Vehicle::setFleeTarget(const QVector2D &fleeTarget)
{
    *m_fleeTarget = fleeTarget;

}

void Vehicle::setFleeRadius(float Radius)
{
    m_fleeRadius = Radius;
}

void Vehicle::toggleFlee(bool onOff)
{
    m_fleeActive = onOff;
}

QRectF Vehicle::boundingRect() const
{
    return QRectF(0,0,m_radius,m_radius);
}

void Vehicle::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    QRectF rect = boundingRect();
    painter->rotate(m_angle);
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawRect(rect);
    painter->resetTransform();
}

void Vehicle::behaviors()
{
    QVector2D force_arrive = arrive(*m_target + *m_targetModifier, 100);
    applyForce(force_arrive);

    if (m_seekActive)
    {
        QVector2D force_seek = seek(*m_seekTarget, m_seekRadius);
        applyForce(force_seek);
    }

    if (m_fleeActive)
    {
        QVector2D force_flee = flee(*m_fleeTarget, m_fleeRadius);
        applyForce(force_flee);
    }
}

void Vehicle::applyForce(const QVector2D &force)
{
    QVector2D v;
    v.setX((qrand() % 10000 - 5000)/5000.0 * m_jitter);
    v.setY((qrand() % 10000 - 5000)/5000.0 * m_jitter);
    v += force;
    *m_acceleration += v;
}

void Vehicle::physicsUpdate()
{
    *m_position += *m_velocity;
    *m_velocity += *m_acceleration;
    *m_acceleration *= 0;
    setPos(m_position->x(), m_position->y());
}

void Vehicle::updateGfx()
{
    update();
}

QVector2D Vehicle::seek(const QVector2D &target, float max_distance)
{
    QVector2D desired = target - *m_position;
    float distance = desired.length();
    if (distance < max_distance)
    {
        desired = vhelper::setMag(desired, m_maxspeed);
        QVector2D steer = desired - *m_velocity;
        steer = vhelper::limit(steer, m_maxforce);
        return steer;
    }
    return QVector2D(0,0);
}

QVector2D Vehicle::flee(const QVector2D &target, float max_distance)
{
    QVector2D desired = target - *m_position;
    float distance = desired.length();
    if (distance < max_distance)
    {
        desired = vhelper::setMag(desired, m_maxspeed);
        desired *= -1;
        QVector2D steer = desired - *m_velocity;
        steer = vhelper::limit(steer, m_maxforce);
        return steer;
    }
    return QVector2D(0,0);
}

QVector2D Vehicle::arrive(const QVector2D &target, float max_distance)
{
    QVector2D desired = target - *m_position;
    float distance_to_target = desired.length();
    float speed = m_maxspeed;
    if (distance_to_target < max_distance)
    {
        speed = mhelper::map(distance_to_target, 0, max_distance, 0, m_maxspeed);
    }
    desired = vhelper::setMag(desired, speed);
    QVector2D steer = desired - *m_velocity;
    steer = vhelper::limit(steer, m_maxforce);
    return steer;
}
